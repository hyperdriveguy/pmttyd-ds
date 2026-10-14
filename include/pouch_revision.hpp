#pragma once
#include "prologue_pouch.hpp"
#include <optional>
namespace ttyd {
bool loadPouchRevisionAssets(const char* root);
bool pouchRevisionAvailable(const ProloguePouch&);
// Source Mario then party writes; finite original item/rank memory domain.
bool revisePouchParameters(ProloguePouch&);
// Original evt_pouch_remove_item result0/-1. nullopt is native admission failure,
// before any removal, rather than an invented original event result.
std::optional<int32_t> removeRevisedKeyItem(ProloguePouch&,int32_t item);
}
