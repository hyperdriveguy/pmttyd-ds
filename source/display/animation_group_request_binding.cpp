#include "animation_group_request_binding.hpp"
namespace ttyd {
AnimationGroupRequestBinding::AnimationGroupRequestBinding(EventAddressSpace& m,std::shared_ptr<NativeFileCache> f):memory_(m),files_(std::move(f)){}
std::unique_ptr<AnimationGroupRequestBinding> AnimationGroupRequestBinding::bind(EventAddressSpace& m,std::shared_ptr<NativeFileCache> f){
 uint32_t manager=0;if(!f || !f->aliases(m) || !m.readWord(NativeFileCache::Global,manager) || manager!=NativeFileCache::Manager)return {};
 // Verified original US SDA2 empty literal8041fb30 is outside the older cold
 // boot image spans. Supply its exact four zero bytes in native VM storage;
 // never overwrite another mapping or infer AG2TG contents from a dictionary.
 uint32_t literal=0;if(!m.readWord(0x8041fb30,literal)){if(!m.unmappedRange(0x8041fb30,4) || !m.registerRam(0x8041fb30,std::vector<uint8_t>(4)))return {};}else if(literal)return {};
 return std::unique_ptr<AnimationGroupRequestBinding>(new AnimationGroupRequestBinding(m,std::move(f)));
}
// 128 bytes includes required NUL;no wrapping pointer scan. This bounds native
// strings only,not the raw original ag2tg/request parent's source scan domain.
bool AnimationGroupRequestBinding::text(uint32_t a,std::string& out,unsigned limit) const{
 out.clear();for(unsigned i=0;i<limit;++i){uint8_t b=0;if(uint64_t(a)+i>UINT32_MAX || !memory_.readByte(a+i,b))return false;if(!b)return true;out+=char(b);}return false;
}
AnimationGroupRequestServices AnimationGroupRequestBinding::services(){
 AnimationGroupRequestServices s;s.user=this;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationGroupRequestBinding*>(p);uint32_t v=0;return b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.archive=[](void* p,uint32_t mode){return static_cast<AnimationGroupRequestBinding*>(p)->files_->archiveType(mode);};
 // Compare decoded byte strings for equality/sign. No optimized SDK word
 // overread or source callback/thread timing is emulated by these native copies.
 s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& n=*static_cast<AnimationGroupRequestBinding*>(p);std::string x,y;if(!n.text(a,x) || !n.text(b,y))return {};return x==y?0:x<y?-1:1;};
 s.request=[](void* p,uint32_t type,uint32_t completion,uint32_t format,uint32_t prefix,uint32_t name)->std::optional<uint32_t>{
  auto& b=*static_cast<AnimationGroupRequestBinding*>(p);std::string fmt,root,text;
  // Only actual MODEL5/TPL4 formats and prefix "a" are admitted.123 name
  // bytes plus "a/",optional '-' and NUL fit the 128-byte file-name envelope.
  if((type!=4 && type!=5) || !b.text(format,fmt) || fmt!=(type==5?"%s/%s":"%s/%s-") || !b.text(prefix,root) || root!="a" || !b.text(name,text) || text.size()>123)return {};
  return b.files_->request(root+"/"+text+(type==4?"-":""),type,completion);
 };return s;
}
// Shared file aliases must still match this VM. Engaged0 is a completed poll,
// not native unavailability;raw parent resets after every completed invocation.
std::optional<uint32_t> AnimationGroupRequestBinding::advance(uint32_t name,uint32_t mode,uint32_t completion,unsigned visits){
 if(!files_->aliases(memory_))return {};
 uint32_t result=0;if(!requestAnimationGroup(name,mode,completion,services(),cursor_,result,visits))return {};return result;
}
}
