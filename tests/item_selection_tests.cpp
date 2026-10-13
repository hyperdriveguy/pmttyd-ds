#include "item_selection.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <tuple>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t Entry=0x81000000,Context=0x82000000,Rows=0x82000100,Args=0x81001000;
    EventEntry event;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint16_t> halves;
    std::vector<std::string> trace;int32_t result=0;bool changeDestinations=false;
    EventMemoryServices memory(){EventMemoryServices m;m.user=this;
        m.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("w"+std::to_string(a));if(a==Entry+0x78)return uint32_t(f.event.waitCount);assert(f.words.count(a));return f.words.at(a);};
        m.half=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("h"+std::to_string(a));assert(f.halves.count(a));return f.halves.at(a);};
        m.storeWord=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);if(a==Entry+0x78){f.trace.push_back("retain");f.event.waitCount=v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}else{f.trace.push_back("s"+std::to_string(a));f.words[a]=v;}};
        m.storeHalf=[](void* p,uint32_t a,uint16_t value){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("t"+std::to_string(a));f.halves[a]=value;};return m;}
    ItemSelectionEventServices service(){ItemSelectionEventServices s;s.user=this;s.memory=memory();s.operands.user=this;
        s.operands.word=memory().word;
        s.operands.resolve=[](void* p,EventEntry&,int32_t raw){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("resolve"+std::to_string(raw));return raw;};
        s.operands.store=[](void* p,EventEntry& e,int32_t dest,int32_t value){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("store"+std::to_string(dest));assert(dest>=0 && dest<16);e.locals[unsigned(dest)]=value;};
        s.create=[](void* p,uint32_t table,int32_t second,int32_t third){auto& f=*static_cast<Fixture*>(p);assert(table==Rows && second==0 && third==1);f.trace.push_back("create");f.event.arguments=0x81002000;return Context;};
        s.keyOff=[](void* p){static_cast<Fixture*>(p)->trace.push_back("off");};
        s.poll=[](void* p,uint32_t context){auto& f=*static_cast<Fixture*>(p);assert(context==Context);f.trace.push_back("poll");return f.result;};
        s.selectedIndex=[](void* p,uint32_t context){auto& f=*static_cast<Fixture*>(p);assert(context==Context);f.trace.push_back("index");return 3;};
        s.remove=[](void* p,uint32_t context){auto& f=*static_cast<Fixture*>(p);assert(context==Context);f.trace.push_back("delete");if(f.changeDestinations){f.words[Args+8]=7;f.words[Args+12]=8;}};
        s.keyOn=[](void* p){static_cast<Fixture*>(p)->trace.push_back("on");};return s;}
};
struct Windows {
    uint32_t flags[3]{1,1,1};std::vector<uint32_t> sounds;std::vector<std::string> trace;
    ItemSelectionMainServices service(){return {this,
        [](void* p,uint32_t i){return static_cast<Windows*>(p)->flags[i];},
        [](void* p,uint32_t i,int32_t a){static_cast<Windows*>(p)->trace.push_back(std::to_string(i)+"action"+std::to_string(a));},
        [](void* p,uint32_t i){static_cast<Windows*>(p)->trace.push_back(std::to_string(i)+"reset");},
        [](void* p,uint32_t i){auto& w=*static_cast<Windows*>(p);w.flags[i]&=~1u;w.trace.push_back(std::to_string(i)+"off");},
        [](void* p,uint32_t id){static_cast<Windows*>(p)->sounds.push_back(id);},
        [](void* p,bool open){static_cast<Windows*>(p)->trace.push_back(open?"statusOn":"statusOff");}};}
};
int main(){
    Fixture f;auto m=f.memory();const auto c=Fixture::Context,r=Fixture::Rows;
    constexpr uint32_t table=0x82000200;
    f.words[table]=95;f.words[table+4]=95;f.words[table+8]=0x1234006b;f.words[table+12]=0xffffffff;
    ItemSelectionRowsServices build{m,&f,
        [](void* p,int32_t heap,uint32_t bytes){auto& f=*static_cast<Fixture*>(p);assert(heap==0 && bytes==20);f.trace.push_back("allocate");return Fixture::Rows;},
        [](void* p,uint32_t at,uint32_t bytes){auto& f=*static_cast<Fixture*>(p);assert(at==Fixture::Rows && bytes==20 && f.words[Fixture::Context+0x30]==at);f.trace.push_back("clear");for(unsigned i=0;i<bytes;i+=2)f.halves[at+i]=0;}};
    fillPackedExplicitItemRows(c,table,0,build);
    assert(f.words[c+0x34]==3 && f.halves[r]==0 && f.halves[r+2]==95 && f.halves[r+6]==95 && f.halves[r+10]==107);
    assert(f.trace[4]=="allocate" && f.trace[5]=="s"+std::to_string(c+0x30) && f.trace[6]=="w"+std::to_string(c+0x30) && f.trace[7]=="clear");
    assert(std::count(f.trace.begin(),f.trace.end(),"w"+std::to_string(table))==3 && std::count(f.trace.begin(),f.trace.end(),"w"+std::to_string(table+12))==2);
    const auto firstItem=std::find(f.trace.begin(),f.trace.end(),"t"+std::to_string(r+2));assert(firstItem!=f.trace.begin() && *(firstItem-1)=="s"+std::to_string(c+0x34));
    f.trace.clear();fillPackedExplicitItemRows(c,table,0x12340001,build);
    assert(f.words[c+0x34]==4 && f.halves[r+2]==1 && f.halves[r+6]==95 && f.halves[r+10]==95 && f.halves[r+14]==107);
    // Allocation can mutate table values; fill must reread original storage.
    auto mutate=build;mutate.allocate=[](void* p,int32_t heap,uint32_t bytes){auto& f=*static_cast<Fixture*>(p);assert(heap==0 && bytes==20);f.words[0x82000200]=107;return Fixture::Rows;};
    fillPackedExplicitItemRows(c,table,0,mutate);assert(f.halves[r+2]==107 && f.words[c+0x34]==3);
    f.trace.clear();
    f.halves[c]=0;assert(packedItemSelectionResult(c,m)==0 && f.trace.size()==1);f.trace.clear();
    f.halves[c]=0x3000;assert(packedItemSelectionResult(c,m)==-1 && f.trace.size()==1);f.trace.clear();
    f.halves[c]=0x1100;f.words[c+0x38]=1;f.words[c+0xc]=0;
    assert(packedItemSelectionResult(c,m)==-1 && f.trace.size()==3);f.trace.clear();
    f.words[c+0xc]=2;f.words[c+0x30]=r;f.halves[r+10]=65535;
    assert(packedItemSelectionResult(c,m)==65535);
    assert((f.trace==std::vector<std::string>{"h"+std::to_string(c),"w"+std::to_string(c+0x38),"w"+std::to_string(c+0xc),"w"+std::to_string(c+0xc),"w"+std::to_string(c+0x30),"h"+std::to_string(r+10)}));
    f.trace.clear();f.words[c+0x38]=0;assert(packedItemSelectionResult(c,m)==65535 && f.trace.size()==5);
    f.words[Fixture::Args]=0;f.words[Fixture::Args+4]=r;f.words[Fixture::Args+8]=5;f.words[Fixture::Args+12]=6;f.event.arguments=Fixture::Args;f.trace.clear();
    auto services=f.service();assert(itemSelectionEvent(f.event,Fixture::Entry,true,services)==0);
    assert(uint32_t(f.event.waitCount)==c && f.trace.size()==9);
    assert(f.trace[4]=="create" && f.trace[5]=="retain" && f.trace[6]=="off" && f.trace[7].rfind("w",0)==0); // Source reload then poll.
    assert(f.trace[8]=="poll");f.trace.clear();f.event.arguments=Fixture::Args;f.result=95;f.changeDestinations=true;
    assert(itemSelectionEvent(f.event,Fixture::Entry,false,services)==2 && f.event.locals[7]==95 && f.event.locals[8]==0);
    const std::vector<std::string> ending{"index","delete","on","w"+std::to_string(Fixture::Args+8),"store7"};
    assert(std::vector<std::string>(f.trace.end()-5,f.trace.end())==ending);
    f.event.arguments=Fixture::Args;f.words[Fixture::Args]=1;f.result=-1;f.trace.clear();
    assert(itemSelectionEvent(f.event,Fixture::Entry,true,services)==2 && f.event.locals[7]==-1 && f.event.locals[8]==3);
    assert(uint32_t(f.event.waitCount)==c && f.trace.back()=="store8"); // No clear-after-free.
    struct Sequence {unsigned reads=0;std::vector<std::pair<int32_t,int32_t>> calls;ItemSelectionWindow* window=nullptr;bool mutate=false;} sequence;
    ItemSelectionSequenceServices seq{&sequence,
        [](void* p){auto& s=*static_cast<Sequence*>(p);++s.reads;return s.mutate?int32_t(s.reads*60):60;},
        [](void* p,int32_t mode,int32_t step,int32_t count,float a,float b){auto& s=*static_cast<Sequence*>(p);assert(mode==12);s.calls.push_back({step,count});if(s.mutate)s.window->clock=s.calls.size()==1?2:5;return *cameraInterpolation(mode,step,count,a,b);}};
    ItemSelectionWindow animated;animated.flags=1;animated.action=1;
    for(unsigned i=0;i<9;++i)tickItemSelectionSequence(animated,seq);
    assert(animated.action==0 && animated.clock==9 && animated.flags==7 && animated.scale==1 && animated.alpha==255 && sequence.reads==27);
    tickItemSelectionSequence(animated,seq);assert(animated.flags==3 && sequence.reads==27); // Open stays visible; idle clears transition only.
    animated.action=2;animated.clock=0;animated.flags|=8;
    for(unsigned i=0;i<9;++i)tickItemSelectionSequence(animated,seq);
    assert(animated.action==0 && !(animated.flags&3) && (animated.flags&4));tickItemSelectionSequence(animated,seq);assert(animated.flags==8);
    ItemSelectionWindow header;header.sequence=2;header.flags=1;header.action=1;const auto before=sequence.reads;
    tickItemSelectionSequence(header,{});assert(header.flags==7 && header.action==0 && header.scale==1 && header.rotation==0);
    tickItemSelectionSequence(header,{});assert(header.flags==3);header.action=2;tickItemSelectionSequence(header,{});assert(header.flags==5 && sequence.reads==before);
    ItemSelectionWindow effects;effects.flags=1;effects.action=1;sequence={0,{},&effects,true};tickItemSelectionSequence(effects,seq);
    assert((sequence.calls==std::vector<std::pair<int32_t,int32_t>>({{0,8},{2,16}}) && sequence.reads==3 && effects.clock==6 && effects.action==1));
    ItemSelectionMain selection;selection.windows={0,1,2};
    for(unsigned i=0;i<12;++i)selection.rows.push_back({0,uint16_t(i%2?107:95)});
    Windows w;auto main=w.service();w.flags[1]=0;
    tickItemSelectionMain(selection,{},main);
    assert(selection.state==1 && w.sounds==std::vector<uint32_t>{0x2002a});
    assert((w.trace==std::vector<std::string>{"0action1","0reset","2action1","2reset","2reset","statusOn"}));
    w.flags[0]=5;tickItemSelectionMain(selection,{0x2000,0x2000,0x100},main);assert(selection.selected==0 && selection.state==1 && selection.cursorY==62);
    w.flags[0]=1;
    // New presses wrap; repeated endpoint presses clamp. Both directions cancel.
    tickItemSelectionMain(selection,{0x1000,0,0},main);assert(selection.selected==0);
    tickItemSelectionMain(selection,{0x1000,0x1000,0},main);assert(selection.selected==11 && selection.firstVisible==4);
    tickItemSelectionMain(selection,{0x2000,0,0},main);assert(selection.selected==11);
    tickItemSelectionMain(selection,{0x2000,0x2000,0},main);assert(selection.selected==0 && selection.firstVisible==0);
    tickItemSelectionMain(selection,{0x3000,0x3000,0},main);assert(selection.selected==0);
    for(unsigned i=0;i<7;++i)tickItemSelectionMain(selection,{0x2000,0,0},main);
    assert(selection.selected==7 && selection.firstVisible==2);
    selection.rows[7].flags=2;tickItemSelectionMain(selection,{0,0,0x300},main);
    assert(selection.state==1 && !(selection.flags&0x2000) && w.sounds.back()==0x20014); // Disabled A does not fall through B.
    selection.rows[7].flags=0;tickItemSelectionMain(selection,{0,0,0x300},main);
    assert(selection.state==2 && !(selection.flags&0x2000) && w.sounds.back()==0x20012); // A precedes B.
    tickItemSelectionMain(selection,{},main);assert(selection.state==3 && w.sounds.back()==0x2002b && w.trace.back()=="statusOff");
    w.flags[0]=5;tickItemSelectionMain(selection,{},main);assert(selection.state==3 && !(selection.flags&0x1000));
    w.flags[0]=1;tickItemSelectionMain(selection,{},main);assert(selection.state==4 && (selection.flags&0x1000) && !w.flags[0] && !w.flags[2]);
    const auto sounds=w.sounds;tickItemSelectionMain(selection,{0x2000,0x2000,0x100},main);assert(w.sounds==sounds);
    selection.state=1;selection.flags=0;w.flags[0]=1;tickItemSelectionMain(selection,{0,0,0x200},main);assert(selection.state==1);
    selection.flags=0x100;tickItemSelectionMain(selection,{0,0,0x200},main);assert(selection.state==2 && (selection.flags&0x2000) && w.sounds.back()==0x20013);
    ItemSelectionMain easing=selection;easing.state=1;easing.selected=7;easing.firstVisible=2;
    easing.displacement=5.3f;easing.cursorX=4.7f;easing.cursorY=-19.2f;
    tickItemSelectionMain(easing,{},main);
    auto bits=[](float value){uint32_t raw;std::memcpy(&raw,&value,4);return raw;};
    // Independent rational/IEEE single-rounding vector, not a runtime measurement.
    assert(bits(easing.displacement)==0x4146aaab && bits(easing.cursorX)==0xbdaaaac0 && bits(easing.cursorY)==0xc1cd5556);
    std::cout<<"Original item-window packed result/read order, event handle/key ownership and list lifecycle/control tests passed\n";
}
