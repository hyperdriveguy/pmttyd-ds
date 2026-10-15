#include "file_handle_release.hpp"
namespace ttyd {
void releasePackedFileHandle(uint32_t entry,const FileHandleReleaseServices& s){
  if(!entry)return;
  const auto& m=s.memory;
  if(!m.word(m.user,entry+0xa0))return;
  if(s.byte(m.user,entry)!=1)return;
  // Entry+0xa0 is tested as holder ADDRESS, never dereferenced here.
  // Low16 decrement wraps; only a fresh exact-zero count changes state1 to2.
  // Underflow/negative counts remain source values, not saturating refcounts.
  const uint16_t count=m.half(m.user,entry+2);
  m.storeHalf(m.user,entry+2,uint16_t(uint32_t(count)-1));
  // Source reload, exact zero: negative counts neither clamp nor retire.
  if(m.half(m.user,entry+2)==0)m.storeByte(m.user,entry,2);
}
} // namespace ttyd
