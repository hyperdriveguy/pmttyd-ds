#pragma once
#include "event_dispatch.hpp"
#include "event_memory_opcodes.hpp"
#include <array>
#include <vector>
namespace ttyd {
// Packed original selector result reader; caller supplies valid context/row memory.
int32_t packedItemSelectionResult(uint32_t context,const EventMemoryServices&);
struct ItemSelectionRowsServices {
    EventMemoryServices memory;
    void* user=nullptr;
    uint32_t (*allocate)(void*,int32_t heap,uint32_t bytes)=nullptr;
    void (*clear)(void*,uint32_t address,uint32_t bytes)=nullptr;
};
// Original negative-address table branch of winMgrSelectEntry. Context and
// finite terminated table/arena extents belong to the caller's valid domain.
void fillPackedExplicitItemRows(uint32_t context,uint32_t table,int32_t second,
                               const ItemSelectionRowsServices&);
struct ItemSelectionEventServices {
    EventDispatchServices operands;
    EventMemoryServices memory;
    void* user=nullptr;
    uint32_t (*create)(void*,uint32_t table,int32_t second,int32_t third)=nullptr;
    int32_t (*poll)(void*,uint32_t context)=nullptr;
    int32_t (*selectedIndex)(void*,uint32_t context)=nullptr;
    void (*remove)(void*,uint32_t context)=nullptr;
    void (*keyOff)(void*)=nullptr;
    void (*keyOn)(void*)=nullptr;
};
// Original evt_win_item_select. Entry+78 is retained in the packed memory
// provider, not a host pointer or a MessageSelection object.
int32_t itemSelectionEvent(EventEntry&,uint32_t entryAddress,bool firstCall,
                           const ItemSelectionEventServices&);
struct ItemSelectionRow {uint16_t flags=0,item=0;};
struct ItemSelectionWindow {
    uint32_t flags=0;
    int32_t action=0,clock=0,sequence=0;
    uint32_t alpha=0;
    float scale=0,rotation=0;
};
struct ItemSelectionSequenceServices {
    void* user=nullptr;
    int32_t (*refreshRate)(void*)=nullptr; // Original GP.word[4] input.
    float (*interpolate)(void*,int32_t mode,int32_t step,int32_t count,float from,float to)=nullptr;
};
// Source sequence0/2 subset for explicit item tables. Other definition kinds
// require their own original branches, not a linear/no-op fallback.
void tickItemSelectionSequence(ItemSelectionWindow&,const ItemSelectionSequenceServices&);
struct ItemSelectionInput {uint32_t directionRepeat=0,directionTrigger=0,buttons=0;};
struct ItemSelectionMain {
    uint16_t flags=0x100;
    int32_t state=0,selected=0,firstVisible=0;
    int32_t listX=-24,listY=116;
    float cursorX=-24,cursorY=62,displacement=0;
    std::vector<ItemSelectionRow> rows;
    std::array<uint32_t,3> windows{}; // Native tokens for the original three manager slots.
};
struct ItemSelectionMainServices {
    void* user=nullptr;
    uint32_t (*windowFlags)(void*,uint32_t)=nullptr;
    void (*action)(void*,uint32_t,int32_t action)=nullptr; // Also resets manager clock0.
    void (*resetHelp)(void*,uint32_t)=nullptr; // Page/text/lineCount to0.
    void (*deactivate)(void*,uint32_t)=nullptr; // Clears manager activeflags1 only.
    void (*sound)(void*,uint32_t sourceID)=nullptr;
    void (*status)(void*,bool open)=nullptr;
};
// select_main: transition/interpolation and key repeat production are external
// services. Call once for the list owner, not once per owned window.
void tickItemSelectionMain(ItemSelectionMain&,const ItemSelectionInput&,
                           const ItemSelectionMainServices&);
}
