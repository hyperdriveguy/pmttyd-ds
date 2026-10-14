#include "opening_book_capture.hpp"
#include "opening_book_cover.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main(){
 // Compare the normal-screen projection with the capture projection at the
 // crop corners and across the book's actual perspective rotation.
 for(float x:{110.f,206.5f,303.f})for(float y:{0.f,120.f,240.f})for(float w:{1.f,2.f}){
  const auto clip=openingBookCaptureClip((2*x/400-1)*w,(1-2*y/240)*w,w);
  assert(std::fabs((clip[0]/w+1)*128-(x-110))<.0001f);
  assert(std::fabs((1-clip[1]/w)*128-y)<.0001f);
 }
 for(float angle:{0.f,45.f,90.f,180.f}){
  const auto quads=openingBookCover(288,352,1.5f,angle);assert(quads);
  for(const auto& quad:*quads)for(const auto& vertex:quad.vertices){
   const auto screen=openingBookScreen(vertex);
   const auto clip=openingBookCaptureClip(2*screen[0]/400-1,1-2*screen[1]/240,1);
   assert(std::fabs((clip[0]+1)*128+110-screen[0])<.0001f);
   assert(std::fabs((1-clip[1])*128-screen[1])<.0001f);
  }
 }
 static_assert(OpeningBookCapture::size==256);
 static_assert(OpeningBookView::captureWidth==193 && OpeningBookView::captureHeight==240);
}
