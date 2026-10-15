#pragma once
#include "source_gx_texture_object.hpp"
namespace ttyd {
struct SourcePaletteTextureObjectServices {
 SourceGxTextureObjectServices gx;
 std::optional<uint16_t>(*half)(void*,uint32_t)=nullptr;
 // One original lfd load, not two independently admitted half-loads.
 std::optional<uint64_t>(*doubleWord)(void*,uint32_t)=nullptr;
 // Original fsubs: two raw binary64 operands -> raw binary32 result.
 std::optional<uint32_t>(*subtractSingle)(void*,uint64_t,uint64_t)=nullptr;
};
// Table captured at entry,header refreshed after Init,one captured lfd bias
// shared by two conversions. Reset cannot reverse SDK object/source mutations;
// keep actual resource/output lifetime through all nested waits.
class SourcePaletteTextureObjectCursor {
public:
 SourcePaletteTextureObjectCursor();~SourcePaletteTextureObjectCursor();
 SourcePaletteTextureObjectCursor(const SourcePaletteTextureObjectCursor&)=delete;
 SourcePaletteTextureObjectCursor&operator=(const SourcePaletteTextureObjectCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
 uint32_t childPending()const;
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourcePaletteTextureObject(uint32_t,uint32_t,uint32_t,const SourcePaletteTextureObjectServices&,SourcePaletteTextureObjectCursor&);
};
// Whole TEXGetGXTexObjFromPalette80037a80/dc. Source r3=palette,r4=object,
// r5=index. object is the actual supplied output address, never an index alias.
bool resumeSourcePaletteTextureObject(uint32_t palette,uint32_t object,uint32_t index,const SourcePaletteTextureObjectServices&,SourcePaletteTextureObjectCursor&);

// Optional private native output domain. Binding supplies an already reserved
// actual address/lifetime; this utility neither allocates guest storage nor
// invents a source SP alias, object readiness, memset, or source generation.
class SourcePrivateTextureObject {
public:
 bool bind(uint32_t address,std::shared_ptr<const void> lifetime,void*user,bool(*current)(void*));
 SourcePrivateTextureObject()=default;
 SourcePrivateTextureObject(const SourcePrivateTextureObject&)=delete;
 SourcePrivateTextureObject&operator=(const SourcePrivateTextureObject&)=delete;
 bool owns(uint32_t address,uint32_t size)const;
 // First-refusal test: a partial overlap is owned but unavailable, never a
 // reason to fall through to another memory provider's bytes.
 bool overlaps(uint32_t address,uint32_t size)const;
 std::optional<uint32_t> read(uint32_t address,uint32_t size)const;
 bool store(uint32_t address,uint32_t size,uint32_t value);
 std::optional<std::array<uint32_t,8>> words()const;
 void retire();
private:
 uint32_t address_=0;std::shared_ptr<const void> lifetime_;void*user_=nullptr;bool(*current_)(void*)=nullptr;
 std::array<std::optional<uint8_t>,32>bytes_{};mutable bool invalid_=false,invoking_=false;
 bool live()const;
};
}
