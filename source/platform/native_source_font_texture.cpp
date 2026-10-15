#include "native_source_font_texture.hpp"
#include "event_native_runtime.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {namespace {
constexpr uint32_t Object=0x8041e97c;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
bool same(const SourceFontSetupCall&a,const SourceFontSetupCall&b){if(a.function!=b.function||a.callsite!=b.callsite||a.integers!=b.integers||a.reals.size()!=b.reals.size()||a.formatDestination!=b.formatDestination||a.path!=b.path)return false;for(size_t i=0;i<a.reals.size();++i)if(bits(a.reals[i])!=bits(b.reals[i]))return false;return true;}
}
struct NativeSourceFontTexture::State {
 EventNativeRuntime*vm=nullptr;std::shared_ptr<NativeSourceFontMemory>memory;
 std::shared_ptr<const void>lifetime;std::unique_ptr<NativeSourceFontBinding>binding;
 std::function<bool()>current;bool invalid=false;
 std::optional<SourceFontSetupCall>call;SourceGxTextureObjectInput input;SourceGxTextureObjectCursor cursor;
 SourceGxTextureObjectServices sdk;
 NativeSourceFontTextureDescription description;bool initialized=false,complete=false;
 bool valid(){if(invalid)return false;if(!binding||!current||!current()){invalid=true;return false;}return true;}
 std::optional<uint32_t> word(uint32_t at){if(!valid())return {};if(memory->owns(at,4))return memory->word(at);uint32_t v;if(!vm->memory.readWord(at,v))return {};return v;}
 std::optional<uint8_t> byte(uint32_t at){if(!valid())return {};if(memory->owns(at))return memory->byte(at);uint8_t v;if(!vm->memory.readByte(at,v))return {};return v;}
 bool sourcePointers(){if(!valid())return false;auto f=memory->word(0x8041e978),g=memory->word(0x8041e99c);if(!f||!g)return false;if(description.font&&(*f!=description.font||*g!=description.glyphs)){invalid=true;return false;}return true;}
 bool prepare(const SourceFontSetupCall&q){
  if(!sourcePointers()||q.path||q.formatDestination)return false;
  const auto f=memory->word(0x8041e978),g=memory->word(0x8041e99c);
  if(!f||!g||!*f||!*g||!memory->owns(*f,32)||!memory->owns(*g,32))return false;
  const auto magic=memory->word(*f),size=memory->word(*f+8),tag=memory->word(*g);const auto width=memory->half(*g+26),height=memory->half(*g+28);
  if(!magic||!size||!tag||!width||!height||*magic!=0x464f4e54||*tag!=0x474c5931||*size!=66560||*width!=512||*height!=256||uint64_t(*f)+*size>0x100000000ULL||uint64_t(*g)<uint64_t(*f)||uint64_t(*g)+32+65536>uint64_t(*f)+*size)return false;
  if(!vm->memory.contiguousRam(*f,*size)||!memory->byte(*f+*size-1)||!memory->owns(*g+32,65536)||!memory->byte(*g+32)||!memory->byte(*g+32+65535))return false;
  if(q.function==0x802b72b0){
   if(initialized||q.callsite!=0x800751b8||!q.reals.empty()||q.integers!=std::vector<uint32_t>{Object,*g+32,*width,*height,0,0,0,0})return false;
   description.object=Object;description.font=*f;description.glyphs=*g;description.image=*g+32;description.width=*width;description.height=*height;
  }else if(q.function==0x802b7544){
   const auto zero=word(0x804203f8);
   if(!initialized||q.callsite!=0x800751e0||q.integers!=std::vector<uint32_t>{Object,1,1,0,0,0}||q.reals.size()!=3||!zero)return false;
   for(float v:q.reals)if(bits(v)!=*zero)return false;
  }else return false;
  input={};for(size_t i=0;i<q.integers.size();++i)input.integers[i]=q.integers[i];for(size_t i=0;i<q.reals.size();++i)input.reals[i]=bits(q.reals[i]);call=q;return true;
 }
 bool store(uint32_t at,uint32_t n,uint32_t value){
  if(!valid()||at<Object||uint64_t(at)+n>uint64_t(Object)+32||!vm->memory.contiguousRam(at,n))return false;
  // Actual VM widths alias the binding's shared process bytes. Do not replace
  // the original STH with an independently maintained shadow descriptor.
  const bool ready=n==1?vm->memory.writeByte(at,uint8_t(value)):n==2?vm->memory.writeHalf(at,uint16_t(value)):n==4?vm->memory.writeWord(at,value):false;
  if(!ready||!valid())return false;
  const auto observed=n==1?std::optional<uint32_t>(memory->byte(at)):n==2?std::optional<uint32_t>(memory->half(at)):memory->word(at);
  const auto mask=n==4?0xffffffffu:(1u<<(8*n))-1;
  if(!observed||*observed!=(value&mask)){invalid=true;return false;}return true;
 }
 std::optional<uint32_t> clear(uint32_t at){
  if(!valid()||at!=Object||!vm->memory.contiguousRam(at,32))return {};
  for(unsigned i=0;i<32;++i){uint8_t v;auto expected=memory->byte(at+i);if(!expected||!vm->memory.readByte(at+i,v)||v!=*expected)return {};}
  // No external service or yield inside the actual admitted raw-RAM fill.
  // Existing source helper preserves eight STW stores for this aligned span.
  const bool ready=vm->memory.withRawRamAccess([&](auto&ram){if(!ram.bind(at,32))return false;auto m=vm->memoryBinding.services();eventFillMemory(at,0,32,m);return true;});
  if(!ready||!valid())return {};
  for(unsigned i=0;i<32;++i)if(memory->byte(at+i)!=std::optional<uint8_t>(0)){invalid=true;return {};}
  return at;
 }
 void services(){sdk.user=this;sdk.lifetime=lifetime;sdk.current=[](void*p){auto&s=*static_cast<State*>(p);return s.valid()&&s.sourcePointers();};
  sdk.word=[](void*p,uint32_t at){return static_cast<State*>(p)->word(at);};sdk.byte=[](void*p,uint32_t at){return static_cast<State*>(p)->byte(at);};
  sdk.store=[](void*p,uint32_t at,uint32_t n,uint32_t value){return static_cast<State*>(p)->store(at,n,value);};sdk.clear=[](void*p,uint32_t at){return static_cast<State*>(p)->clear(at);};
  sdk.truncate=[](void*p,float value)->std::optional<int32_t>{auto&s=*static_cast<State*>(p);if(!s.valid()||!std::isfinite(value)||double(value)<-2147483648.0||double(value)>=2147483648.0||!s.vm->platform.truncate)return {};const auto v=s.vm->platform.truncate(s.vm->platform.user,value);return s.valid()?std::optional<int32_t>(v):std::nullopt;};
 }
};
NativeSourceFontTexture::NativeSourceFontTexture()=default;NativeSourceFontTexture::~NativeSourceFontTexture(){retire();}
std::unique_ptr<NativeSourceFontTexture> NativeSourceFontTexture::bind(EventNativeRuntime&vm,std::shared_ptr<NativeSourceFontMemory>memory,std::shared_ptr<const void>lifetime,std::function<bool()>current){
 if(!memory||!lifetime||!current||!current())return {};
 auto binding=memory->bind(vm.memory);if(!binding)return {};
 auto out=std::unique_ptr<NativeSourceFontTexture>(new NativeSourceFontTexture);out->state_=std::make_unique<State>();auto&s=*out->state_;s.vm=&vm;s.memory=std::move(memory);s.binding=std::move(binding);s.lifetime=std::move(lifetime);s.current=std::move(current);s.services();if(!s.valid())return {};return out;
}
std::optional<uint32_t> NativeSourceFontTexture::request(const SourceFontSetupCall&q){
 if(invoking_||retiring_||!state_||state_->complete)return {};
 invoking_=true;struct Guard{NativeSourceFontTexture&owner;~Guard(){owner.invoking_=false;if(owner.retiring_)owner.state_.reset();}}guard{*this};auto&s=*state_;
 if(!s.valid()||!s.sourcePointers())return {};
 if(s.call){if(!same(*s.call,q))return {};}else if(!s.prepare(q))return {};
 if(!resumeSourceGxTextureObject(q.function,s.input,s.sdk,s.cursor)||!s.valid())return {};
 s.call.reset();if(q.function==0x802b72b0){s.initialized=true;return 0;}
 for(unsigned i=0;i<32;++i){auto b=s.memory->byte(Object+i);if(!b){s.invalid=true;return {};}s.description.objectBytes[i]=*b;}
 s.complete=true;return 0;
}
std::optional<NativeSourceFontTextureDescription> NativeSourceFontTexture::object()const{
 if(invoking_||retiring_||!state_)return {};
 auto&s=*state_;if(!s.complete||!s.valid()||!s.sourcePointers())return {};
 for(unsigned i=0;i<32;++i)if(s.memory->byte(Object+i)!=std::optional<uint8_t>(s.description.objectBytes[i])){s.invalid=true;return {};}
 return s.description;
}
bool NativeSourceFontTexture::stale()const{return retiring_||(state_&&(state_->invalid||state_->cursor.stale()));}
uint32_t NativeSourceFontTexture::pending()const{return state_?state_->cursor.pending():0;}
void NativeSourceFontTexture::retire(){if(!state_)return;state_->invalid=true;retiring_=true;if(!invoking_)state_.reset();}
}
