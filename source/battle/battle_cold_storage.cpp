#include "battle_cold_storage.hpp"
#include <cassert>

namespace ttyd {
namespace {
constexpr uint32_t BattlePointer = 0x8041eb90;
constexpr uint32_t PadBytes = 0x1fc;
constexpr unsigned GeneralPadCount = 4, RegistrySlots = 64;
constexpr unsigned SnapshotBytes = 12, SnapshotCount = 2, HistoryWords = 30;
void resetPad(uint32_t pad, const EventMemoryServices& m) {
    assert(m.storeHalf && m.storeByte && m.storeWord);
    // Preserve byte b of each snapshot, the release bank and the raw tail.
    for (unsigned snapshot = 0; snapshot < SnapshotCount; ++snapshot) {
        const auto p = pad + SnapshotBytes * snapshot;
        m.storeHalf(m.user, p, 0);
        for (unsigned byte = 2; byte <= 10; ++byte) m.storeByte(m.user, p + byte, 0);
    }
    for (unsigned index = 0; index < HistoryWords; ++index) {
        const auto p = pad + 4 * index;
        m.storeWord(m.user, p + 0x18, 0);
        m.storeWord(m.user, p + 0x90, 0);
        m.storeWord(m.user, p + 0x108, 0);
    }
    m.storeByte(m.user, pad + 0x1f8, 0);
    m.storeByte(m.user, pad + 0x1f9, 0);
}
}
std::optional<uint32_t> initializePackedBattleColdStorage(uint32_t function, uint32_t argument,
                                                         const BattleActorServices& actors) {
    const auto& m = actors.memory;
    switch (function) {
    case 0x80216908: return 0; // Whole BattleAcHelpInit is blr.
    case 0x8011a38c: resetPad(argument, m); return 0;
    case 0x8011a48c:
        assert(m.word);
        for (unsigned pad = 0; pad < GeneralPadCount; ++pad)
            resetPad(m.word(m.user, BattlePointer) + 0xf2c + PadBytes * pad, m);
        return 0;
    case 0x800fa62c:
        resetPad(argument + 0x1d1c, m);
        for (auto offset : {0x1ca0u, 0x1ca4u, 0x1ca8u, 0x1cacu}) m.storeWord(m.user, argument + offset, 0);
        m.storeWord(m.user, argument + 0x1f18, 0xffffffff);
        return 0;
    case 0x800f7af0: {
        assert(m.word && m.storeWord);
        const auto records = m.word(m.user, BattlePointer) + 0x16f5c;
        for (unsigned slot = 0; slot < RegistrySlots; ++slot) {
            m.storeWord(m.user, records + 8 * slot, 0xffffffff);
            m.storeWord(m.user, records + 8 * slot + 4, 0);
        }
        return 0;
    }
    case 0x80129694: {
        assert(m.word && m.storeWord);
        const auto battle = m.word(m.user, BattlePointer);
        for (unsigned slot = 0; slot < RegistrySlots; ++slot) setBattleRegistryUnit(battle, int32_t(slot), 0, actors);
        return 1;
    }
    case 0x801aa228: {
        assert(m.word);
        const auto icons = m.word(m.user, BattlePointer) + 0x182cc;
        eventFillMemory(icons, 0, 0x9c0, m);
        return 0;
    }
    case 0x80195a94: {
        assert(m.word && m.storeByte);
        const auto messages = m.word(m.user, BattlePointer) + 0x18da0;
        eventFillMemory(messages, 0, 600, m);
        for (unsigned slot = 0; slot < 28; ++slot) m.storeByte(m.user, messages + 12 * slot, 255);
        return 0;
    }
    case 0x800f7c20: {
        assert(m.word && m.storeWord && m.storeHalf);
        const auto initialScalar = m.word(m.user, 0x80422174);
        // Seven original kinds224..230, records44 bytes. Preserve the gaps
        // around the 30-byte status snapshot and its later resource halves.
        for (unsigned party = 0; party < 7; ++party) {
            const auto record = argument + 0x162a0 + 44 * party;
            m.storeWord(m.user, record, initialScalar);
            m.storeWord(m.user, record + 0x24, 0);
            eventFillMemory(record + 4, 0, 30, m);
            m.storeHalf(m.user, record + 0x28, 0);
            m.storeHalf(m.user, record + 0x2a, 0);
        }
        return 0;
    }
    default: return {};
    }
}
}
