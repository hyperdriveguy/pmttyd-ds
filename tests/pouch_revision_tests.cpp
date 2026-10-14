#include "pouch_revision.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    auto p=initializeProloguePouch("Yoshi");
    assert(!pouchRevisionAvailable(p));
    const auto initial=p.bytes;
    assert(!removeRevisedKeyItem(p,9) && p.bytes==initial);
    assert(loadPouchRevisionAssets("romfs"));
    assert(!loadPouchRevisionAssets("/does-not-exist"));
    assert(pouchRevisionAvailable(p));
    p.half(0x70,7);p.half(0x74,2);
    // Final equipment slot participates, duplicates stack, all partner records
    // revise even when not recruited. Source badge IDs/costs are ISO backed.
    p.half(0x38a,278);p.half(0x38c,279);p.half(0x38e,280);p.half(0x38a+398,278);
    for(unsigned id=1;id<=7;++id){p.half(14*id+8,2);p.half(14*id+6,300);}
    assert(revisePouchParameters(p));
    assert(p.half(0x72)==20 && p.half(0x76)==10 && p.half(0x70)==7 && p.half(0x74)==2);
    assert(p.half(0x92)==-12); // Source costs3+6+3+3.
    const int expected[]={0,30,25,40,30,35,30,25};
    for(unsigned id=1;id<=7;++id){assert(p.half(14*id+4)==expected[id]);assert(p.half(14*id+2)==expected[id]+5);assert(p.half(14*id+6)==expected[id]+5);}
    p.half(0x70,300);p.half(0x74,300);
    assert(removeRevisedKeyItem(p,9)==0 && p.keyItemCount(9)==0);
    assert(p.half(0x70)==20 && p.half(0x74)==10);
    p.half(0x70,300);const auto before=p.bytes;
    assert(removeRevisedKeyItem(p,9)==-1 && p.bytes==before); // Failure skips revision.
    p.half(14+8,4);const auto invalid=p.bytes;
    assert(!removeRevisedKeyItem(p,6) && p.bytes==invalid);
    p.half(14+8,3);p.half(0x38a+398,340);assert(!pouchRevisionAvailable(p));
    p.half(0x38a+398,-1);assert(!pouchRevisionAvailable(p));
    p.half(0x38a+398,278);p.half(0x8e,32765);p.half(0x94,-32768);
    assert(revisePouchParameters(p));assert(p.half(0x72)==-32761 && p.half(0x70)==-32761);
    assert(p.half(0x92)==32753 && p.half(14+4)==200 && p.half(14+2)==205);
    std::cout<<"Original pouch revision tables, all equipment/party records, halfword wrap and removal ordering passed\n";
}
