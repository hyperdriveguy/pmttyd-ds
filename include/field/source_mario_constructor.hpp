#pragma once
#include "field_map_source.hpp"
namespace ttyd::SourceMarioConstructor {
// Whole bodies execute in the shared fixed native continuation graph. Original
// source PCs name holds; budgets/private stack frames are native policy.
constexpr uint32_t Init = 0x8005b5c8, PouchInit = 0x800d59c8,
                   PouchCheckItem = 0x800d4ed4, PouchGetItem = 0x800d50cc;
constexpr uint32_t MarioGlobal = 0x8041e900, MarioBytes = 0x2f8,
                   PouchGlobal = 0x8041eb00, PouchBytes = 0x5d4,
                   MarioPoseHandleOffset = 0x22c,
                   BlurIndex = 0x8041e8f0, BlurPositions = 0x803d9848,
                   BlurBytes = 0x60;
} // namespace ttyd::SourceMarioConstructor
