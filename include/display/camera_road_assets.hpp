#pragma once
#include "field_camera_state.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <optional>
#include <vector>

namespace ttyd {
// Immutable original, big-endian authored camera container. Offsets remain
// file-relative; unknown fields and zero-count sentinel pointers stay intact.
class CameraRoadAsset {
public:
    struct Collection {uint32_t count=0,offset=0;};
    static constexpr uint32_t RoadStride=184,ZoneStride=104;
    bool decode(std::vector<uint8_t> bytes);
    bool load(const char* path);
    const std::vector<uint8_t>& bytes() const {return data;}
    const std::array<Collection,9>& collections() const {return blocks;}
    const std::string& version() const {return versionText;}
    uint32_t versionCode() const {return data.empty()?0:101;} // Decoder's explicit shipped-US1.01 domain.
    std::optional<std::array<float,3>> lensMetadata() const;
    std::optional<std::string> roadName(uint32_t index) const;
    std::optional<uint32_t> roadWord(uint32_t index,uint32_t field) const;
    std::optional<float> roadFloat(uint32_t index,uint32_t field) const;
    std::optional<CameraPoint> pathVector(uint32_t index) const;
    std::optional<CameraPoint> zoneVertex(uint32_t index) const;
    std::optional<uint32_t> zoneWord(uint32_t index,uint32_t field) const;
    std::optional<std::array<uint32_t,2>> polygon(uint32_t index) const;
    std::optional<uint32_t> vertexIndex(uint32_t index) const;
private:
    std::vector<uint8_t> data;
    std::array<Collection,9> blocks{};
    std::string versionText;
    std::optional<uint32_t> word(uint32_t offset) const;
    std::optional<float> number(uint32_t offset) const;
    std::optional<uint32_t> element(size_t collection,uint32_t index,uint32_t stride,uint32_t field) const;
};
// Native five-room asset service; all-or-nothing reload preserves prior data.
bool loadCameraRoadAssets(const char* directory);
const CameraRoadAsset* cameraRoadAsset(uint32_t room);
}
