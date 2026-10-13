#include "item_selection.hpp"
#include <cassert>
#include <cmath>
namespace ttyd {
void tickItemSelectionSequence(ItemSelectionWindow& window,const ItemSelectionSequenceServices& s) {
    assert(window.sequence==0 || window.sequence==2);
    window.flags|=4;
    if(!window.action){window.flags&=~4u;return;}
    assert(window.action==1 || window.action==2);
    const bool opening=window.action==1;
    if(window.sequence==2) {
        if(opening)window.flags|=2;else {window.flags&=~2u;if(window.flags&8)window.flags&=~1u;}
        window.rotation=0;window.scale=1;window.alpha=255;window.action=0;return;
    }
    assert(s.refreshRate && s.interpolate);
    if(opening)window.flags|=2;
    const auto count=[&](){const uint32_t bits=uint32_t(s.refreshRate(s.user))<<3;
        const int32_t product=bits<=0x7fffffff?int32_t(bits):int32_t(int64_t(bits)-0x100000000LL);return product/60;};
    int32_t rotationStep=0,rotationCount=0;
    if(opening){rotationStep=window.clock;rotationCount=count();}
    else {rotationCount=count();rotationStep=window.clock;}
    window.rotation=s.interpolate(s.user,12,rotationStep,rotationCount,opening?30.f:0.f,opening?0.f:30.f);
    const int32_t scaleCount=count(),scaleStep=window.clock;
    window.scale=s.interpolate(s.user,12,scaleStep,scaleCount,opening?0.f:1.f,opening?1.f:0.f);
    window.alpha=255;const uint32_t next=uint32_t(window.clock)+1;
    window.clock=next<=0x7fffffff?int32_t(next):int32_t(int64_t(next)-0x100000000LL);
    const int32_t completionStep=window.clock,completionCount=count();
    if(completionStep>completionCount){window.action=0;if(!opening){window.flags&=~2u;if(window.flags&8)window.flags&=~1u;}}
}
void fillPackedExplicitItemRows(uint32_t context,uint32_t table,int32_t second,
                               const ItemSelectionRowsServices& s) {
    const auto& m=s.memory;assert((table&0x80000000u) && m.word && m.storeWord && m.storeHalf && s.allocate && s.clear);
    uint32_t count=0,offset=0,value;
    do{value=m.word(m.user,table+offset);++count;offset+=4;}while(value!=0xffffffffu);
    const uint32_t bytes=(count+1u)<<2;
    m.storeWord(m.user,context+0x30,s.allocate(s.user,0,bytes));
    s.clear(s.user,m.word(m.user,context+0x30),bytes);
    m.storeWord(m.user,context+0x34,0);
    if(second) {
        const uint32_t old=m.word(m.user,context+0x34),rows=m.word(m.user,context+0x30);
        m.storeWord(m.user,context+0x34,old+1);m.storeHalf(m.user,rows+4*old+2,uint16_t(second));
    }
    uint32_t cursor=table;offset=0;
    while(m.word(m.user,cursor)!=0xffffffffu) {
        const uint32_t old=m.word(m.user,context+0x34);cursor+=4;
        const uint32_t item=m.word(m.user,table+offset);offset+=4;
        const uint32_t rows=m.word(m.user,context+0x30);
        m.storeWord(m.user,context+0x34,old+1);m.storeHalf(m.user,rows+4*old+2,uint16_t(item));
    }
}
void tickItemSelectionMain(ItemSelectionMain& s,const ItemSelectionInput& input,
                           const ItemSelectionMainServices& v) {
    assert(v.windowFlags && v.action && v.resetHelp && v.deactivate && v.sound && v.status);
    if(s.state==0) {
        for(auto window:s.windows)if(v.windowFlags(v.user,window)&1){v.action(v.user,window,1);v.resetHelp(v.user,window);}
        v.resetHelp(v.user,s.windows[2]);v.sound(v.user,0x2002a);v.status(v.user,true);++s.state;return;
    }
    if(s.state==2) {
        for(auto window:s.windows)if(v.windowFlags(v.user,window)&1)v.action(v.user,window,2);
        v.sound(v.user,0x2002b);v.status(v.user,false);++s.state;return;
    }
    if(s.state==3) {
        if(v.windowFlags(v.user,s.windows[0])&4)return;
        for(auto window:s.windows)v.deactivate(v.user,window);
        s.flags|=0x1000;++s.state;return;
    }
    if(s.state!=1 || (v.windowFlags(v.user,s.windows[0])&4))return;
    // Positive, addressable authored tables are the current native domain.
    assert(!s.rows.empty() && s.rows.size()<=0x7fffffff && s.selected>=0 && size_t(s.selected)<s.rows.size());
    const int32_t count=int32_t(s.rows.size()),previous=s.selected;
    if(input.directionRepeat&0x2000)++s.selected;
    if(input.directionRepeat&0x1000)--s.selected;
    if(s.selected>=count){if(input.directionTrigger&0x2000){s.selected=0;s.firstVisible=0;}else --s.selected;}
    if(s.selected<0){if(input.directionTrigger&0x1000){s.selected=count-1;if(count-8>0)s.firstVisible=count-8;}else ++s.selected;}
    if(previous<s.selected && s.selected>5){if(s.firstVisible<count-8)++s.firstVisible;}
    else if(previous>s.selected && s.selected-s.firstVisible<3){if(s.firstVisible>0)--s.firstVisible;}
    if(s.selected!=previous)v.sound(v.user,0x20005);
    if(input.buttons&0x100) {
        if(s.rows[size_t(s.selected)].flags&2)v.sound(v.user,0x20014);
        else {v.sound(v.user,0x20012);++s.state;}
    } else if((input.buttons&0x200) && (s.flags&0x100)) {
        v.sound(v.user,0x20013);s.flags|=0x2000;++s.state;
    }
    s.displacement+=std::fma(24.f,float(s.firstVisible),-s.displacement)/6.f;
    s.cursorX+=(float(s.listX)-s.cursorX)/6.f;
    const float target=-std::fma(24.f,float(s.selected-s.firstVisible),-float(s.listY-54));
    s.cursorY+=(target-s.cursorY)/6.f;
}
int32_t packedItemSelectionResult(uint32_t context,const EventMemoryServices& m) {
    assert(m.word && m.half);
    const uint16_t flags=m.half(m.user,context);
    if(!(flags&0x1000))return 0;
    if(flags&0x2000)return -1;
    if(m.word(m.user,context+0x38) && m.word(m.user,context+0xc)==0 && (flags&0x100))return -1;
    const uint32_t index=m.word(m.user,context+0xc);
    const uint32_t rows=m.word(m.user,context+0x30);
    return m.half(m.user,rows+4*index+2);
}
int32_t itemSelectionEvent(EventEntry& e,uint32_t address,bool first,const ItemSelectionEventServices& s) {
    const auto& o=s.operands;
    assert(o.word && o.resolve && o.store && s.memory.word && s.memory.storeWord &&
           s.create && s.poll && s.selectedIndex && s.remove && s.keyOff && s.keyOn);
    const uint32_t arguments=e.arguments; // Original caches E+18 before callback side effects.
    const auto raw=[&](unsigned i){const uint32_t n=o.word(o.user,arguments+4*i);return n<=0x7fffffff?int32_t(n):int32_t(int64_t(n)-0x100000000LL);};
    const int32_t mode=o.resolve(o.user,e,raw(0));
    const uint32_t table=uint32_t(o.resolve(o.user,e,raw(1)));
    if(first) {
        const uint32_t context=s.create(s.user,table,0,1);
        s.memory.storeWord(s.memory.user,address+0x78,context);s.keyOff(s.user);
    }
    const uint32_t context=s.memory.word(s.memory.user,address+0x78);
    const int32_t result=s.poll(s.user,context);
    if(!result)return 0;
    const int32_t index=s.selectedIndex(s.user,context);
    s.remove(s.user,context);s.keyOn(s.user);
    o.store(o.user,e,raw(2),result);
    if(mode)o.store(o.user,e,raw(3),index);
    return 2;
}
}
