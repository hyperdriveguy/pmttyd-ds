#include "opening_book_cover.hpp"
#include "opening_book_view.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main(){
    const auto left=openingBookPaperPoint(-32,24,0,6),right=openingBookPaperPoint(32,-24,0,6);
    assert(std::fabs(left[0]-212)<.0001f && left[1]==0);
    assert(std::fabs(right[0]-405)<.0001f && right[1]==240);
    assert(openingBookPan(1.5f,0)==0 && openingBookPan(1.5f,180)==102);
    assert(openingBookPan(1.5f,90)==51 && openingBookPan(3,180)==204);
    assert(openingBookPan(3,180,1)==0 && openingBookPan(3,180,.5f)==102);
    // The opened cover hinge is centered, giving each inside cover the same
    // visible width. Its source dimensions and vertical projection are intact.
    assert(98+openingBookPan(1.5f,180)==200);
    // Global image scaling keeps the captured crop registered to screen center,
    // and image offsets retain the same half-size pixels as the cover's offset.
    const auto adjusted=openingBookPaperPoint(-32,24,0,6,2,3,-10.5f);
    assert(std::fabs(adjusted[0]-238.25f)<.001f && adjusted[1]==-67.5f);
    const auto faded=openingBookPaperPoint(-32,24,0,6,2,3,-10.5f,0);
    assert(std::fabs(faded[0]-34.25f)<.001f && faded[1]==adjusted[1]);
    // The captured front stays193px wide. The source final deformation's
    //39.9395-unit back spans199.6975px at the same5px/unit as its height.
    const auto turned=openingBookPaperPoint(-71.9395f,24,0,6);
    assert(std::fabs(turned[0]-12.3025f)<.001f);
    const auto hingeLeft=openingBookPaperPoint(-32.001f,24,0,6);
    const auto hingeRight=openingBookPaperPoint(-31.999f,24,0,6);
    assert(std::fabs(hingeLeft[0]-212)<.006f && std::fabs(hingeRight[0]-212)<.006f);
    assert(OpeningBookView::desktop[0]==0 && OpeningBookView::desktop[2]==400);
    assert(OpeningBookView::desktop[1]<0 && OpeningBookView::desktop[3]>240);
    auto zero=openingBookCover(288,352,1.5f,0);assert(zero);
    assert((*zero)[0].palette==2 && (*zero)[1].palette==1 && (*zero)[2].palette==2);
    assert((*zero)[0].blend && !(*zero)[1].blend && (*zero)[0].cull==2 && (*zero)[2].cull==1);
    assert((*zero)[1].vertices[0].x==-204 && (*zero)[1].vertices[1].x==228 && (*zero)[1].vertices[0].y==264);
    assert((*zero)[1].vertices[0].u==0 && (*zero)[2].vertices[0].u==1);
    assert(openingBookFaceVisible((*zero)[1]) && !openingBookFaceVisible((*zero)[2]));
    auto side=openingBookCover(288,352,1.5f,90);assert(side);
    const auto& hinge=(*side)[1].vertices[0];assert(hinge.x==-204 && hinge.z==0);
    assert(std::fabs((*side)[1].vertices[1].x+204)<.0001f && std::fabs((*side)[1].vertices[1].z-432)<.0001f);
    assert(!openingBookFaceVisible((*side)[1]) && openingBookFaceVisible((*side)[2]));
    auto end=openingBookCover(288,352,1.5f,180);assert(end && std::fabs((*end)[1].vertices[1].x+636)<.001f);
    assert(!openingBookFaceVisible((*end)[1]) && openingBookFaceVisible((*end)[2]));
    auto offset=openingBookCover(288,352,1,0,10,-20,.5f);assert(offset && (*offset)[0].alpha==127 && (*offset)[1].alpha==255);
    assert((*offset)[1].vertices[0].x==-126 && (*offset)[1].vertices[0].y==156);
    auto page=openingBookPage(288,352,1.5,0);assert(page && page->palette==3);
    assert(std::fabs(page->vertices[1].x-206.4f)<.001f && std::fabs(page->vertices[0].y-250.8f)<.001f);
    auto illustration=openingBookIllustration(1.5,4,10,-20,.5f);assert(illustration && illustration->palette==12 && illustration->blend && illustration->alpha==127 && illustration->cull==2);
    assert(std::fabs(illustration->vertices[0].x+142)<.001f && illustration->vertices[0].y==152.5f);
    assert(std::fabs(illustration->vertices[2].x-162)<.001f && illustration->vertices[2].y==-87.5f);
    assert(illustration->vertices[0].u==0 && illustration->vertices[2].v==1);
    assert(!openingBookIllustration(1,5) && !openingBookIllustration(NAN,0));
    const auto screen=openingBookScreen((*zero)[1].vertices[0]);assert(screen[0]==98 && screen[1]==-12);
    const auto far=openingBookScreen((*side)[1].vertices[1]);assert(far[0]<98); // Actual perspective; no flat-quad cosine squeeze.
    assert(openingBookCameraDistance()>1082 && openingBookCameraDistance()<1083);
    assert(!openingBookCover(0,352,1,0) && !openingBookCover(288,352,0,0) && !openingBookCover(288,352,1,NAN));
    assert(!openingBookPage(288,352,1,5));
}
