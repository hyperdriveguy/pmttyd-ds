#include "field_door_parameter.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct State {std::map<uint32_t,uint32_t> memory;std::vector<uint32_t> reads;std::vector<int32_t> values;std::vector<std::pair<int32_t,int32_t>> stores;};
int main(){
 State state;EventEntry e;e.arguments=0x100;
 FieldDoorParameterServices s;s.user=&state;
 s.word=[](void* p,uint32_t a){auto& s=*static_cast<State*>(p);s.reads.push_back(a);return s.memory.at(a);};
 s.value=[](void* p,EventEntry& e,int32_t raw){auto& s=*static_cast<State*>(p);s.values.push_back(raw);e.arguments=0x999;return raw;};
 s.setValue=[](void* p,EventEntry&,int32_t raw,int32_t v){auto& s=*static_cast<State*>(p);s.stores.push_back({raw,v});if(s.stores.size()==1){s.memory[0x10c]=77;s.memory[0x410]=123;}};
 const uint32_t offsets[]={4,12,24,32,0,0,40,64,8,0};
 const unsigned counts[]={1,3,2,2,0,0,3,2,1,1};
 for(unsigned selector=0;selector<10;++selector){if(!counts[selector])continue;
  state={};e.arguments=0x100;state.memory[0x100]=0x400;state.memory[0x104]=selector;
  for(unsigned n=0;n<3;++n){state.memory[0x108+4*n]=10+n;state.memory[0x400+offsets[selector]+4*n]=100+n;}
  assert(fieldDoorParameterEvent(e,s)==2);assert((state.values==std::vector<int32_t>{0x400,int32_t(selector)}));
  assert(state.stores.size()==counts[selector] && state.stores[0].first==10);
  std::vector<uint32_t> expected{0x100,0x104};for(unsigned n=0;n<counts[selector];++n){expected.push_back(0x108+4*n);expected.push_back(0x400+offsets[selector]+4*n);}assert(state.reads==expected);
  if(selector==1)assert(state.stores[1]==std::make_pair(77,123));
 }
 for(uint32_t selector:{4u,5u,10u,11u,12u,0xffffffffu}){state={};e.arguments=0x100;state.memory[0x100]=0;state.memory[0x104]=selector;
  assert(fieldDoorParameterEvent(e,s)==(selector>11?2:0));assert(state.stores.empty() && state.reads.size()==2);}
}
