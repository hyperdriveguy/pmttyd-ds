#include "file_async.hpp"
namespace ttyd {
namespace {
bool same(const FileAsyncServices& a,const FileAsyncServices& b){
 const auto& x=a.file;const auto& y=b.file;
 return x.memory.user==y.memory.user && x.memory.word==y.memory.word && x.memory.half==y.memory.half && x.memory.storeWord==y.memory.storeWord && x.memory.storeHalf==y.memory.storeHalf && x.memory.storeByte==y.memory.storeByte && x.user==y.user && x.byte==y.byte && x.compare==y.compare && x.copy==y.copy && x.garbage==y.garbage && x.archive==y.archive && x.relocate==y.relocate && x.open==y.open && x.length==y.length && x.allocate==y.allocate && x.close==y.close && a.submit==b.submit && a.callback==b.callback;
}
}
bool requestPackedFile(uint32_t path,uint32_t type,uint32_t completion,const FileAsyncServices& s,FileAsyncCursor& c,uint32_t& result){
 const auto& f=s.file;const auto& m=f.memory;
 auto word=[&](uint32_t a){return m.word(m.user,a);};auto put=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};auto manager=[&](){return word(0x80418498);};
 auto finish=[&](uint32_t v){result=v;c={};return true;};
 if(c.phase && (c.path!=path || c.type!=type || c.completion!=completion || !same(c.owner,s)))return false;
 if(!c.phase){c.owner=s;c.path=path;c.type=type;c.completion=completion;c.entry=word(manager()+8);c.phase=1;}
 for(;;)switch(c.phase){
 case 1:if(!c.entry){c.phase=4;break;}if(f.byte(m.user,c.entry)){c.phase=2;break;}c.entry=word(c.entry+0xa4);break;
 case 2:{if(!f.compare)return false;auto v=f.compare(f.user,c.entry+0x20,path);if(!v)return false;if(!*v)c.phase=3;else{c.entry=word(c.entry+0xa4);c.phase=1;}break;}
 case 3:if(word(c.entry+0xac))return finish(0);if(f.byte(m.user,c.entry)==2)m.storeByte(m.user,c.entry,3);return finish(c.entry);
 case 4:c.entry=word(manager()+0x10);c.phase=c.entry?7:5;break;
 case 5:if(!f.garbage || !f.garbage(f.user,1))return false;c.entry=word(manager()+0x10);c.phase=c.entry?7:6;break;
 case 6:if(!f.garbage || !f.garbage(f.user,0))return false;c.entry=word(manager()+0x10);c.phase=7;break;
 case 7:{const auto current=manager();c.next=word(c.entry+0xa4);const auto mode=word(current+4);c.trials=mode==0?1:(mode==1 || mode==2?3:0);c.attempt=0;c.phase=c.trials?8:12;break;}
 case 8:{if(!f.archive)return false;auto p=f.archive(f.user,path,0,0);if(!p)return false;c.payload=*p;++c.attempt;c.phase=c.payload?9:c.attempt<c.trials?8:12;break;}
 case 9:if(!f.relocate || !f.relocate(f.user,c.payload,type))return false;c.phase=10;break;
 case 10:m.storeByte(m.user,c.entry,3);put(c.entry+0xa0,c.entry+4);put(word(c.entry+0xa0),c.payload);m.storeHalf(m.user,c.entry+2,0);m.storeByte(m.user,c.entry+1,uint8_t(type));put(c.entry+0xa4,0);c.phase=11;break;
 case 11:if(!f.copy || !f.copy(f.user,c.entry+0x20,path))return false;put(c.entry+0xac,0);c.phase=20;break;
 case 12:{if(!f.open)return false;auto d=f.open(f.user,path,2,0);if(!d)return false;c.disc=*d;if(!c.disc)return finish(0xffffffff);c.phase=13;break;}
 case 13:{if(!f.length)return false;auto n=f.length(f.user,c.disc);if(!n)return false;c.rounded=*n;if(c.rounded&31)c.rounded+=32-(c.rounded&31);c.phase=14;break;}
 case 14:if(!f.close || !f.close(f.user,c.disc))return false;if(!c.rounded)return finish(0);if(c.rounded&31)c.rounded+=32-(c.rounded&31);c.phase=15;break;
 case 15:{if(!f.allocate)return false;auto h=f.allocate(f.user,c.rounded,0);if(!h)return false;c.header=*h;put(c.header+8,c.entry);put(c.entry+0xa0,c.header);m.storeByte(m.user,c.entry,3);m.storeHalf(m.user,c.entry+2,0);m.storeByte(m.user,c.entry+1,uint8_t(type));put(c.entry+0xa4,0);put(c.entry+0xa8,completion);c.phase=16;break;}
 case 16:if(!f.copy || !f.copy(f.user,c.entry+0x20,path))return false;c.phase=21;break;
 // The archive branch pops the CURRENT head's live next AFTER clearing
 // retained E.a4. The DVD branch instead publishes captured NEXT.
 case 20:{const auto current=manager();const auto head=word(current+0x10);put(current+0x10,word(head+0xa4));c.phase=22;break;}
 case 21:put(manager()+0x10,c.next);c.phase=22;break;
 case 22:{auto current=manager();if(!word(current+0x10))put(current+0x14,0);current=manager();if(!word(current+8)){put(current+8,c.entry);put(manager()+0xc,c.entry);}else{put(word(current+0xc)+0xa4,c.entry);put(manager()+0xc,c.entry);}if(c.payload)return finish(c.entry);c.phase=23;break;}
 case 23:{if(!f.open)return false;auto d=f.open(f.user,path,2,0);if(!d)return false;put(c.entry+0xac,*d);c.phase=24;break;}
 case 24:c.disc=word(c.entry+0xac);c.destination=word(c.header);c.phase=25;break;
 case 25:if(!s.submit || !s.submit(f.user,c.disc,c.destination,c.rounded,0,0x8018a1a0))return false;return finish(0);
 default:return false;
 }
}
void submitPackedDvdRead(uint32_t d,uint32_t to,uint32_t n,uint32_t off,uint32_t callback,const EventMemoryServices& m){
 m.storeWord(m.user,d+0x7c,to);m.storeWord(m.user,d+0x80,n);m.storeWord(m.user,d+0x84,off);
 m.storeHalf(m.user,d+0x90,uint16_t(m.half(m.user,d+0x90)|1));m.storeHalf(m.user,d+0x90,uint16_t(m.half(m.user,d+0x90)&~2u));m.storeWord(m.user,d+0x8c,callback);m.storeWord(m.user,d+0x88,0);
}
bool completePackedFile(uint32_t result,uint32_t info,const FileAsyncServices& s,FileAsyncCompletionCursor& c){
 const auto& f=s.file;const auto& m=f.memory;auto word=[&](uint32_t a){return m.word(m.user,a);};auto put=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};
 if(c.phase && (c.result!=result || c.info!=info || !same(c.owner,s)))return false;
 if(!c.phase){c.owner=s;c.result=result;c.info=info;c.entry=word(word(0x80418498)+8);c.phase=1;}
 for(;;)switch(c.phase){
 case 1:if(!c.entry || word(c.entry+0xac)+0x40==info){c.disc=word(c.entry+0xac);put(c.entry+0xac,0);if(result==0xfffffffd){m.storeByte(m.user,c.entry,2);m.storeByte(m.user,c.entry+1,0xb);c.phase=5;}else{const auto h=word(c.entry+0xa0);const auto t=f.byte(m.user,c.entry+1);c.type=t<128?t:uint32_t(int32_t(t)-256);c.payload=word(h);c.phase=2;}break;}c.entry=word(c.entry+0xa4);break;
 case 2:if(!f.relocate || !f.relocate(f.user,c.payload,c.type))return false;c.phase=3;break;
 case 3:if(!f.close || !f.close(f.user,c.disc))return false;c.callback=word(c.entry+0xa8);c.phase=c.callback?4:6;break;
 case 4:if(!s.callback || !s.callback(f.user,c.callback,c.entry))return false;c.phase=6;break;
 case 5:if(!f.close || !f.close(f.user,c.disc))return false;c.phase=6;break;
 case 6:c={};return true;
 default:return false;
 }
}
}
