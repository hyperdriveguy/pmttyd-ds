#include "field_bgm_envelope.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main() {
 assert(fieldBgmDuration(1000,60)==60 && fieldBgmDuration(100,60)==6);
 assert(fieldBgmDuration(1100000,60)==464 && !fieldBgmDuration(1000000,60));
 assert(!fieldBgmDuration(0,60) && !fieldBgmDuration(1000,0) && !fieldBgmDuration(1,60));
 FieldBgmEnvelope fanfare;assert(fanfare.check()==-1 && !fanfare.off201(60));
 fanfare.bind(80);assert(fanfare.check()==0 && fanfare.main()->volume==80 && !fanfare.off201(0));
 assert(fanfare.off201(60));
 for(int remaining=60;remaining>0;--remaining) {
  assert(fanfare.primaryRemaining()==remaining);
  assert(fanfare.off201(1)); // Repeated Off does not restart source counters.
  const auto p=fanfare.main();assert(p && p->published && p->volume==80*remaining/60);
  assert(p->detach==(remaining==1));assert(p->tailMilliseconds==(remaining==1?100:0));
 }
 assert(fanfare.check()==-1 && !fanfare.main()->published);
 FieldBgmEnvelope field;field.bind(80);assert(field.mute3800(60));
 for(int r=60;r>24;--r){assert(field.secondaryRemaining()==r);auto p=field.main();assert(p->volume==80*(r*127/60)/127);}
 assert(field.silent() && field.secondaryRemaining()==24 && field.main()->volume==31);
 assert(field.mute3800(1) && field.secondaryRemaining()==24 && field.main()->volume==31);
 assert(field.resumeA0(60) && !field.silent() && field.primaryRemaining()==1);
 for(int r=24;r<=60;++r){assert(field.secondaryRemaining()==r);assert(field.secondaryState()==16);auto p=field.main();assert(p->volume==80*(r*127/60)/127);}
 assert(field.secondaryState()==0 && field.main()->volume==80 && field.check()==0);
 FieldBgmEnvelope plaza;plaza.bind(70,100,false);assert(plaza.mute3800(60));
 for(int i=0;i<36;++i)plaza.main();
 assert(plaza.main()->volume==27);
 assert(plaza.resumeA0(60));assert(plaza.main()->volume==27 && plaza.secondaryRemaining()==25); // On_f_d synchronous BGMMain.
 FieldBgmEnvelope fast;fast.bind(80,50,false);assert(fast.mute3800(60,true));
 for(int i=0;i<8;++i)fast.main();
 assert(fast.secondaryRemaining()==24 && fast.main()->volume==15);
 assert(fast.resumeA0(60,true));for(int i=0;i<8;++i)fast.main();assert(fast.secondaryRemaining()==60 && fast.secondaryState()==16);
 assert(fast.main()->volume==40 && fast.secondaryState()==0);
 fast.backendVacant();assert(!fast.silent() && !fast.main()->published);
 FieldStreamFade tail;assert(tail.begin(1,100,60));assert(tail.decrement()==1.f/6.f);
 for(int i=0;i<6;++i){auto p=tail.main();assert(p.published && !p.stop && p.volume==0);}
 assert(tail.volume()==0 && tail.active());auto stop=tail.main();assert(stop.stop && !stop.published && !tail.active());
 assert(!tail.main().stop && !tail.begin(-1,100,60));
 assert(tail.begin(80,100,60));const float before=tail.volume();assert(!tail.main(false).published && tail.volume()==before);
 assert(tail.main().volume==66);tail.clear();assert(!tail.main().published);
}
