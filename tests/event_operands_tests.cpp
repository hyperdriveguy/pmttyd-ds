#include "event_operands.hpp"
#include <cassert>
#include <cmath>
#include <map>
#include <vector>
#include <limits>
#include <iostream>
using namespace ttyd;
struct Fixture {
    EventEntry entry;uint32_t context=0x2000,gp=0x3000,base=0x1000;
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<int> calls;std::vector<uint32_t> addresses;
    bool changeGp=false,mutateDuringConversion=false;int gpReads=0;
    Fixture() {entry.userWork=0x5000;entry.userFlags=0x6000;}
    uint32_t read(uint32_t address) {
        if(address>=base+0x9c && address<base+0xdc && (address-base-0x9c)%4==0)return uint32_t(entry.locals[(address-base-0x9c)/4]);
        return words[address];
    }
    void write(uint32_t address,uint32_t value) {
        if(address>=base+0x9c && address<base+0xdc && (address-base-0x9c)%4==0)entry.locals[(address-base-0x9c)/4]=value<=0x7fffffff?int32_t(value):int32_t(int64_t(value)-0x100000000LL);
        else words[address]=value;
    }
    EventOperandServices services() {
        EventOperandServices s;s.user=this;
        s.contextAddress=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);return f.context;};
        s.entryAddress=[](void* p,EventEntry& e) {auto& f=*static_cast<Fixture*>(p);assert(&e==&f.entry);f.calls.push_back(2);return f.base;};
        s.globalAddress=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(6);++f.gpReads;return f.changeGp && f.gpReads>1?0x4000u:f.gp;};
        s.readWord=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);f.addresses.push_back(a);return f.read(a);};
        s.writeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(5);f.addresses.push_back(a);f.write(a,v);};
        s.readByte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(7);f.addresses.push_back(a);return f.bytes[a];};
        s.writeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(8);f.addresses.push_back(a);f.bytes[a]=v;};
        s.truncate=[](void* p,float x)->int32_t {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);assert(std::isfinite(x) && double(x)>=-2147483648.0 && double(x)<2147483648.0);
            if(f.mutateDuringConversion)f.entry.locals[0]=888;return static_cast<int32_t>(x);};return s;
    }
};
int main() {
    Fixture f;auto s=f.services();f.entry.locals[0]=7;
    assert(setEventFloat(f.entry,-30000000,1.5f,s)==7 && f.entry.locals[0]==-229998464);
    assert(f.calls==std::vector<int>({1,2,3,4,5}));
    assert(getEventFloat(f.entry,-30000000,s)==1.5f && getEventValue(f.entry,-30000000,s)==1);
    assert(setEventValue(f.entry,-30000000,-250000000,s)==-229998464);
    assert(getEventValue(f.entry,-250000000,s)==-250000000 && getEventValue(f.entry,-30000000,s)==-19531);
    f.entry.locals[0]=-270000000;assert(getEventValue(f.entry,-30000000,s)==-270000000 && getEventFloat(f.entry,-30000000,s)==-39062.5f);
    assert(setEventFloat(f.entry,-30000000,.5f,s)==-39062.5f);
    Fixture order;order.entry.locals[0]=7;order.mutateDuringConversion=true;
    assert(setEventFloat(order.entry,-30000000,1.5f,order.services())==7 && order.entry.locals[0]==-229998464);
    Fixture direct;assert(getEventValue(direct.entry,0x12345678,direct.services())==0x12345678 && direct.calls==std::vector<int>({1}));
    direct.calls.clear();assert(setEventValue(direct.entry,-270000000,123,direct.services())==123 && direct.calls==std::vector<int>({1}));
    assert(setEventValue(direct.entry,-250000000,-229998464,direct.services())==1);
    assert(getEventFloat(direct.entry,-250000000,direct.services())==-250000000.f);
    assert(eventShiftMask(-1)==0 && eventShiftMask(-32)==0 && eventShiftMask(32)==0 && eventShiftMask(64)==1);
    Fixture flags;flags.words[flags.context+0x84]=0x12345678;
    assert(getEventValue(flags.entry,-90000001,flags.services())==0);
    flags.calls.clear();flags.addresses.clear();assert(setEventValue(flags.entry,-90000033,7,flags.services())==7);
    assert(flags.addresses==std::vector<uint32_t>({flags.context+0x80,flags.context+0x80}));
    flags.words[flags.context+0x84]=0x80;
    assert(setEventValue(flags.entry,-90000000,7,flags.services())==7 && flags.words[flags.context+0x84]==0x81);
    assert(setEventFloat(flags.entry,-90000000,-0.f,flags.services())==0 && flags.words[flags.context+0x84]==0x80);
    const float nan=std::numeric_limits<float>::quiet_NaN();assert(std::isnan(setEventFloat(flags.entry,-90000000,nan,flags.services())) && flags.words[flags.context+0x84]==0x81);
    flags.words[flags.entry.userFlags]=1;
    const uint32_t floatUf=flags.entry.userWork+4u*uint32_t(-20000000);
    setEventFloat(flags.entry,-210000000,1.5f,flags.services());
    assert(flags.words[floatUf]==uint32_t(-229998464) && getEventFloat(flags.entry,-210000000,flags.services())==1.5f && getEventValue(flags.entry,-210000000,flags.services())==1);
    Fixture saved;saved.words[saved.gp+0x174]=0x12345678;
    assert(getEventValue(saved.entry,-170000000,saved.services())==0x12345678);
    assert(setEventValue(saved.entry,-170000000,-229998464,saved.services())==0x12345678);
    assert(getEventValue(saved.entry,-170000000,saved.services())==-229998464 && getEventFloat(saved.entry,-170000000,saved.services())==1.5f);
    setEventValue(saved.entry,-170000000+1,0x1234,saved.services());assert(saved.bytes[saved.gp+0x579]==0x34);
    setEventValue(saved.entry,-150000000,0x1234,saved.services());assert(saved.bytes[saved.gp+0xdb8]==0x34);
    saved.words[saved.gp+0x178]=0x80;assert(setEventValue(saved.entry,-130000000,123,saved.services())==0 && saved.words[saved.gp+0x178]==0x81);
    Fixture changed;changed.changeGp=true;changed.words[0x3000+0x174]=7;changed.words[0x4000+0x174]=9;
    assert(setEventValue(changed.entry,-170000000,123,changed.services())==7 && changed.words[0x3000+0x174]==7 && changed.words[0x4000+0x174]==123);
    Fixture gf;gf.words[gf.gp+0x178]=1;
    const uint32_t floatGsFlag=gf.context+0x84+4u*uint32_t(-40000000/32);gf.words[floatGsFlag]=0;
    assert(getEventValue(gf.entry,-130000000,gf.services())==1 && getEventFloat(gf.entry,-130000000,gf.services())==0);
    const uint32_t floatSaved=gf.context+0x84+4u*uint32_t(-80000000/32);
    const int priorGp=gf.gpReads;setEventFloat(gf.entry,-170000000,1.f,gf.services());
    assert(gf.words[floatSaved]==1 && gf.gpReads==priorGp);
    // Signed routing boundaries are not capacity clamps. Test providers supply
    // sparse word/byte memory for addresses outside conventional array indices.
    struct Boundary {int32_t operand;std::vector<int> calls;};
    for(const auto& test:std::vector<Boundary>{{-270000001,{1}},{-270000000,{1}},{-250000000,{1}},{-249999999,{1,4}},{-220000000,{1,4}},{-219999999,{1,3}},{-200000000,{1,3}},{-199999999,{1,3}},{-180000000,{1,3}},{-179999999,{1,6,7}},{-160000000,{1,6,7}},{-159999999,{1,6,7}},{-140000000,{1,6,7}},{-139999999,{1,6,3}},{-120000000,{1,6,3}},{-119999999,{1,6,3}},{-100000000,{1,6,3}},{-99999999,{1,3}},{-80000000,{1,3}},{-79999999,{1,2,3}},{-60000000,{1,2,3}},{-59999999,{1,3}},{-40000000,{1,3}},{-39999999,{1,2,3}},{-20000000,{1,2,3}},{-19999999,{1}}}) {
        Fixture boundary;getEventValue(boundary.entry,test.operand,boundary.services());assert(boundary.calls==test.calls);
    }
    // Copy-back/constructor locals use the very same semantic words as provider.
    EventEntry copy=f.entry;copy.locals[0]=42;assert(f.entry.locals[0]!=copy.locals[0]);
    std::cout<<"event operand tests passed\n";
}
