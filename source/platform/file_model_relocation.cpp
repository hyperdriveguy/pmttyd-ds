#include "file_model_relocation.hpp"
namespace ttyd {
void setPackedFileModelPointers(uint32_t base,const EventMemoryServices& m){
 // Source pointer ordering is the relocation sentinel, not a separate flag.
 // Add base to ALL25 root words0x14c..0x1ac, including zero/unused sentinels.
 // Signed count+0x148 controls0x40-byte records; +0x3c holds child pointer.
 const auto first=m.word(m.user,base+0x14c);if(first>=base)return;
 m.storeWord(m.user,base+0x14c,first+base);
 for(uint32_t o=0x150;o<=0x1ac;o+=4)m.storeWord(m.user,base+o,m.word(m.user,base+o)+base);
 // Signed COUNT is captured after root publication; table and child are live.
 const auto count=m.word(m.user,base+0x148);
 if(!count || count>0x7fffffff)return;
 uint32_t offset=0;
 for(uint32_t remaining=count;remaining;--remaining,offset+=0x40){
  const auto slot=m.word(m.user,base+0x1ac)+offset+0x3c;
  m.storeWord(m.user,slot,m.word(m.user,slot)+base);
  const auto child=m.word(m.user,m.word(m.user,base+0x1ac)+offset+0x3c);
  // Nested first pointer+0x24 compares against CHILD base. Relocate all
  // eight child words0x24..0x40 if relative; shared/aliased source graphs keep
  // their observable stores and reloads rather than a precomputed rewrite.
  const auto firstChild=m.word(m.user,child+0x24);
  if(firstChild>=child)continue;
  m.storeWord(m.user,child+0x24,firstChild+child);
  for(uint32_t o=0x28;o<=0x40;o+=4)m.storeWord(m.user,child+o,m.word(m.user,child+o)+child);
 }
}
// Reverse nested relocation before root words, table pointer last. Count is
// signed-positive only. Parent pointer store may alias child state: retain the
// captured absolute child here, unlike Set's post-store child reload.
void clearPackedFileModelPointers(uint32_t base,const EventMemoryServices& m){
 if(m.word(m.user,base+0x14c)<base)return;
 const auto count=m.word(m.user,base+0x148);
 if(count && count<=0x7fffffff){
  uint32_t offset=0;
  for(uint32_t remaining=count;remaining;--remaining,offset+=0x40){
   const auto slot=m.word(m.user,base+0x1ac)+offset+0x3c;
   // Clear retains absolute C across the parent store; Set reloads it.
   const auto child=m.word(m.user,slot);m.storeWord(m.user,slot,child-base);
   const auto firstChild=m.word(m.user,child+0x24);
   if(firstChild<child)continue;
   m.storeWord(m.user,child+0x24,firstChild-child);
   for(uint32_t o=0x28;o<=0x40;o+=4)m.storeWord(m.user,child+o,m.word(m.user,child+o)-child);
  }
 }
 // Fresh first word after child aliases, then table pointer last.
 for(uint32_t o=0x14c;o<=0x1ac;o+=4)m.storeWord(m.user,base+o,m.word(m.user,base+o)-base);
}
}
