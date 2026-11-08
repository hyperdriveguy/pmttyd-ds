#pragma once
#include "event_native_runtime.hpp"
#include <array>
#include <atomic>
#include <cassert>
#include <vector>
namespace ttyd::test {
// CPU-only cache-publication observer. Guest RAM is synchronously coherent on
// the host; a real host fence publishes prior stores. Record each source dcbf
// operand/line snapshot and its subsequent sc boundary, rather than silently
// dropping source effects. This does not emulate a Gekko or a native GPU cache.
class HostEventCache {
public:
    static constexpr uint32_t CacheLineBytes=32;
    struct Line {uint32_t operand;std::array<uint8_t,CacheLineBytes> bytes;};
    explicit HostEventCache(EventNativeRuntime& vm):vm_(vm),old_(vm.platform){
        // These fixtures previously had no platform owner. Do not steal a live
        // user pointer or replace an existing publication implementation.
        assert(!old_.user && !old_.flushBlock && !old_.systemCall);
        vm_.platform.user=this;
        vm_.platform.flushBlock=[](void* user,uint32_t address){static_cast<HostEventCache*>(user)->flush(address);};
        vm_.platform.systemCall=[](void* user){static_cast<HostEventCache*>(user)->publish();};
    }
    ~HostEventCache(){
        assert(vm_.platform.user==this);
        vm_.platform.user=old_.user;vm_.platform.flushBlock=old_.flushBlock;vm_.platform.systemCall=old_.systemCall;
    }
    HostEventCache(const HostEventCache&)=delete;
    HostEventCache& operator=(const HostEventCache&)=delete;
    void requireBattlePoolPublication() const {
        constexpr uint32_t RootCell=0x8041e80c,PoolBytes=0x64000;
        uint32_t root=0;assert(vm_.memory.readWord(RootCell,root) && root);
        assert(!boundaries_.empty() && boundaries_.front()==PoolBytes/CacheLineBytes);
        for(uint32_t i=0;i<PoolBytes/CacheLineBytes;++i){
            const auto& line=lines_[i];assert(line.operand==root+i*CacheLineBytes);
            for(auto byte:line.bytes)assert(byte==0);
        }
    }
private:
    void flush(uint32_t operand){
        Line line;line.operand=operand;const auto base=operand&~(CacheLineBytes-1);
        for(uint32_t i=0;i<CacheLineBytes;++i)assert(vm_.memory.readByte(base+i,line.bytes[i]));
        lines_.push_back(line);
    }
    void publish(){
        assert(lines_.size()>(boundaries_.empty()?0:boundaries_.back()));
        std::atomic_thread_fence(std::memory_order_seq_cst);boundaries_.push_back(lines_.size());
    }
    EventNativeRuntime& vm_;EventNativePlatform old_;
    std::vector<Line> lines_;std::vector<size_t> boundaries_;
};
}
