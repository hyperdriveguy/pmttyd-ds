#include "event_if_opcodes.hpp"
#include "event_data_opcodes.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
uint32_t command(int n,int op) {return uint32_t(uint16_t(n))<<16|uint32_t(op);}
struct Access {char type;uint32_t address;bool operator==(const Access& a) const {return type==a.type && address==a.address;}};
struct Fixture {
    EventEntry e;
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::map<int32_t,float> floats;
    std::vector<Access> accesses;
    std::vector<int32_t> queries;
    std::function<void(int32_t)> onQuery;
    std::function<void(uint32_t)> onByte;
    Fixture() {e.arguments=0x1000;e.nextCommand=0x2000;words[0x1000]=1;words[0x1004]=2;words[0x2000]=command(0,32);words[0x2004]=command(0,33);}
    void string(uint32_t a,const std::string& text) {for(size_t i=0;i<text.size();++i)bytes[a+uint32_t(i)]=uint8_t(text[i]);for(unsigned i=0;i<8;++i)bytes[a+uint32_t(text.size())+i]=0;}
    EventIfOpcodeServices services() {
        EventIfOpcodeServices s;s.values.user=this;
        s.values.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.accesses.push_back({'w',a});if(f.words.count(a))return f.words.at(a);uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|f.bytes.at(a+i);return v;};
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.accesses.push_back({'b',a});if(f.onByte)f.onByte(a);return f.bytes.at(a);};
        s.values.resolve=[](void* p,EventEntry& e,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return op>=-30000000 && op<-29999984?e.locals[op+30000000]:op;};
        s.values.resolveFloat=[](void* p,EventEntry&,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return f.floats.at(op);};
        s.values.store=[](void*,EventEntry& e,int32_t d,int32_t v) {assert(d>=-30000000 && d<-29999984);e.locals[d+30000000]=v;};
        return s;
    }
    void run(uint8_t op) {assert(eventIfOpcode(e,op,services())==2);}
};
bool relation(unsigned op,int32_t l,int32_t r) {switch(op) {case 0:return l==r;case 1:return l!=r;case 2:return l<r;case 3:return l>r;case 4:return l<=r;default:return l>=r;}}
}
int main() {
    Fixture unsupported;assert(!eventIfOpcode(unsupported.e,11,unsupported.services()));assert(!eventIfOpcode(unsupported.e,34,unsupported.services()));assert(unsupported.accesses.empty());
    for(uint8_t op=24;op<=29;++op)for(auto pair:std::vector<std::pair<int32_t,int32_t>>{{-7,2},{2,-7},{2,2},{std::numeric_limits<int32_t>::min(),std::numeric_limits<int32_t>::max()}}) {
        Fixture f;f.words[0x1000]=uint32_t(pair.first);f.words[0x1004]=uint32_t(pair.second);f.run(op);
        assert(f.e.nextCommand==(relation(op-24,pair.first,pair.second)?0x2000u:0x2004u));
        assert(f.queries==std::vector<int32_t>({pair.first,pair.second}));
    }
    Fixture retained;retained.words[0x1000]=uint32_t(-30000000);retained.words[0x1004]=99;retained.e.locals[0]=7;
    retained.onQuery=[&](int32_t op) {if(op==-30000000) {retained.words[0x1004]=7;retained.e.arguments=0x9999;}else retained.e.locals[0]=88;};
    retained.run(24);assert(retained.e.nextCommand==0x2000 && retained.queries==std::vector<int32_t>({-30000000,7}) && retained.e.locals[0]==88);
    Fixture liveNext;liveNext.onQuery=[&](int32_t op) {if(op==2)liveNext.e.nextCommand=0x3000;};liveNext.words[0x3000]=command(0,33);liveNext.run(24);assert(liveNext.e.nextCommand==0x3004);
    for(uint8_t op:{uint8_t(30),uint8_t(31)})for(auto pair:std::vector<std::pair<uint32_t,uint32_t>>{{0x80000000,0x80000000},{2,3},{7,0},{1,uint32_t(-30000000)}}) {
        Fixture f;f.words[0x1000]=pair.first;f.words[0x1004]=pair.second;f.run(op);
        const bool match=op==30?(pair.first&pair.second)!=0:(pair.first&pair.second)==0;
        assert(f.e.nextCommand==(match?0x2000u:0x2004u) && f.queries.size()==1);
    }
    Fixture mask;mask.onQuery=[&](int32_t) {mask.words[0x1004]=1;mask.e.arguments=0x9999;};mask.run(30);assert(mask.e.nextCommand==0x2000 && mask.queries.size()==1);
    const float nan=std::numeric_limits<float>::quiet_NaN(),inf=std::numeric_limits<float>::infinity();
    for(uint8_t op=18;op<=23;++op)for(auto pair:std::vector<std::pair<float,float>>{{1,2},{2,1},{1,1},{0,-0.f},{inf,-inf},{nan,1},{1,nan}}) {
        Fixture f;f.floats[1]=pair.first;f.floats[2]=pair.second;f.run(op);
        const bool unordered=pair.first!=pair.first || pair.second!=pair.second;
        const bool match=unordered?op!=18:relation(op-18,pair.first<pair.second?-1:pair.first>pair.second?1:0,0);
        assert(f.e.nextCommand==(match?0x2000u:0x2004u) && f.queries==std::vector<int32_t>({1,2}));
    }
    Fixture floatOrder;floatOrder.floats[1]=1;floatOrder.floats[3]=1;
    floatOrder.onQuery=[&](int32_t op) {if(op==1) {floatOrder.words[0x1004]=3;floatOrder.e.arguments=0x9999;}else floatOrder.floats[1]=99;};
    floatOrder.run(18);assert(floatOrder.e.nextCommand==0x2000 && floatOrder.queries==std::vector<int32_t>({1,3}));
    // A false IF consumes its same-level ELSE/END. Nested markers and DELETE
    // do not terminate it; lexical opcodes/counts retain16 bits.
    Fixture nested;nested.words[0x2000]=command(0,24);nested.words[0x2004]=command(0,32);nested.words[0x2008]=command(0,2);nested.words[0x200c]=command(0,33);nested.words[0x2010]=command(0,0x120);nested.words[0x2014]=command(257,32);
    nested.run(24);assert(nested.e.nextCommand==0x241c);
    Fixture noElse;noElse.words[0x2000]=command(-2,53);noElse.words[0x1ffc]=command(0,1);noElse.run(24);assert(noElse.e.nextCommand==0x2000);
    Fixture elseScan;elseScan.words[0x2000]=command(0,32);elseScan.words[0x2004]=command(0,24);elseScan.words[0x2008]=command(0,32);elseScan.words[0x200c]=command(0,33);elseScan.words[0x2010]=command(0,33);elseScan.run(32);assert(elseScan.e.nextCommand==0x2014 && elseScan.queries.empty());
    Fixture end;end.run(33);assert(end.e.nextCommand==0x2000 && end.accesses.empty() && end.queries.empty());
    // All alignment pairs exercise bytewise content sign, unsigned bytes and
    // padded readable memory for the original word-load extent.
    for(unsigned la=0;la<4;++la)for(unsigned ra=0;ra<4;++ra)for(auto pair:std::vector<std::pair<std::string,std::string>>{{"",""},{"same","same"},{"prefix","prefixes"},{"abcx","abcy"},{"Case","case"},{std::string(1,char(0xff)),std::string(1,char(0x80))}}) {
        Fixture f;f.string(0x4000+la,pair.first);f.string(0x5000+ra,pair.second);
        const int32_t result=eventCompareStrings(0x4000+la,0x5000+ra,f.services());
        const int expected=pair.first.compare(pair.second);assert((result>0)-(result<0)==(expected>0)-(expected<0));
    }
    Fixture wordMismatch;wordMismatch.string(0x4000,"abcx");wordMismatch.string(0x5000,"abcz");assert(eventCompareStrings(0x4000,0x5000,wordMismatch.services())==-1);
    assert(wordMismatch.accesses==std::vector<Access>({{'b',0x4000},{'b',0x5000},{'w',0x4000},{'w',0x5000}}));
    Fixture byteMismatch;byteMismatch.string(0x4000,"x");byteMismatch.string(0x5000,"z");assert(eventCompareStrings(0x4000,0x5000,byteMismatch.services())==-2);
    Fixture empty;empty.string(0x4000,"");empty.string(0x5000,"");assert(eventCompareStrings(0x4000,0x5000,empty.services())==0);
    assert(empty.accesses==std::vector<Access>({{'b',0x4000},{'b',0x5000},{'w',0x4000},{'w',0x5000},{'b',0x4000},{'b',0x5000}}));
    for(uint8_t op=12;op<=17;++op) {
        Fixture f;f.string(0x4000,"lower");f.string(0x5000,"upper");f.words[0x1000]=0x4000;f.words[0x1004]=0x5000;f.run(op);assert(f.e.nextCommand==(relation(op-12,-1,0)?0x2000u:0x2004u));
    }
    Fixture nulls;nulls.words[0x1000]=0;nulls.words[0x1004]=0;nulls.string(0x8041fa2c,"");nulls.run(12);assert(nulls.e.nextCommand==0x2000);
    nulls.e.nextCommand=0x2000;nulls.string(0x5000,"nonempty");nulls.words[0x1004]=0x5000;nulls.run(14);assert(nulls.e.nextCommand==0x2000);
    Fixture stringOrder;stringOrder.string(0x4000,"same");stringOrder.string(0x5000,"same");stringOrder.words[0x1000]=0x4000;
    stringOrder.onQuery=[&](int32_t op) {if(op==0x4000) {stringOrder.words[0x1004]=0x5000;stringOrder.e.arguments=0x9999;}};stringOrder.run(12);assert(stringOrder.queries==std::vector<int32_t>({0x4000,0x5000}) && stringOrder.e.nextCommand==0x2000);
    Fixture stringLiveNext;stringLiveNext.string(0x4000,"a");stringLiveNext.string(0x5000,"b");stringLiveNext.words[0x1000]=0x4000;stringLiveNext.words[0x1004]=0x5000;stringLiveNext.words[0x3000]=command(0,33);
    stringLiveNext.onByte=[&](uint32_t) {stringLiveNext.e.nextCommand=0x3000;};stringLiveNext.run(12);assert(stringLiveNext.e.nextCommand==0x3004);
    // False and accepted branches converge and yield only at WAIT_FRM,using
    // the actual dispatcher and assignment handler.
    for(int test:{0,1}) {
        Fixture program;uint32_t a=0x6000;
        auto emit=[&](int op,std::initializer_list<int32_t> args) {program.words[a]=command(args.size(),op);a+=4;for(int32_t v:args) {program.words[a]=uint32_t(v);a+=4;}};
        emit(24,{test,1});emit(50,{-30000000,7});emit(32,{});emit(50,{-30000000,9});emit(33,{});emit(9,{1});program.e.nextCommand=0x6000;
        auto dispatcher=program.services().values;dispatcher.handler=[](void* p,EventEntry& e,uint8_t op) {auto& f=*static_cast<Fixture*>(p);if(auto r=eventIfOpcode(e,op,f.services()))return *r;EventDataOpcodeServices s;s.values=f.services().values;auto r=eventDataOpcode(e,op,s);assert(r);return *r;};
        assert(dispatchEvent(program.e,dispatcher)==0 && program.e.locals[0]==(test?7:9) && program.e.opcode==0);
    }
    std::cout<<"event IF opcode tests passed\n";
}
