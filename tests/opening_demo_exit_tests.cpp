#include "opening_demo_exit.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    OpeningDemoExit f;f.begin(100);assert(!f.request(0x100,false));
    f.advance(1000);assert(!f.entryFinished); // Unadmitted resources cannot advance.
    f.admitEntry();assert(f.entryType==9 && f.entryMilliseconds==500 && f.entryAlpha()==255);
    f.advance(1015);assert(f.entryAlpha()==127 && !f.entryFinished);
    f.entryPresented(0);assert(!f.entryFinished); // Fabricated early alpha rejected.
    f.advance(1030);assert(!f.entryDriverFinished);f.advance(1031);assert(f.entryDriverFinished && !f.entryFinished);
    f.entryPresented(1);assert(!f.entryFinished);f.entryPresented(0);assert(f.entryFinished);
    assert(!f.request(0x200,false));assert(f.request(0x1000,false));
    assert(!f.finished() && f.configuredMilliseconds==500 && f.actualMilliseconds==2000);
    f.advance(1091);assert(f.alpha()==127 && !f.finished());
    f.advance(1151);assert(!f.driverFinished);
    f.advance(1152);assert(f.driverFinished && f.alpha()==255 && !f.finished());
    f.presented(254);assert(!f.finished());f.presented(255);assert(f.finished());
    assert(!f.requestAutomatic());
    f.begin(1500);assert(!f.requestAutomatic());f.admitEntry();f.advance(1531);f.entryPresented(0);assert(f.requestAutomatic() && f.parentExitRequest);f.advance(1652);assert(f.driverFinished && !f.finished());f.presented(255);assert(f.finished());
    f.begin(2000);assert(!f.finished() && !f.entryFinished && f.entryAlpha()==255);
    f.admitEntry();f.advance(2031);f.entryPresented(0);
    assert(f.request(0x100,true) && f.parentExitRequest && f.configuredMilliseconds==2000);
    f.advance(2152);assert(!f.finished());f.presented(255);assert(f.finished());
}
