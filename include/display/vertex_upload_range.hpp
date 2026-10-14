#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>

namespace ttyd {
// Inclusive indices, accumulated from actual writers, not whole map capacity.
struct VertexUploadRange {
    uint32_t first=std::numeric_limits<uint32_t>::max(),last=0;
    void include(uint32_t index) {
        first=std::min(first,index);last=std::max(last,index);
    }
    uint32_t count() const {return first>last?0:last-first+1;}
};
// Fixed workspace avoids allocation in the frame loop. Highly fragmented
// writes fall back to one enclosing range, retaining complete coverage.
class VertexUploadRanges {
public:
    void include(uint32_t index) {
        bounds.include(index);
        if(collapsed)return;
        if(used) {
            auto& tail=ranges[used-1];
            if(index>=tail.first && index<=tail.last)return;
            if(index==tail.last+1 || (index<tail.first && index+1==tail.first)) {
                tail.include(index);return;
            }
        }
        if(used==ranges.size()){collapsed=true;return;}
        ranges[used++].include(index);
    }
    template<class Flush> void flush(const Flush& writer) {
        if(!bounds.count())return;
        if(collapsed){writer(bounds.first,bounds.count());return;}
        std::sort(ranges.begin(),ranges.begin()+used,[](const auto& a,const auto& b){return a.first<b.first;});
        auto run=ranges[0];
        for(unsigned i=1;i<used;++i) {
            if(ranges[i].first<=run.last+1)run.include(ranges[i].last);
            else {writer(run.first,run.count());run=ranges[i];}
        }
        writer(run.first,run.count());
    }
private:
    std::array<VertexUploadRange,64> ranges{};
    VertexUploadRange bounds;
    unsigned used=0;
    bool collapsed=false;
};
}
