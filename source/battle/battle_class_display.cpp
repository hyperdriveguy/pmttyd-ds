#include "battle_class_display.hpp"
namespace ttyd {
namespace {
bool same(const BattleActDrawServices& a,const BattleActDrawServices& b) {
    const auto& x=a.memory.memory;const auto& y=b.memory.memory;
    return a.user==b.user && a.memory.user==b.memory.user &&
        a.memory.byte==b.memory.byte && x.user==y.user && x.word==y.word &&
        x.half==y.half && x.storeWord==y.storeWord && x.storeHalf==y.storeHalf &&
        x.storeByte==y.storeByte && x.flushBlock==y.flushBlock &&
        x.systemCall==y.systemCall && a.camera==b.camera && a.center==b.center &&
        a.offset==b.offset && a.interpolate==b.interpolate &&
        a.translation==b.translation && a.rotation==b.rotation &&
        a.scale==b.scale && a.concat==b.concat && a.icons==b.icons &&
        a.partyButton==b.partyButton && a.pinchMark==b.pinchMark;
}
}
// Both queued camera argument and this immediate wrapper's B are ignored
// by original draw body. Do not substitute passed B for global draw ownership.
bool resumePackedBattleClassDisplay(uint32_t battle,
                                    const BattleActDrawServices& s,
                                    BattleActDrawCursor& c) {
    (void)battle;
    return resumePackedBattleActMenuDraw(s,c);
}
// Reject changed owner/services/passed B while pending without resetting
// progress. Lookup completes once; later child holds cannot replay it. This
// executes an already queued callback and does not insert another queue record.
bool resumePackedBattleClassQueuedDisplay(uint32_t camera,uint32_t battle,
                                          const BattleClassDisplayServices& s,
                                          BattleClassDisplayCursor& c) {
    (void)camera;
    if(c.active && (c.battle!=battle || c.owner.user!=s.user ||
       c.owner.cursorLookup!=s.cursorLookup || !same(c.owner.draw,s.draw)))return false;
    if(!c.active) {
        c.active=true;c.lookupComplete=false;c.battle=battle;
        c.lookupOutput.reset();c.owner=s;c.draw.reset();
    }
    if(!c.lookupComplete) {
        if(!s.cursorLookup || !s.cursorLookup(s.user,c.battle+0x171c,14,c.lookupOutput))return false;
        c.lookupComplete=true;
    }
    if(!resumePackedBattleClassDisplay(c.battle,s.draw,c.draw))return false;
    c.active=false;c.lookupComplete=false;c.lookupOutput.reset();
    return true;
}
}
