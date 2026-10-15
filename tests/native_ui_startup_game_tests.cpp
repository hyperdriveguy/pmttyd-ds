#include <memory>
#include "game.hpp"
#include "native_ui_startup.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
int main(){
 assert(loadFieldPlayerPoseAssets("romfs"));
 EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
 p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};
 p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};
 p.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};
 auto game=std::make_unique<Game>(false);assert(game->initializeNativeUiFieldAnimation("romfs",NativeFileCache::create(),p));
 const auto ui=game->nativeUiStartup();assert(ui&&ui->fontObject()&&game->nativeMarioPose());
 assert(ui->windowReady()&&ui->windowMemory()&&ui->windowObject(50)&&ui->windowCounts().callbacks==1&&ui->windowCounts().objectClears==51);
 const auto process=ui->fieldProcess();assert(process==game->fieldPartyState().nativeAnimation);
 const auto counts=ui->counts();assert(counts.gpStores==1&&counts.reads==1&&counts.heapAllocations==1&&counts.heapTransfers==1&&counts.closes==1);
 auto&vm=process->process().vm();uint32_t gp;assert(vm.memory.readWord(0x80415fd0,gp)&&vm.memory.writeWord(gp+0x16c,2));
 game->tick({});assert(ui->language()==2);
 auto candidate=std::make_unique<Game>(*game);assert(!candidate->nativeUiStartup());
 assert(candidate->retainNativeFieldAnimation(*game)&&candidate->nativeUiStartup()==ui);
 auto saved=game->progress;game->restore(saved);assert(game->nativeUiStartup()==ui&&ui->language()==2);
 auto checkpoint=std::make_unique<Game>(false);checkpoint->restore(saved);
 assert(checkpoint->retainNativeFieldAnimation(*game,true)&&checkpoint->nativeUiStartup()==ui&&ui->windowReady()&&!game->nativeUiStartup());
 assert(checkpoint->fieldPartyState().nativeAnimation==process&&ui->language()==2);
 checkpoint->tick({});assert(checkpoint->nativeMarioPose()&&ui->language()==2&&ui->counts().reads==counts.reads&&ui->counts().heapAllocations==counts.heapAllocations);
 std::cout<<"actual Game UI startup/room/checkpoint process retention PASS\n";
}
