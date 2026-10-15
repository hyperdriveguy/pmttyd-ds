#include "file_acquisition.hpp"
namespace ttyd {
namespace {
bool same(const FileAcquisitionServices& a,const FileAcquisitionServices& b){
  const auto& x=a.memory;const auto& y=b.memory;
  return a.user==b.user && a.byte==b.byte && x.user==y.user && x.word==y.word &&
    x.half==y.half && x.storeWord==y.storeWord && x.storeHalf==y.storeHalf &&
    x.storeByte==y.storeByte && a.compare==b.compare && a.copy==b.copy &&
    a.yield==b.yield && a.garbage==b.garbage && a.archive==b.archive &&
    a.relocate==b.relocate && a.open==b.open && a.length==b.length &&
    a.allocate==b.allocate && a.read==b.read && a.close==b.close;
}
// Native continuation steps, not original file states or event return codes.
// Exact captured entry/next/payload/operands survive child holds; guest manager
// loads remain fresh where source reloads the global pointer.
enum Phase : uint32_t {Scan=1,Compare,Pending,Yield,Reuse,Available,CollectOne,
  CollectZero,CaptureNext,Archive,RelocateArchive,InitializeArchive,Open,Length,
  CloseFailure,Allocate,Read,Close,InitializeDisc,CopyArchive,CopyDisc,
  RelocateDisc,Publish};
}
bool acquirePackedFile(uint32_t path,uint32_t type,const FileAcquisitionServices& s,
                       FileAcquisitionCursor& c,uint32_t& result){
  const auto& m=s.memory;
  if(c.phase && (c.path!=path || c.type!=type || !same(c.owner,s)))return false;
  auto word=[&](uint32_t a){return m.word(m.user,a);};
  auto put=[&](uint32_t a,uint32_t n){m.storeWord(m.user,a,n);};
  // Global0x80418498 -> manager: +4 archive mode, +8/+c used head/tail,
  // +0x10/+0x14 available head/tail. Entry: state byte0, type byte1, refs half2,
  // filename+0x20, payload holder+0xa0, next+0xa4, pending gate+0xac.
  // These are guest address tokens, not host pointers or a complete entry ABI.
  auto manager=[&](){return word(0x80418498);};
  auto finish=[&](uint32_t value){result=value;c={};return true;};
  if(!c.phase){c={};c.path=path;c.type=type;c.owner=s;c.entry=word(manager()+8);c.phase=Scan;}
  for(;;)switch(c.phase){
  // First nonzero-state matching FILENAME wins; requested type is not part
  // of cache identity. Capture used head once, then follow live next words.
  case Scan:
    if(!c.entry){c.phase=Available;break;}
    if(s.byte(m.user,c.entry)){c.phase=Compare;break;}
    c.entry=word(c.entry+0xa4);break;
  case Compare:{
    if(!s.compare)return false;
    const auto comparison=s.compare(s.user,c.entry+0x20,c.path);
    if(!comparison)return false;
    if(*comparison==0)c.phase=Pending;
    else {c.entry=word(c.entry+0xa4);c.phase=Scan;}
    break;
  }
  case Pending:
    c.phase=word(c.entry+0xac)?Yield:Reuse;break;
  // Poll the same retained entry's pending word after actual OSYieldThread
  // acknowledgement. No timeout, simulated tick credit or invented readiness.
  case Yield:
    if(!s.yield || !s.yield(s.user))return false;
    c.phase=Pending;break;
  // State3 reactivates and resets refs; state2 reactivates preserving refs.
  // Then increment low16 for every admitted current state, without clamping,
  // recopying filename, changing type or repositioning the used-chain entry.
  case Reuse:{
    const auto state=s.byte(m.user,c.entry);
    if(state==3){m.storeByte(m.user,c.entry,1);m.storeHalf(m.user,c.entry+2,0);}
    else if(state==2)m.storeByte(m.user,c.entry,1);
    const auto count=m.half(m.user,c.entry+2);
    m.storeHalf(m.user,c.entry+2,uint16_t(uint32_t(count)+1));
    return finish(c.entry);
  }
  // Miss: try available head, garbage(1), then garbage(0), each with fresh
  // manager reads. Source has NO final null-entry guard before CaptureNext;
  // unsupported native provisioning must not become fabricated failure0.
  case Available:
    c.entry=word(manager()+0x10);c.phase=c.entry?CaptureNext:CollectOne;break;
  case CollectOne:
    if(!s.garbage || !s.garbage(s.user,1))return false;
    c.entry=word(manager()+0x10);c.phase=c.entry?CaptureNext:CollectZero;break;
  case CollectZero:
    if(!s.garbage || !s.garbage(s.user,0))return false;
    c.entry=word(manager()+0x10);c.phase=CaptureNext;break;
  // Retain next available link before loading. Archive mode0 tries once,
  // modes1/2 three times, other modes go directly to disc. A real archive0
  // advances the attempt; nullopt retains the current attempt unchanged.
  case CaptureNext:{
    const auto current=manager();c.next=word(c.entry+0xa4);
    const auto mode=word(current+4);
    c.trials=mode==0?1:(mode==1 || mode==2?3:0);c.attempt=0;
    c.phase=c.trials?Archive:Open;break;
  }
  case Archive:{
    if(!s.archive)return false;
    const auto payload=s.archive(s.user,c.path,0,0);if(!payload)return false;
    c.payload=*payload;++c.attempt;
    c.phase=c.payload?RelocateArchive:(c.attempt<c.trials?Archive:Open);break;
  }
  case RelocateArchive:
    if(!s.relocate || !s.relocate(s.user,c.payload,c.type))return false;
    c.phase=InitializeArchive;break;
  // Archive stores embedded holder entry+4, while disc uses smart header.
  // Holder contains payload pointer; returned result is the ENTRY, not payload.
  // Filename/cursor/list writes are source ordered, not a transaction.
  case InitializeArchive:
    m.storeByte(m.user,c.entry,1);put(c.entry+0xa0,c.entry+4);
    put(word(c.entry+0xa0),c.payload);
    m.storeHalf(m.user,c.entry+2,1);m.storeByte(m.user,c.entry+1,uint8_t(c.type));
    put(c.entry+0xa4,0);c.phase=CopyArchive;break;
  case Open:{
    if(!s.open)return false;
    const auto disc=s.open(s.user,c.path,2,0);if(!disc)return false;
    c.disc=*disc;if(!c.disc)return finish(0);
    c.phase=Length;break;
  }
  // Disc open(mode2,flags0) failure0 completes without list publication.
  // Length rounds upward to32 bytes with word wrapping; rounded0 closes then
  // fails. Preserve the source's repeated alignment and missing header0 guard.
  case Length:{
    if(!s.length)return false;
    const auto length=s.length(s.user,c.disc);if(!length)return false;
    c.rounded=*length;
    if(c.rounded&31)c.rounded+=32-(c.rounded&31);
    if(!c.rounded){c.phase=CloseFailure;break;}
    // The original repeats the unsigned alignment before smartAlloc.
    if(c.rounded&31)c.rounded+=32-(c.rounded&31);
    c.phase=Allocate;break;
  }
  case CloseFailure:
    if(!s.close || !s.close(s.user,c.disc))return false;
    return finish(0);
  // Smart header+0 points to destination, +8 gets entry backlink. Source
  // accepts any engaged header (even0); no invented allocator retry/null branch.
  case Allocate:{
    if(!s.allocate)return false;
    const auto header=s.allocate(s.user,c.rounded,0);if(!header)return false;
    c.header=*header;put(c.header+8,c.entry);c.destination=word(c.header);
    c.phase=Read;break;
  }
  case Read:
    if(!s.read || !s.read(s.user,c.disc,c.destination,c.rounded,0))return false;
    c.phase=Close;break; // A present read result0 is ignored by source.
  case Close:
    if(!s.close || !s.close(s.user,c.disc))return false;
    c.phase=InitializeDisc;break;
  case InitializeDisc:
    put(c.entry+0xa0,c.header);m.storeByte(m.user,c.entry,1);
    m.storeHalf(m.user,c.entry+2,1);m.storeByte(m.user,c.entry+1,uint8_t(c.type));
    put(c.entry+0xa4,0);c.phase=CopyDisc;break;
  case CopyArchive:
  case CopyDisc:{
    if(!s.copy || !s.copy(s.user,c.entry+0x20,c.path))return false;
    put(c.entry+0xac,0);
    if(c.phase==CopyArchive)c.phase=Publish;
    else {c.payload=word(c.header);c.phase=RelocateDisc;}
    break;
  }
  case RelocateDisc:
    if(!s.relocate || !s.relocate(s.user,c.payload,c.type))return false;
    c.phase=Publish;break;
  // Pop retained next, clear available tail if empty, append used entry.
  // Each manager() is a source reload; providers may have changed global owner.
  // Do not snapshot manager or undo earlier writes on backend failure/hold.
  case Publish:{
    put(manager()+0x10,c.next);
    auto current=manager();if(!word(current+0x10))put(current+0x14,0);
    current=manager();
    if(!word(current+8)){
      put(current+8,c.entry);put(manager()+0xc,c.entry);
    }else{
      put(word(current+0xc)+0xa4,c.entry);put(manager()+0xc,c.entry);
    }
    return finish(c.entry);
  }
  default:return false;
  }
}
} // namespace ttyd
