#include "map_draw_cache.hpp"
#include "vertex_upload_range.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <vector>

int main() {
    ttyd::VertexUploadRange upload;
    assert(upload.count()==0);
    upload.include(80);upload.include(4);upload.include(80);upload.include(12);
    assert(upload.first==4 && upload.count()==77);
    upload.include(0);
    assert(upload.first==0 && upload.count()==81);
    ttyd::VertexUploadRanges ranges;
    std::vector<std::array<uint32_t,2>> flushed;
    auto flush=[&](uint32_t first,uint32_t count){flushed.push_back({first,count});};
    ranges.flush(flush);assert(flushed.empty());
    for(auto index:{10u,11u,12u,4u,5u,11u,6u,7u,8u,9u,80u})ranges.include(index);
    ranges.flush(flush);
    assert((flushed==std::vector<std::array<uint32_t,2>>{{4,9},{80,1}}));
    flushed.clear();
    ttyd::VertexUploadRanges fragmented;
    for(unsigned i=0;i<100;++i)fragmented.include(i*3);
    fragmented.flush(flush);
    assert((flushed==std::vector<std::array<uint32_t,2>>{{0,298}}));
    int textures[3]{};
    ttyd::MapDrawCache cache;
    auto first=cache.apply(textures,0,0,0,0);
    assert(first.texture && first.cull && first.depth);
    auto same=cache.apply(textures,0,0,0,0);
    assert(!same.texture && !same.cull && !same.depth);
    // A different descriptor needs its own wrap setup, even for equal modes.
    assert(cache.apply(textures+1,0,0,0,0).texture);
    // The same pointer with changed wrap must be rebound after the mutation.
    assert(cache.apply(textures+1,1,2,0,0).texture);
    auto material=cache.apply(textures+1,1,2,2,15);
    assert(!material.texture && material.cull && material.depth);

    // Replay mixed passes against unconditional state setup with arbitrary
    // incoming UI state. Every submitted draw must see the identical state.
    uint32_t random=12345;
    auto next=[&](){random=random*1664525u+1013904223u;return random;};
    for(unsigned pass=0;pass<100;++pass) {
        ttyd::MapDrawCache local;
        const void* bound=nullptr;
        uint32_t cull=99,mask=99,s=99,t=99;
        bool depthEnabled=false;
        std::array<std::array<uint32_t,2>,3> descriptors{};
        for(auto& descriptor:descriptors)descriptor={next()%3,next()%3};
        for(unsigned draw=0;draw<100;++draw) {
            const auto id=next()%3, ws=next()%3, wt=next()%3;
            const auto wantedCull=next()%3,wantedMask=next()%16;
            const auto changed=local.apply(textures+id,ws,wt,wantedCull,wantedMask);
            if(changed.texture) {
                descriptors[id]={ws,wt};bound=textures+id;
                s=descriptors[id][0];t=descriptors[id][1];
            }
            if(changed.cull)cull=wantedCull;
            if(changed.depth){mask=wantedMask;depthEnabled=true;}
            assert(bound==textures+id && s==ws && t==wt);
            assert(cull==wantedCull && mask==wantedMask && depthEnabled);
        }
    }
}
