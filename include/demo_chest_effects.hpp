#pragma once
#include <array>
#include <cstdint>
#include <vector>
namespace ttyd {
struct DemoEffectVertex {std::array<float,3> xyz{};std::array<float,2> uv{};std::array<uint8_t,4> color{};};
static_assert(sizeof(DemoEffectVertex)==24,"Original N64 export stride");
struct DemoCrystal {bool active=false;uint32_t type=0,age=0,camera=5;int32_t remaining=100,alpha=0;float scale=.5f;std::array<float,3> position{};};
struct DemoRadiationRay {int32_t remaining=0,age=0,alpha=0;std::array<float,3> angle{};float zVelocity=0;};
struct DemoEffectRequest {uint32_t sourceCommand=0,type=0,camera=5;uint64_t tick=0;std::array<float,3> position{};float scale=.5f;int32_t duration=0;};
struct DemoEffectDrawVertex {std::array<float,3> xyz{};std::array<float,2> uv{};std::array<float,4> color{};};
struct DemoEffectDraw {int texture=-1;bool textureColor=true;uint32_t camera=5;std::array<DemoEffectDrawVertex,6> vertices{};unsigned count=0;};
bool loadDemoChestEffectAssets(const char* root);
// Source-used CrystalN64 types2/6 and RadiationN64 type3 only. Scalar/alpha/
// lifetime/loop ordering is recovered; trig/matrices/manager cadence remain
// explicit native numeric/simulation adapters.
class DemoChestEffects {
public:
    std::array<DemoCrystal,2> crystals{};
    std::array<DemoRadiationRay,16> rays{};
    std::vector<DemoEffectRequest> requests;
    std::array<float,3> radiationPosition{};
    uint64_t tick=0;
    bool started=false,radiationActive=false,failed=false;
    void requestCrystals(uint64_t,std::array<float,3>);
    void requestRadiation(uint64_t,std::array<float,3>);
    void advance(uint64_t);
    std::vector<DemoEffectDraw> draw(float cameraYawDegrees) const;
private:
    void update();void updateCrystals();void updateRadiation();void resetRay(unsigned);
};
}
