#pragma once
#include "field_map_source.hpp"
#include "animation_sdk_matrix_binding.hpp"
#include <functional>
namespace ttyd {
// Shared original matrix engine, with explicit raw source-address outputs.
// Native finite vector domain requires complete stable raw spans per child;
// sparse/semantic aliases and full GQR/FPSCR/exception parity remain unbound.
class FieldMapSdk {
public:
 // Empty/undelivered estimate retains the source Distance child. The caller
 // chooses a Gekko model or an explicitly labeled native approximation.
 using Estimate=std::function<std::optional<double>(float)>;
 FieldMapSdk(EventAddressSpace&,std::function<bool()> current,Estimate={});
 FieldMapSdk(const FieldMapSdk&)=delete;FieldMapSdk& operator=(const FieldMapSdk&)=delete;
 bool advance(FieldMapSourceCall&);
private:
 bool vector(FieldMapSourceCall&);
 EventAddressSpace& memory_;std::function<bool()> current_;
 AnimationSdkMatrixBinding matrices_;
 uint32_t destination_=0;
 Estimate estimate_;
 struct DistanceState {bool active=false;FieldMapSourceCall call;float squared=0,half=0,three=0;} distance_;
 bool finishDistance(FieldMapSourceCall&);
};
}
