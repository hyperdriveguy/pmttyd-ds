#pragma once
#include "battle_class_backend.hpp"
namespace ttyd {
// Connect source icon ordering and whole bubble/circle bodies to the actual
// original-assets backend. Source memory, camera query, math and numeric
// providers are required owners; backend supplies actual labels/font/icon/RNG.
// Appends in source order; does not begin, publish, acknowledge or clear a batch.
// False may follow provider effects/RNG/geometry and is NOT a resumable cursor.
// Execute only from a simulation-owned successful source draw pass, never by
// render-frame replay. Publish the batch only after the whole caller succeeds.
bool drawBattleClassBackendIcons(uint32_t work, uint32_t rows,
    const std::vector<BattleActDrawSlot>& slots,
    const BattleClassRasterServices&, BattleClassBackend&);
} // namespace ttyd
