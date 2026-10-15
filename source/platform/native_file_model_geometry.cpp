#include "native_file_cache.hpp"
#include "animation_draw_resource.hpp"
#include <cstring>
namespace ttyd {
// MODEL type5 must be the exact staged payload and immutable draw resource.
// This closes static assembly inputs; it is not general MODEL interpretation,
// timeline/output equivalence, file readiness or a completed source draw pass.
bool NativeFileCache::modelGeometryCurrent(EventAddressSpace& memory,uint32_t model,std::shared_ptr<const AnimationDrawResource> draw) const{
 if(!draw)return false;
 const File* file=nullptr;for(const auto& f:files_)if(f->address==model && f->type==5){file=f.get();break;}if(!file)return false;
 if(file->geometryIdentity.lock()!=draw){if(file->original!=draw->data_)return false;file->geometryIdentity=draw;}
 return memory.withRawRamAccess([&](EventAddressSpace::RawRamAccess& ram){
  if(!ram.aliases(model,file->payload.data(),uint32_t(file->payload.size())) || file->payload.size()<draw->data_.size())return false;
  const auto* live=file->payload.data();const auto& original=draw->data_;
  // Header before14c is unchanged. Root fields14c..1ac are BE relocated
  // pointers: each must equal model base + the original file-relative word.
  if(std::memcmp(live,original.data(),0x14c))return false;
  for(uint32_t o=0x14c;o<=0x1ac;o+=4){uint32_t word;if(!ram.readWord(model+o,word) || word!=model+draw->word(o))return false;}
  // {count offset, pointer offset, decimal stride}: shapes168, runs8,
  // position/normal indices4, colors/indices4, UVs8, surfaces24, tex slots8,
  // descriptors64, draws108, groups88. Base positions/normals, visibility,
  // nodes and timeline data are dynamic/separate consumers, hence omitted.
  constexpr uint32_t tables[][3]={{0xe8,0x14c,168},{0xec,0x150,8},{0xf4,0x158,4},{0xfc,0x160,4},{0x100,0x164,4},{0x104,0x168,4},{0x128,0x18c,8},{0x12c,0x190,24},{0x130,0x194,8},{0x134,0x198,64},{0x138,0x19c,108},{0x144,0x1a8,88}};
  auto same=[&](uint32_t offset,uint64_t n){return uint64_t(offset)+n<=original.size() && !std::memcmp(live+offset,original.data()+offset,size_t(n));};
  for(const auto& t:tables)if(!same(draw->word(t[1]),uint64_t(draw->word(t[0]))*t[2]))return false;
  // Eight UV-index streams use count108+4*i and pointer16c+4*i (hex),
  // each serialized as four-byte words. Directory rows are64 bytes; compare
  // only their60-byte names, excluding the relocated timeline pointer at+60.
  // Original empty streams can have sentinel offsets. Root relocation
  // equality above still covers them; there are no stream bytes to compare.
  for(unsigned i=0;i<8;++i){const auto count=draw->word(0x108+4*i);if(count && !same(draw->word(0x16c+4*i),uint64_t(count)*4))return false;}
  const uint32_t directory=draw->word(0x1ac);for(uint32_t i=0;i<draw->word(0x148);++i)if(!same(directory+64*i,60))return false;
  return true;
 });
}
}
