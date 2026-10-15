#pragma once
#include "battle_class_surface_gpu.hpp"
#include "source_matrix_font.hpp"
#include "native_camera_draw.hpp"
#include <functional>
namespace ttyd {
class BattleFontGpuResource;
class BattleFontGeneration {
public:
 ~BattleFontGeneration()=default;
 BattleFontGeneration(const BattleFontGeneration&)=delete;
 BattleFontGeneration& operator=(const BattleFontGeneration&)=delete;
private:
 BattleFontGeneration()=default;bool resident_=false;
 const void* texture_=nullptr;std::shared_ptr<const MessageFont> font_;
 friend class BattleFontGpuResource;friend struct BattleFontLease;
};
struct BattleFontLease {
 std::shared_ptr<const BattleFontGeneration> generation;
 std::shared_ptr<const MessageFont> font;
 const void* texture=nullptr;
 bool current()const;
};
// Borrows an already resident actual import. Does not import, free, substitute
// a GX object, or initialize original BSS. The real native owner MUST retire
// this resource before freeing/replacing its texture, including failed init.
class BattleFontGpuResource {
public:
 BattleFontGpuResource()=default;~BattleFontGpuResource();
 BattleFontGpuResource(const BattleFontGpuResource&)=delete;
 BattleFontGpuResource& operator=(const BattleFontGpuResource&)=delete;
 bool adopt(const MessageFont&,const BattleClassSurfaceImport&);
 void retire();BattleFontLease lease()const;
 bool accepts(const BattleFontLease&)const;
private:
 std::shared_ptr<BattleFontGeneration> generation_;BattleFontLease lease_;
};
struct BattleFontRasterView {
 // Actual native tilt of source projection ONLY; glyph matrix already has
 // the source camera view. No second camera/selector projection or unit scale.
 CameraProjectionMatrix projection{};
 NativeCameraRect viewport{},scissor{};
 float depthScale=-1,depthOffset=0;
};
struct BattleFontGlyphPacket {
 BattleFontLease font;WindowFontQuad quad;BattleActMatrix matrix{};
 BattleFontRasterView raster;
};
bool validBattleFontGlyphPacket(const BattleFontGlyphPacket&);
CameraProjectionMatrix battleFontGlyphTransform(const BattleFontGlyphPacket&);
struct BattleFontGlyphBinding {
 BattleFontLease font;BattleActorServices actors;
 std::shared_ptr<const void> resources;
 std::function<bool()> sourceCurrent,publicationCurrent;
 // Actual source GX owner, including static font object8041e97c, texgen/TEV,
 // fog and ordinary C0 RGB / A0*TEXA profile. Unknown/noise/rainbow must hold.
 std::function<bool()> ordinaryProfile;
 std::function<uint32_t(uint32_t)> random;
 BattleFontRasterView raster;size_t capacity=0;
};
// Actual one-glyph atomic sink, not a queue acknowledgement. Matrix/TEV loads
// are actual source children; emission copies their current state per glyph.
// Outer ordered publication/GPU submission must finish before queue advances.
class BattleFontGlyphSink {
public:
 BattleFontGlyphSink()=default;
 BattleFontGlyphSink(const BattleFontGlyphSink&)=delete;
 BattleFontGlyphSink& operator=(const BattleFontGlyphSink&)=delete;
 bool bind(BattleFontGlyphBinding);
 bool loadMatrix(const BattleActMatrix&,uint32_t);
 bool tevColor(uint32_t,uint32_t);
 bool glyph(const SourceMatrixFontGlyph&);
 bool publish();std::shared_ptr<const std::vector<BattleFontGlyphPacket>> snapshot()const;
 void retire();bool active()const;
private:
 BattleFontGlyphBinding binding_;
 std::vector<BattleFontGlyphPacket> building_;
 std::shared_ptr<const std::vector<BattleFontGlyphPacket>> published_;
 std::optional<BattleFontGlyphPacket> pending_;
 std::optional<BattleActMatrix> matrix_;std::optional<uint32_t> color_;
 bool bound_=false,publishing_=false,invoking_=false,running_=false,retireRequested_=false;mutable bool invalid_=false;
 bool current()const;bool source();bool admit(const WindowFontQuad&);
 void emit(const WindowFontQuad&);
 SourceMatrixFontAtomicGlyphServices glyphServices();void clear();
};
}
