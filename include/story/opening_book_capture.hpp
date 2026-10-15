#pragma once
#include "opening_book_view.hpp"
#include <array>
namespace ttyd {
// Clip-space crop of the ordinary, untilted 400x240 book projection into its
// native256-square render texture. Content occupies193x240 at its top left.
struct OpeningBookCapture {
 static constexpr unsigned size=256;
 static constexpr float scaleX=400.f/size,scaleY=240.f/size;
 static constexpr float offsetX=(400.f-2*OpeningBookView::captureLeft-size)/size;
 static constexpr float offsetY=(float(size)-240.f)/size;
};
// Homogeneous clip transform (offset*w before perspective divide), not a
// postprojected screen translation. Only computes crop coordinates; actual GPU
// completion, target lifetime and capture admission remain renderer-owned.
inline std::array<float,2> openingBookCaptureClip(float x,float y,float w){
 return {{OpeningBookCapture::scaleX*x+OpeningBookCapture::offsetX*w,
          OpeningBookCapture::scaleY*y+OpeningBookCapture::offsetY*w}};
}
}
