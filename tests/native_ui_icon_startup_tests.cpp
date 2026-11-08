#include <memory>
#include "native_ui_startup.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double x){volatile float v=float(x);return v;};return p;}
void actual(){auto process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());auto ui=NativeUiStartup::create(process,"romfs");assert(ui&&!ui->iconProcess());assert(ui->resumeIcons());auto icons=ui->iconProcess();assert(icons&&icons->ready()&&ui->windowReady()&&ui->fontObject());assert(ui->sourceResourceWord(0x8041e638)==1&&ui->sourceResourceWord(0x8041e63c)==1);const auto palette=ui->sourceResourceWord(0x8041e630);assert(palette&&*palette&&ui->sourceResourceWord(*palette+4)==613);auto c=icons->counts();assert(c.allocations==4&&c.callbacks==2&&c.deliveries==2);assert(ui->resumeIcons()&&icons==ui->iconProcess()&&icons->counts().allocations==c.allocations&&icons->counts().reads==c.reads);ui->retire();assert(!icons->current()&&!icons->ready()&&!ui->iconProcess()&&ui->ownsSourceResource(0x8041e630)&&!ui->sourceResourceWord(0x8041e630));}
}
int main(){actual();std::cout<<"actual shared UI Font/Window/Icon startup tests PASS\n";}
