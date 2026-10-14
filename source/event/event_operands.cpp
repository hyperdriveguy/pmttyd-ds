#include "event_operands.hpp"
#include <cassert>

// Signed operand tags select original storage; they are not host pointers.
// Numeric band/base tables and API asymmetry: docs/code/event-vm.md.
// Preserve wrapped subtraction and provider call order even for immediates.
namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t index(int32_t operand,int32_t base) {return sw(static_cast<uint32_t>(operand)-static_cast<uint32_t>(base));}
uint32_t read(const EventOperandServices& s,uint32_t a) {assert(s.readWord);return s.readWord(s.user,a);}
void write(const EventOperandServices& s,uint32_t a,uint32_t v) {assert(s.writeWord);s.writeWord(s.user,a,v);}
uint32_t context(const EventOperandServices& s) {assert(s.contextAddress);return s.contextAddress(s.user);}
uint32_t entry(EventEntry& e,const EventOperandServices& s) {assert(s.entryAddress);return s.entryAddress(s.user,e);}
float mul(float a,float b) {return a*b;}
// Encoded float = signed32(word + 230000000) / 1024. The .0009765625
// factor is exact 2^-10; stores multiply by1024 before fctiwz and rebiasing.
float decode(int32_t word) {return mul(static_cast<float>(sw(static_cast<uint32_t>(word)+230000000u)),.0009765625f);}
int32_t truncate(const EventOperandServices& s,float f) {assert(s.truncate);return s.truncate(s.user,f);}
struct Bit {uint32_t address,mask;};
// Flags use 32-bit words at4-byte stride. Signed division truncates toward
// zero; negative indices are not normalized into a guessed positive bit index.
Bit bit(uint32_t base,int32_t i) {const int32_t q=i/32,r=i-32*q;return {base+4u*static_cast<uint32_t>(q),eventShiftMask(r)};}
int32_t readBit(const EventOperandServices& s,Bit b) {return (read(s,b.address)&b.mask)?1:0;}
void writeBit(const EventOperandServices& s,Bit b,bool value) {const uint32_t old=read(s,b.address);write(s,b.address,value?old|b.mask:old&~b.mask);}
enum class Domain {Immediate,Encoded,UF,UW,GSW,LSW,GSF,LSF,GF,LF,GW,LW};
// Upper bounds -250M/-220M then20M-spaced bands classify namespaces.
// Each namespace index is operand minus its midpoint base (see address helpers).
Domain integerDomain(int32_t op) {
    if(op<=-250000000)return Domain::Immediate;
    if(op<=-220000000)return Domain::Encoded;
    if(op<=-200000000)return Domain::UF;
    if(op<=-180000000)return Domain::UW;
    if(op<=-160000000)return Domain::GSW;
    if(op<=-140000000)return Domain::LSW;
    if(op<=-120000000)return Domain::GSF;
    if(op<=-100000000)return Domain::LSF;
    if(op<=-80000000)return Domain::GF;
    if(op<=-60000000)return Domain::LF;
    if(op<=-40000000)return Domain::GW;
    if(op<=-20000000)return Domain::LW;
    return Domain::Immediate;
}
// Work offsets: context+4=GW, entry+0x9c=LW. UW uses caller-supplied base.
// Original words are4 bytes even when the host has64-bit addresses.
uint32_t wordAddress(Domain d,EventEntry& e,uint32_t c,int32_t op,const EventOperandServices& s) {
    if(d==Domain::UW)return e.userWork+4u*static_cast<uint32_t>(index(op,-190000000));
    if(d==Domain::GW)return c+4+4u*static_cast<uint32_t>(index(op,-50000000));
    assert(d==Domain::LW);return entry(e,s)+0x9c+4u*static_cast<uint32_t>(index(op,-30000000));
}
// context+0x84=GF, entry+0xdc=LF; UF uses the supplied user flag base.
Bit flagAddress(Domain d,EventEntry& e,uint32_t c,int32_t op,const EventOperandServices& s) {
    if(d==Domain::UF)return bit(e.userFlags,index(op,-210000000));
    if(d==Domain::GF)return bit(c+0x84,index(op,-90000000));
    assert(d==Domain::LF);return bit(entry(e,s)+0xdc,index(op,-70000000));
}
SavedSwitchFamily family(Domain d) {
    if(d==Domain::GSW)return SavedSwitchFamily::GlobalByte;
    if(d==Domain::LSW)return SavedSwitchFamily::LocalByte;
    if(d==Domain::GSF)return SavedSwitchFamily::GlobalFlag;
    assert(d==Domain::LSF);return SavedSwitchFamily::LocalFlag;
}
int32_t savedIndex(Domain d,int32_t op) {
    return index(op,d==Domain::GSW?-170000000:d==Domain::LSW?-150000000:d==Domain::GSF?-130000000:-110000000);
}
// Original float getter intentionally omits distinct UF/GSF/LSF bands.
// Those operands route through the broader UW/GF bands, unlike integer reads.
Domain floatReadDomain(int32_t op) {
    if(op<=-250000000)return Domain::Immediate;
    if(op<=-220000000)return Domain::Encoded;
    if(op<=-180000000)return Domain::UW;
    if(op<=-160000000)return Domain::GSW;
    if(op<=-140000000)return Domain::LSW;
    if(op<=-80000000)return Domain::GF;
    if(op<=-60000000)return Domain::LF;
    if(op<=-40000000)return Domain::GW;
    if(op<=-20000000)return Domain::LW;
    return Domain::Immediate;
}
}
// PowerPC slw examines low6 shift bits: counts32..63 yield zero, not a
// C++ undefined >=32 shift. Counts wrap modulo64 before that test.
uint32_t eventShiftMask(int32_t count) {
    const uint32_t n=static_cast<uint32_t>(count)&63;return (n&32)?0:1u<<(n&31);
}
// GP saved storage: story word+0x174 (GSW0 special case), global bytes+0x578,
// local bytes+0xdb8, global flags+0x178, local flags+0xd78. Reload GP per leaf.
int32_t readSavedSwitch(SavedSwitchFamily f,int32_t i,const EventOperandServices& s) {
    assert(s.globalAddress);const uint32_t gp=s.globalAddress(s.user);
    if(f==SavedSwitchFamily::GlobalByte) {
        if(i==0)return sw(read(s,gp+0x174));
        assert(s.readByte);return s.readByte(s.user,gp+0x578+static_cast<uint32_t>(i));
    }
    if(f==SavedSwitchFamily::LocalByte) {assert(s.readByte);return s.readByte(s.user,gp+0xdb8+static_cast<uint32_t>(i));}
    return readBit(s,bit(gp+(f==SavedSwitchFamily::GlobalFlag?0x178:0xd78),i));
}
void writeSavedSwitch(SavedSwitchFamily f,int32_t i,int32_t value,const EventOperandServices& s) {
    assert(s.globalAddress);const uint32_t gp=s.globalAddress(s.user);
    if(f==SavedSwitchFamily::GlobalByte && i==0) {write(s,gp+0x174,static_cast<uint32_t>(value));return;}
    if(f==SavedSwitchFamily::GlobalByte || f==SavedSwitchFamily::LocalByte) {
        assert(s.writeByte);s.writeByte(s.user,gp+(f==SavedSwitchFamily::GlobalByte?0x578:0xdb8)+static_cast<uint32_t>(i),static_cast<uint8_t>(value));return;
    }
    writeBit(s,bit(gp+(f==SavedSwitchFamily::GlobalFlag?0x178:0xd78),i),value!=0);
}
int32_t getEventValue(EventEntry& e,int32_t op,const EventOperandServices& s) {
    const uint32_t c=context(s);const Domain d=integerDomain(op);
    switch(d) {
    case Domain::Immediate:return op;
    case Domain::Encoded:return truncate(s,decode(op));
    case Domain::UF:case Domain::GF:case Domain::LF:return readBit(s,flagAddress(d,e,c,op,s));
    case Domain::UW:case Domain::GW:case Domain::LW: {
        const int32_t w=sw(read(s,wordAddress(d,e,c,op,s)));
        return w<=-270000000 || w>-220000000?w:truncate(s,decode(w));
    }
    default:return readSavedSwitch(family(d),savedIndex(d,op),s);
    }
}
// Writes to word/byte storage return its OLD value; flag writes return the
// requested value. Immediate/encoded destinations can return without a store.
int32_t setEventValue(EventEntry& e,int32_t op,int32_t value,const EventOperandServices& s) {
    const uint32_t c=context(s);
    if(op<=-270000000 || op>-20000000)return value;
    if(op<=-220000000)return truncate(s,value<=-220000000?decode(value):static_cast<float>(value));
    const Domain d=integerDomain(op);
    if(d==Domain::UF || d==Domain::GF || d==Domain::LF) {writeBit(s,flagAddress(d,e,c,op,s),value!=0);return value;}
    if(d==Domain::UW || d==Domain::GW || d==Domain::LW) {
        const uint32_t address=wordAddress(d,e,c,op,s);const int32_t old=sw(read(s,address));write(s,address,static_cast<uint32_t>(value));return old;
    }
    const auto f=family(d);const int32_t i=savedIndex(d,op),old=readSavedSwitch(f,i,s);
    writeSavedSwitch(f,i,d==Domain::LSW?static_cast<uint8_t>(value):value,s);return old;
}
float getEventFloat(EventEntry& e,int32_t op,const EventOperandServices& s) {
    const uint32_t c=context(s);const Domain d=floatReadDomain(op);
    if(d==Domain::Immediate)return static_cast<float>(op);
    if(d==Domain::Encoded)return decode(op);
    if(d==Domain::GF || d==Domain::LF)return static_cast<float>(readBit(s,flagAddress(d,e,c,op,s)));
    const int32_t w=d==Domain::GSW || d==Domain::LSW?readSavedSwitch(family(d),savedIndex(d,op),s):sw(read(s,wordAddress(d,e,c,op,s)));
    return w<=-220000000?decode(w):static_cast<float>(w);
}
// Float setter is also asymmetric: no saved-switch routing. It uses broader
// UW/GF bands and returns the decoded OLD word for work stores. No IEEE float
// bits are stored here: this is the original quantized event representation.
float setEventFloat(EventEntry& e,int32_t op,float value,const EventOperandServices& s) {
    const uint32_t c=context(s);
    if(op<=-220000000 || op>-20000000)return value;
    Domain d;
    if(op<=-180000000)d=Domain::UW;
    else if(op<=-80000000)d=Domain::GF;
    else if(op<=-60000000)d=Domain::LF;
    else if(op<=-40000000)d=Domain::GW;
    else d=Domain::LW;
    if(d==Domain::GF || d==Domain::LF) {writeBit(s,flagAddress(d,e,c,op,s),value!=0.f);return value;}
    const float product=mul(value,1024.f);const uint32_t address=wordAddress(d,e,c,op,s);
    const int32_t old=sw(read(s,address));const int32_t converted=truncate(s,product);
    write(s,address,static_cast<uint32_t>(converted)-230000000u);
    return old<=-220000000?decode(old):static_cast<float>(old);
}
}
