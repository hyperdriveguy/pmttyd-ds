#pragma once
#include <memory>
#include <array>
#include <optional>
#include <cstdint>
namespace ttyd {
struct SourceGxTextureObjectInput {
 // Init: object,image,width,height,format,wrapS,wrapT,mipmap.
 // LOD: object,minFilter,magFilter,biasClamp,edgeLOD,anisotropy,0,0.
 std::array<uint32_t,8> integers{};
 // LOD f1=minLOD,f2=maxLOD,f3=bias, stored as binary32 source bits.
 std::array<uint32_t,3> reals{};
};
struct SourceGxTextureObjectServices {
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 bool (*current)(void*)=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 // size is exactly 1,2,4. A held write owns its retained invocation and must
 // not replay already committed effects when called again with identical args.
 bool (*store)(void*,uint32_t address,uint32_t size,uint32_t value)=nullptr;
 // Genuine whole memset800050b4(object,0,32), including actual memory effects.
 // Engaged zero is still a completed source return; return value is ignored.
 std::optional<uint32_t> (*clear)(void*,uint32_t object)=nullptr;
 // Actual fctiwz numeric provider; unavailable is distinct from integer zero.
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
};
class SourceGxTextureObjectCursor {
public:
 SourceGxTextureObjectCursor();~SourceGxTextureObjectCursor();
 SourceGxTextureObjectCursor(const SourceGxTextureObjectCursor&)=delete;
 SourceGxTextureObjectCursor&operator=(const SourceGxTextureObjectCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceGxTextureObject(uint32_t,const SourceGxTextureObjectInput&,const SourceGxTextureObjectServices&,SourceGxTextureObjectCursor&);
};
// Complete source bodies; no native texture import or GPU submission is implied.
bool resumeSourceGxTextureObject(uint32_t function,const SourceGxTextureObjectInput&,const SourceGxTextureObjectServices&,SourceGxTextureObjectCursor&);
}
