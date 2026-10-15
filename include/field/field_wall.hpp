#pragma once
#include <cstdint>

namespace ttyd {
// Original engine-unit coordinates. Native geometry adapters own scale/cull and
// hit lifetime; this semantic interface is not the original hit-record ABI.
struct FieldWallVector {float x=0,y=0,z=0;};
// token0 is original miss; distanceWritten separately controls caller cache.
// Token lifetime and native coordinate conversion belong to the ray provider.
struct FieldWallHit {
    uintptr_t token=0;
    float distance=0;
    FieldWallVector normal;
    uint32_t attributes=0;
    uint32_t objectKey=0; // Dynamic namespace, separate from the geometry token.
};
struct FieldWallActor {
    uint32_t flags=0,viewFlags=0;
    uint16_t motion=0;
    int16_t counter=0;
    float width=20,height=37;
};
struct FieldWallServices {
    void* user=nullptr;
    FieldWallHit (*ray)(void*,FieldWallVector origin,FieldWallVector direction,float length)=nullptr;
};
// Side-only blockage need not return a forward hit. Contact distance may be
// written from the last side result; do not replace it with nearest forward range.
struct FieldWallMove {
    FieldWallVector position;
    FieldWallHit forward;
    bool sideBlocked=false;
    bool distanceWritten=false;
    float contactDistance=0;
};
float quantizeFieldWall(float);
bool fieldWallPassThrough(uint32_t attributes,const FieldWallActor&);
// Ordinary forward/side policy: excludes transformed, forced/map-specific
// movement, balloon and named-block/hazard event ownership.
FieldWallMove searchFieldFrontWall(FieldWallVector,FieldWallVector displacement,
                                  const FieldWallActor&,const FieldWallServices&);
// mode0 is the ordinary marioCheckWall post-pass. Two surrounding passes retain
// separate first/second pushes and the original second-pass dot admission.
bool correctFieldWallsAround(FieldWallVector&,FieldWallVector heading,int32_t mode,
                            const FieldWallActor&,const FieldWallServices&);
}
