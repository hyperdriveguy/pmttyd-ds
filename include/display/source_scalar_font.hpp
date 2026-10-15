#pragma once
#include <memory>
#include "source_matrix_font.hpp"
namespace ttyd {
struct SourceScalarFontServices {
 // Reuse actual shared JUTFont lead/map/width/command and glyph owners, not
 // the different FontDrawStringMtx layout or its LF/tag policy.
 SourceMatrixFontServices common;
 // Binary32 fused multiply-add; source operands captured at each reached
 // fmadds instruction before a held finite native math provider.
 std::optional<float> (*fma)(void*,float,float,float)=nullptr;
 // common.nullByte/nullHalf own actual faulting guest reads after a source
 // command-parser null return; never borrow private stream offset zero.
};
class SourceScalarFontCursor {
public:
 SourceScalarFontCursor();~SourceScalarFontCursor();
 SourceScalarFontCursor(const SourceScalarFontCursor&)=delete;
 SourceScalarFontCursor&operator=(const SourceScalarFontCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceScalarFont(float,float,const SourceMatrixFontText&,const SourceScalarFontServices&,SourceScalarFontCursor&);
};
// Whole FontDrawString800773d0/68 -> FontDrawStringVecPitch80077034/334.
// Uses actual copied source XYZ/pitch, live source font globals and camera;
// input descriptor identity/provider tables remain fixed across waits.
// Glyph submission uses the genuine shared JUTFont_DrawPos80074720 owner.
// Captured requests alone do not acknowledge the outer queued display.
bool resumeSourceScalarFont(float x,float y,const SourceMatrixFontText&,
 const SourceScalarFontServices&,SourceScalarFontCursor&);
}
