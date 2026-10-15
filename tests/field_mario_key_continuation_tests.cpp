#include "field_mario_key_continuation.hpp"
#include "field_key.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    bool live=true,partyLive=true,nullParty=false,recurse=false;
    int hold=-1,waits=0;uint32_t loseAt=0;uint8_t key=0,ctrl=0,kind=1;uint32_t flags=0;
    std::map<uint32_t,int32_t> atSite;
    std::map<uint32_t,int32_t> values{{0x80141050,1},{0x8002e210,2},{0x800a4224,0},{0x800a16dc,1},{0x800a68e8,1},{0x800c5744,0},{0x800c0068,0},{0x800b6e94,0},{0x800d0874,0}};
    std::vector<uint32_t> trace;std::shared_ptr<int> registry=std::make_shared<int>(1),actor=std::make_shared<int>(2);
    FieldMarioKeyServices s;FieldMarioKeyCursor c;
    Fixture(){
        s.user=this;s.current=[](void*p){return static_cast<Fixture*>(p)->live;};
        s.partyCurrent=[](void*p,const FieldMarioKeyPartyCapture&v){auto&f=*static_cast<Fixture*>(p);return f.partyLive&&v.registry.get()==f.registry.get()&&(!v.actor||v.incarnation.get()==f.actor.get());};
        s.query=[](void*p,const FieldMarioKeyRequest&r)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(f.recurse){f.recurse=false;assert(!resumeOriginalMarioChkKey(f.s,f.c));assert(!f.c.reset());}if(f.hold==int(r.callsite)&&f.waits++<2)return {};f.trace.push_back(r.callsite);if(f.loseAt==r.callsite)f.live=false;return f.atSite.count(r.callsite)?f.atSite.at(r.callsite):f.values.at(r.function);};
        s.party=[](void*p,int32_t id)->std::optional<FieldMarioKeyPartyCapture>{auto&f=*static_cast<Fixture*>(p);assert(id==1);if(f.hold==1&&f.waits++<2)return {};f.trace.push_back(0x8005c490);return FieldMarioKeyPartyCapture{f.nullParty?nullptr:f.actor.get(),f.registry,f.nullParty?nullptr:f.actor};};
        s.marioByte=[](void*p,uint32_t o)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(o==0x38)return f.ctrl;assert(o==0x39);if(f.hold==3&&f.waits++<2)return {};f.trace.push_back(0x8005c4ac);return f.key;};
        s.partyWord=[](void*p,const FieldMarioKeyPartyCapture&,uint32_t o)->std::optional<uint32_t>{assert(o==0);auto&f=*static_cast<Fixture*>(p);if(f.hold==12&&f.waits++<2)return {};f.trace.push_back(0x8005c558);return f.flags;};
        s.partyByte=[](void*p,const FieldMarioKeyPartyCapture&,uint32_t o)->std::optional<uint8_t>{assert(o==0x31);auto&f=*static_cast<Fixture*>(p);if(f.hold==13&&f.waits++<2)return {};f.trace.push_back(0x8005c564);return f.kind;};
    }
    int run(){for(int i=0;i<8;++i)if(auto r=resumeOriginalMarioChkKey(s,c))return *r;assert(false);return -1;}
};
int main(){
    // Differential result AND successful source boundary trace against the
    // existing synchronous source predicate, across all gate combinations.
    for(unsigned mask=0;mask<1024;++mask){
        Fixture f;f.nullParty=mask&1;f.key=(mask&2)?128:0;f.flags=(mask&4)?0x100:0;f.kind=(mask&8)?6:1;
        f.values[0x8002e210]=(mask&16)?4:2;f.values[0x800a4224]=(mask&32)?2:0;
        f.values[0x800a16dc]=(mask&64)?0:1;f.values[0x800c5744]=(mask&128)?1:0;
        f.values[0x800b6e94]=(mask&256)?1:0;f.values[0x800d0874]=(mask&512)?1:0;
        std::vector<uint32_t> ref;FieldKeyPartner partner{f.flags,int8_t(f.kind)};FieldKeyServices q;
        auto v=[&](uint32_t fn,uint32_t site){ref.push_back(site);return f.values.at(fn);};
        q.partyId=[&](){return v(0x80141050,0x8005c48c);};q.party=[&](int){ref.push_back(0x8005c490);return f.nullParty?nullptr:&partner;};
        q.sequence=[&](){return v(0x8002e210,0x8005c498);};q.inputByte=[&](){ref.push_back(0x8005c4ac);return int(f.key);};
        unsigned ships=0;q.shipChanging=[&](){return v(0x800a4224,ships++?0x8005c4d0:0x8005c4c4);};
        q.rollEnabled=[&](){return v(0x800a16dc,0x8005c4e4);};q.slitEnabled=[&](){return v(0x800a68e8,0x8005c4f8);};
        q.christineStatus=[&](){return v(0x800c5744,0x8005c51c);};q.bomheiKeyCheck=[&](){return v(0x800c0068,0x8005c530);};
        q.yoshiStatus=[&](){return v(0x800b6e94,0x8005c544);};q.holdItem=[&](){return v(0x800d0874,0x8005c58c);};
        const int wanted=fieldKeyAdmitted(q);assert(f.run()==wanted);
        // The synchronous typed reference reads partner fields directly.
        std::vector<uint32_t> calls;for(auto site:f.trace)if(site!=0x8005c558&&site!=0x8005c564)calls.push_back(site);
        assert(calls==ref);
    }
    Fixture base;assert(base.run()==1);const auto expected=base.trace;
    for(const auto site:expected){Fixture f;f.hold=site==0x8005c490?1:site==0x8005c4ac?3:site==0x8005c558?12:int(site);assert(f.run()==1);assert(f.trace==expected);assert(f.waits==3);assert(f.run()==1);assert(f.trace==expected);}
    for(unsigned b=0;b<256;++b){Fixture f;f.ctrl=uint8_t(b);assert(originalMarioChkCtrl(f.s)==int(b==0));}
    for(const auto fn:{0x8002e210u,0x800a4224u,0x800a16dcu,0x800a68e8u,0x800c5744u,0x800c0068u,0x800b6e94u,0x800d0874u}){Fixture f;f.values[fn]=(fn==0x800a16dc||fn==0x800a68e8)?0:1;assert(f.run()==0);}
    Fixture changingShip;changingShip.atSite[0x8005c4c4]=2;changingShip.atSite[0x8005c4d0]=0;assert(changingShip.run()==1);
    Fixture secondShip;secondShip.values[0x800a4224]=2;assert(secondShip.run()==0);assert(secondShip.trace.back()==0x8005c4d0);
    Fixture null;null.nullParty=true;null.values[0x800c5744]=1;assert(null.run()==1);assert(null.trace.back()==0x8005c4f8);
    for(unsigned k=0;k<256;++k){Fixture f;f.flags=0x100;f.kind=uint8_t(k);assert(f.run()==int(k!=5&&k!=6&&k!=7));}
    Fixture late;late.hold=0x8005c544;assert(!resumeOriginalMarioChkKey(late.s,late.c));late.flags=0x100;late.kind=5;assert(late.run()==0);assert(late.trace.back()==0x8005c564);
    Fixture byteHold;byteHold.flags=0x100;byteHold.hold=13;assert(!resumeOriginalMarioChkKey(byteHold.s,byteHold.c));byteHold.flags=0;byteHold.kind=6;assert(byteHold.run()==0);assert(byteHold.trace.back()==0x8005c564);
    Fixture lost;lost.hold=0x8005c544;assert(!resumeOriginalMarioChkKey(lost.s,lost.c));auto trace=lost.trace;const auto oldActor=lost.actor;lost.actor=std::make_shared<int>(3);assert(!resumeOriginalMarioChkKey(lost.s,lost.c));assert(lost.trace==trace);lost.actor=oldActor;
    assert(!resumeOriginalMarioChkKey(lost.s,lost.c));assert(lost.trace==trace);
    Fixture swap;swap.hold=0x8005c544;assert(!resumeOriginalMarioChkKey(swap.s,swap.c));auto other=swap.s;other.partyByte=nullptr;assert(!resumeOriginalMarioChkKey(other,swap.c));swap.recurse=true;assert(swap.run()==1);
    Fixture during;during.loseAt=0x8005c544;assert(!resumeOriginalMarioChkKey(during.s,during.c));auto before=during.trace;during.live=true;assert(!resumeOriginalMarioChkKey(during.s,during.c));assert(during.trace==before);assert(during.c.reset());during.loseAt=0;assert(during.run()==1);
    Fixture noWord;noWord.s.partyWord=nullptr;assert(!resumeOriginalMarioChkKey(noWord.s,noWord.c));assert(noWord.c.pending()==0x8005c558);
    Fixture noByte;noByte.flags=0x100;noByte.s.partyByte=nullptr;assert(!resumeOriginalMarioChkKey(noByte.s,noByte.c));assert(noByte.c.pending()==0x8005c564);
    Fixture missing;missing.s.query=nullptr;assert(!resumeOriginalMarioChkKey(missing.s,missing.c));assert(missing.trace.empty());
    return 0;
}
