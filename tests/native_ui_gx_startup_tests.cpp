#include "native_ui_startup.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
EventNativePlatform platform(){
 EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
 p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};
 p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};
 p.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};return p;
}
std::shared_ptr<NativeUiStartup> actual(){auto process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());auto ui=NativeUiStartup::create(process,"romfs");assert(ui);return ui;}
uint32_t gpOf(const std::shared_ptr<NativeUiStartup>&ui){uint32_t gp;assert(ui->fieldProcess()->process().vm().memory.readWord(0x80415fd0,gp));return gp;}
void processAndRetirement(){
 auto ui=actual();auto gx=ui->gxHistory();assert(gx&&!gx->coldComplete()&&!gx->capture());
 assert(ui->resumeFont()&&ui->gxHistory()==gx&&gx->coldComplete());auto packet=gx->capture(16);assert(packet&&packet->size()==84&&packet->current());
 auto process=ui->fieldProcess();auto&vm=process->process().vm();assert(vm.memory.writeWord(gpOf(ui)+0x16c,2));
 assert(ui->language()==2&&packet->current()&&ui->resumeWindow()&&ui->windowReady());
 auto memory=ui->windowMemory();assert(memory&&ui->windowObject(50)&&gx->capture()&&packet->current());
 const auto counters=ui->counts();assert(ui->resumeWindow()&&ui->counts().gpStores==counters.gpStores&&ui->language()==2);
 ui->retire();assert(process->ready()&&!ui->gxHistory()&&!packet->current()&&!gx->current()&&!memory->current()&&!ui->windowReady());
}
void actualGpRegistration(){
 auto ui=actual();assert(ui->resumeFont());auto gx=ui->gxHistory();auto packet=gx->capture();assert(packet&&packet->current());
 auto process=ui->fieldProcess();auto&vm=process->process().vm();const auto gp=gpOf(ui);auto registration=vm.gpAddresses.languageRegistration(gp);assert(registration);
 assert(vm.gpAddresses.unregisterGp(gp));assert(vm.gpAddresses.registerGp(gp,vm.gp));assert(vm.gpAddresses.writeWord(0x80415fd0,gp));
 assert(vm.gpAddresses.languageRegistration(gp)!=registration&&process->ready());
 assert(!packet->current()&&!gx->current()&&!ui->gxHistory());
}
}
int main(){processAndRetirement();actualGpRegistration();std::cout<<"actual UI GX process/Window/language/registration retirement PASS\n";}
