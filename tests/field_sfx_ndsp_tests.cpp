#include "field_sfx_ndsp.hpp"
#include "mario_motion_sfx.hpp"
#include <cassert>
#include <vector>
std::vector<int> stops;
void ndspChnWaveBufClear(int channel){stops.push_back(channel);}
using namespace ttyd;
int main() {
 FieldSfxNdsp native;MarioMotionSfx mario;
 assert(mario.idle==-1 && mario.secondary==-1 && mario.fall==-1);
 assert(mario.post(0,native.registry(),native.services()) && stops.empty());
 assert(!native.attach(40,225,0x200002c1,20));
 assert(!native.attach(0,225,0x200002c1,18)); // Music channel cannot be adopted.
 assert(!native.attach(0,225,0x200002c1,14)); // Opening-house voice cannot be adopted.
 assert(!native.attach(0,225,0x200002c1,19)); // Opening-map voice cannot be adopted.
 assert(!native.attach(0,225,0x800002c1,20)); // Stream/song backend is separate.
 assert(native.attach(0x100,225,0x200002c1,20));mario.idle=0x100;
 assert(!native.attach(0x101,226,0x200002c2,20)); // Another owner already has this channel.
 assert(!native.attach(0x100,225,0x200002c1,21));
 assert(native.attach(0x101,226,0x200002c2,21));mario.secondary=0x101;
 assert(native.attach(0x102,91,0x20000112,22));mario.fall=0x102;
 assert(!native.canOff(mario.idle,false) && native.canOff(mario.idle,true));
 assert(mario.post(0,native.registry(),native.services()));assert((stops==std::vector<int>{20,21}));
 assert(mario.idle==-1 && mario.secondary==-1 && mario.fall==0x102);
 assert(mario.post(15,native.registry(),native.services()) && stops.size()==2);
 assert(mario.post(10,native.registry(),native.services()));assert(stops.back()==22 && mario.fall==-1);
 assert(mario.post(10,native.registry(),native.services()) && stops.size()==3);
 // Original Off ignores the generation and stops the current low8 slot.
 assert(native.attach(0x900,225,0x200002c1,20));mario.idle=0x100;
 assert(mario.post(0,native.registry(),native.services()) && stops.back()==20);
 assert(native.registry().slots[0].key==-1 && native.registry().slots[0].engine==-1);
 assert(native.attachPending(0x200,239,0x200002cb));mario.idle=0x200;
 const auto before=stops.size();assert(native.canOff(mario.idle,false));
 assert(mario.post(0,native.registry(),native.services()) && stops.size()==before && mario.idle==-1);
 // Unavailable bank/backend preserves the borrowed handle at its frontier.
 mario.idle=40;assert(!mario.post(0,native.registry(),native.services()) && mario.idle==40);
 mario.idle=-1;assert(native.attach(0x300,225,0x200002c1,20));assert(native.clear());
 const auto count=stops.size();assert(native.clear() && stops.size()==count);
}
