#include "native_field_animation.hpp"
#include "field_pose_pool.hpp"
#include "animation_group_request_binding.hpp"
#include <cassert>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
uint8_t byte(EventAddressSpace& m,uint32_t a){uint8_t v=0;assert(m.readByte(a,v));return v;}
uint32_t payload(EventAddressSpace& m,uint32_t h){return word(m,word(m,h+0xa0));}
void native(const char* root){
 auto files=NativeFileCache::create();EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};auto animation=NativeFieldAnimation::create(root,files,p);assert(animation);auto& m=animation->process().vm().memory;
 EventNativeRuntime other(0x81000000,{});assert(files->bind(other.memory));
 const auto ag=word(m,0x803d9570),holder=word(m,ag+0xa0);assert(word(m,holder+4)==41984 && half(m,ag+2)==1);
 const auto model=files->request("a/c_kanbu_ibt",5),texture=files->request("a/c_kanbu-",4);assert(model && !*model && texture && !*texture && files->asyncPending());
 const auto first=word(m,ag+0xa4),second=word(m,first+0xa4);assert(first && second && !word(m,second+0xa4));
 const auto md=word(m,first+0xac),td=word(m,second+0xac);assert(md && td && md!=td && byte(m,first)==3 && byte(m,second)==3 && !half(m,first+2) && !half(m,second+2));assert(word(m,md+0x7c)==payload(m,first) && word(m,td+0x7c)==payload(m,second) && word(m,md+0x8c)==0x8018a1a0);
 assert(files->request("a/c_kanbu_ibt",4)==0 && !half(m,first+2)); // Pending hit ignores requested type, returns source0.
 // Ordinary acquisition waits without busy-looping; worker can run while
 // that same _fileAlloc cursor retains its pending Entry and source filename.
 assert(!files->acquire("a/c_kanbu_ibt",5));assert(!files->advanceAsync(1));assert(word(m,first+0xac)==md && word(m,second+0xac)==td && !files->request("a/c_kanbu-",4));
 for(unsigned n=0;n<4096 && files->asyncPending();++n)files->advanceAsync(4096);
 assert(!files->asyncPending() && !word(m,first+0xac) && !word(m,second+0xac));assert(half(m,md+0x90)&8);assert(files->acquire("a/c_kanbu_ibt",5)==first && half(other.memory,first+2)==1 && byte(m,first)==1);
 assert(files->request("a/c_kanbu_ibt",0)==first && half(m,first+2)==1);assert(files->request("a/c_kanbu-",4)==second && !half(m,second+2));assert(files->release(first) && byte(m,first)==2);assert(files->request("a/c_kanbu_ibt",999)==first && byte(m,first)==3 && !half(m,first+2));assert(files->acquire("a/c_kanbu_ibt",5)==first && half(m,first+2)==1);
 FieldPosePool pool(animation->pool());const auto base=std::string(root)+"/field-native-c_kanbu_ibt";auto resource=loadBattlePoseResource((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str());assert(resource && pool.install(resource));FieldPoseEntryCursor c;const auto h=pool.resumeEnter("c_kanbu_ibt",0,{},c);assert(h && *h>=0 && animation->pool()->pose(*h)->resource==resource && half(m,first+2)==2 && half(m,second+2)==1); // Same preloaded holders/bytes become real pose owners.
 assert(!files->request("a/missing",5) && !files->request("a/c_pkr",5,0x1234));
}

void groups(const char* root){
 auto files=NativeFileCache::create();EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};auto animation=NativeFieldAnimation::create(root,files,p);assert(animation);auto& m=animation->process().vm().memory;
 auto binding=AnimationGroupRequestBinding::bind(m,files);assert(binding);
 auto foreign=NativeFileCache::create();assert(!AnimationGroupRequestBinding::bind(m,foreign));
 const char* models[]={"c_pkr","c_kanbu_ibt","c_zako_n","c_gara","c_suifu_b"};
 constexpr uint32_t Name=0x81800000;
 for(auto name:models){
  for(unsigned i=0;i<=std::string(name).size();++i)assert(m.writeByte(Name+i,uint8_t(name[i])));
  // A bounded native visit retains the archive/model/table/texture prefix.
  auto result=binding->advance(Name,0,0,1);assert(!result && binding->pending());assert(!binding->advance(Name,1));
  for(unsigned n=0;n<8192 && !result;++n)result=binding->advance(Name,0,0,256);
  assert(result && !*result && !binding->pending()); // Source0, with BOTH requests submitted.
 }
 assert(files->asyncPending());for(unsigned n=0;n<8192 && files->asyncPending();++n)files->advanceAsync(4096);assert(!files->asyncPending());
 FieldPosePool poses(animation->pool());
 for(auto name:models){
  for(unsigned i=0;i<=std::string(name).size();++i)assert(m.writeByte(Name+i,uint8_t(name[i])));
  assert(binding->advance(Name,1)==1 && word(m,NativeFileCache::Manager+4)==2);
  const auto base=std::string(root)+"/field-native-"+name;auto resource=loadBattlePoseResource((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str());assert(resource && poses.install(resource));
  const auto model=files->request(std::string("a/")+name,5),texture=files->request("a/"+resource->textureName+"-",4);assert(model && *model && texture && *texture && !half(m,*model+2) && !half(m,*texture+2));
  FieldPoseEntryCursor c;auto handle=poses.resumeEnter(name,0,{},c);assert(handle && *handle>=0 && half(m,*model+2)==1 && half(m,*texture+2)==1);
 }
}
}
int main(){native("romfs");groups("romfs");}
