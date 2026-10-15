#pragma once
#include "battle_sequence_entry.hpp"

namespace ttyd {
// These are retained source invocations, not scheduling phases or actor IDs.
// Providers return empty/false before effects for a held memory operation.
// A child may progress before returning false; its queued operands stay pinned.
struct FieldPartyRemovalCursor {
    bool active=false,forget=false,queued=false;
    uint32_t index=0,phase=0,pending=0,actor=0,mario=0,slot=0;
    uint32_t linkedIndex=0,linked=0,definition=0,hook=0,value=0;
    BattleSequenceEntryServices owner;
    BattleSequenceCall call;
};
// partyKill800be69c / partyKill2 800be594. Both source scalar returns are0.
// forget selects Kill2's saved-kind zero; Kill still observes post-hook changes.
// No native index, null-linked-actor or active-pose validation is added here.
// Keep captured actors, Mario owners and service context alive across holds.
bool removeFieldParty(int32_t index,bool forget,const BattleSequenceEntryServices&,FieldPartyRemovalCursor&);

struct FieldPartyRemovalEventCursor {
    bool active=false,clear=false,queued=false;
    uint32_t entry=0,phase=0,pending=0,mario=0,args=0,slot=0,index=0;
    BattleSequenceEntryServices owner;
    BattleSequenceCall call;
};
// Actual EventEntry+18/raw args[0], without evtGetValue. Nonzero raw slots use
// literal actor index1: clear calls Kill2; kill calls Kill. Raw slot0 uses Kill
// with the captured signed primary byte. Result is assigned2 only on completion.
bool removeFieldPartyEvent(uint32_t entry,bool clear,const BattleSequenceEntryServices&,FieldPartyRemovalEventCursor&,int32_t& result);

struct FieldMarioPartyRemovalCursor {
    bool active=false,queued=false;
    uint32_t phase=0,pending=0,mario=0,index=0;
    BattleSequenceEntryServices owner;
    BattleSequenceCall call;
};
// marioPartyKill80141198: query Mario separately for each index byte, call Kill
// with its signed value, then explicitly ff the corresponding captured owner.
// This original void caller does not synthesize a scalar return value.
bool removeMarioParties(const BattleSequenceEntryServices&,FieldMarioPartyRemovalCursor&);
}
