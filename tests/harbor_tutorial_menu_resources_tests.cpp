#include "harbor_tutorial.hpp"
#include <cassert>
using namespace ttyd;
namespace {
uint32_t word(const HarborTutorial& scene,uint32_t a){uint32_t v=0;assert(scene.readPackedWord(a,v));return v;}
}
int main(){
 assert(loadHarborTutorialAssets("romfs"));
 HarborTutorial first,second;
 assert(first.beginFresh(0,10,10,4) && second.beginFresh(0,10,10,4));
 assert(first.initializeClassMenuResources());
 const auto entry=word(first,0x840163f8),holder=word(first,entry+0xa0),payload=word(first,holder);
 assert(entry && holder && payload && word(first,payload)==0x0020af30);
 assert((word(first,entry)&0xffff)==1);
 assert(second.initializeClassMenuResources() && word(second,0x840163f8)==entry);
 assert((word(first,entry)&0xffff)==2);
 assert(first.endClassMenuResources() && (word(second,entry)&0xffff)==1);
 assert(second.endClassMenuResources() && (word(first,entry)>>24)==2);
 // Source End leaves B163f8 intact; only the source file state/count change.
 assert(word(first,0x840163f8)==entry && word(first,holder)==payload);
 assert(second.initializeClassMenuResources() && (word(first,entry)&0xffff)==1);
 // Reloading assets stages a new catalog owner. An already active battle keeps
 // its original actual backing while future VMs receive the new catalog.
 assert(loadHarborTutorialAssets("romfs"));
 HarborTutorial later;assert(later.beginFresh(0,10,10,4));
 assert(later.initializeClassMenuResources() && (word(later,entry)&0xffff)==1);
 assert((word(second,entry)&0xffff)==1);
 assert(second.endClassMenuResources() && (word(first,entry)>>24)==2);
 assert((word(later,entry)>>24)==1 && later.endClassMenuResources());
}
