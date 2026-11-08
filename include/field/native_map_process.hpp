#pragma once
#include "field_map_source.hpp"
#include <functional>
#include <memory>
namespace ttyd {
class NativeCameraProcess;
// CPU-only source-map namespace: no native source-map vertex/index/texture or
// display-list backing has been submitted, and no DMA targets these VM bytes.
// Providers and drawing run on the renderer thread. Current is pure and safe
// inside a VM raw borrow. The renderer must outlive all retained invocations.
struct NativeMapCpuServices {
 std::function<bool()> current;
 std::function<bool()> waitDrawSync; // Outside-frame completion of earlier GPU work.
 std::function<bool(uint8_t*,uint32_t)> flushBacking; // CPU ordering, not invalidation.
};
class NativeMapProcess {
public:
 using Estimate=std::function<std::optional<double>(float)>;
 struct Counts {unsigned heap0Allocations=0,stableFlushes=0,drawSyncs=0,vertexInvalidations=0,cacheRanges=0,estimates=0;};
 static std::shared_ptr<NativeMapProcess> create(std::shared_ptr<NativeCameraProcess>,const char* root,NativeMapCpuServices,Estimate);
 ~NativeMapProcess();
 // Actual lightInit then mapInit. No map data or GPU capture readiness inferred.
 bool initialize(unsigned visits=4096);
 bool initialized()const;
 // Retains one original mapLoad("gor_00") invocation. GX capture remains an
 // unavailable child until a complete native translation owner is supplied.
 bool loadHarbor(unsigned visits=4096);
 bool resume(unsigned visits=4096);
 bool current()const;bool busy()const;bool loaded()const;
 uint32_t pendingInstruction()const;
 std::optional<FieldMapSourceCall> pendingCall()const;
 Counts counts()const;
 uint32_t workAddress()const;uint32_t poolAddress()const;uint32_t poolBytes()const;
 std::shared_ptr<NativeCameraProcess> cameras()const;
private:
 NativeMapProcess();struct State;std::unique_ptr<State> state_;bool invoking_=false;
};
}
