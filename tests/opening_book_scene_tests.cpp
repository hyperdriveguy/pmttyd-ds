#include "opening_book_scene.hpp"
#include "actor_animation.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
using namespace ttyd;
int main(int argc,char**argv){
    OpeningBookScene book;book.begin(100);
    assert(book.scale==.6f && !book.scaleComplete() && !book.thirdMessageReady());
    book.advance(160);assert(std::fabs(book.scale-.675f)<1e-6f);
    book.advance(460);assert(std::fabs(book.scale-1.05f)<1e-6f);
    book.advance(819);assert(book.scale<1.5f);book.advance(820);assert(book.scaleComplete());
    assert(!book.thirdMessageReady());book.advance(880);assert(book.thirdMessageReady());
    book.open();assert(book.image==8 && book.rotation==0 && book.opening);
    book.advance(910);assert(std::fabs(book.rotation-26.3604f)<.001f);
    book.advance(940);assert(std::fabs(book.rotation-90)<.001f);
    book.advance(1000);assert(book.rotation==180 && !book.opening);
    assert(book.beginPage(0));assert(book.image==8 && book.pages[0].image==8 && book.pages[0].depth==6);
    book.advance(1200);book.presented(0,180);assert(!book.pageComplete(0) && book.pageFrame(0)==0);
    book.captureComplete(0);assert(book.image==9 && book.pageFrame(0)==0);
    book.advance(1379);book.presented(0,180);assert(!book.pageComplete(0));
    book.advance(1380);assert(!book.pageComplete(0));book.presented(0,179);assert(!book.pageComplete(0));
    book.presented(0,180);assert(book.pageComplete(0));assert(!book.beginPage(0));
    assert(book.beginPage(1) && book.pages[0].active && book.pages[1].image==9 && book.pages[1].depth==5);
    book.captureComplete(1);book.advance(1560);book.presented(1,180);assert(book.pageComplete(1) && !book.pages[0].active);
    book.releasePages();for(auto& p:book.pages)assert(!p.active);assert(!book.beginPage(4));
    book.beginPage(3);book.captureComplete(3);book.startAdjust();
    assert(book.adjusting && !book.adjusted && book.pages[3].active && book.fade==0);
    book.advance(1650);assert(book.adjusting && book.fade==.5f);
    assert(std::fabs(book.scale-2.5606601f)<1e-6f && std::fabs(book.offsetY+74.24621f)<1e-5f);
    assert(book.finalPaperScale==book.scale/1.5f && book.finalPaperOffsetY==book.offsetY/10 && book.pages[3].active);
    book.advance(1739);assert(book.adjusting && book.pages[3].active && book.fade<1);
    book.advance(1740);assert(!book.adjusting && book.adjusted && book.fade==1 && book.scale==3 && book.offsetY==-105 && !book.pages[3].active);
    book.advance(1900);assert(book.scale==3 && book.fade==1); // Scale brother does not overwrite final adjustment.
    if(argc>1){
        ActorAnimationPack pack;assert(pack.load(argv[1]));const auto* pose=pack.pose("A_1");assert(pose && !pose->loop && pose->start==0 && pose->end==180);
        assert(pose->bindings.size()==372 && pose->keys.size()==181 && pose->textures.size()==2);
        ActorAnimationSample zero,half,end;assert(pose->evaluate(0,zero) && pose->evaluate(90,half) && pose->evaluate(180,end));
        float maxZ=0;for(auto&v:half.vertices)maxZ=std::max(maxZ,v.z);assert(std::fabs(maxZ-13.5f)<.001f);
        assert(half.vertices.size()==372 && end.vertices.size()==372);
        for(unsigned i=0;i<zero.vertices.size();++i){const auto& b=pose->bindings[i];const auto&v=zero.vertices[i];const auto cell=pose->textures[b.texture].cells[0];assert(cell<=1);
            assert(std::fabs((v.u*1024-cell*128-2)/124-b.u)<1e-6f);
            assert(std::fabs(((1-v.v)*1024-2)/124-b.v)<1e-6f);
        }
        if(argc>2){
            FILE* file=std::fopen(argv[2],"rb");assert(file);char magic[8];uint32_t count=0;
            assert(std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMDPG001",8) && std::fread(&count,4,1,file)==1 && count==124);
            unsigned captured=0,back=0;
            for(unsigned i=0;i<count;++i){uint32_t row[3];assert(std::fread(row,4,3,file)==3 && row[0]<=1 && row[1]==2 && row[2]==1);
                const auto& binding=pose->bindings[3*i];assert(row[0]==pose->textures[binding.texture].cells[0]);
                if(row[0])++captured;else ++back;
            }
            assert(captured==62 && back==62 && std::fgetc(file)==EOF);std::fclose(file);
        }
        std::puts("Original page A_1 retained372vertices/181keys; frame90 deformation and captureUV inverse passed");
    }
}
