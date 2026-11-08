#include "stage_map_objects.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Hidden=1,Secondary=2;
using Point=StageMapObjects::Point;
uint32_t visible(const StageMapObjects& map){uint32_t n=0;map.visibleRanges(0,map.vertices(),[&](uint32_t,uint32_t count){n+=count;});return n;}
void compare(StageMapObjects& map,const char* path){
 FILE* file=std::fopen(path,"rb");assert(file);uint32_t checked=0;
 map.apply([&](uint32_t index,const Point& point){float expected[3];assert(std::fseek(file,long(index)*36,SEEK_SET)==0 && std::fread(expected,4,3,file)==3);
  for(unsigned axis=0;axis<3;++axis)assert(std::fabs(point[axis]-expected[axis])<0.00003f);
  ++checked;
 });assert(checked==map.vertices());std::fclose(file);
}
}
int main(){
 StageMapObjects foreground,base;
 assert(foreground.load("romfs/harbor-tutorial-stg_00_0.mapobj") && foreground.vertices()==7995);
 assert(base.load("romfs/harbor-tutorial-bti_01.mapobj") && base.vertices()==11829);
 assert(std::strcmp(foreground.root(),"world_root")==0);
 for(const char* name:{"A1","A2","B","C"})assert(foreground.flags(name) && foreground.world(name));
 assert(base.flags("escape_wall") && base.flags("spotlight"));
 // Actual source-selected offsets. Existing owner contributes its documented
 // source-to-native translation and root scale; do not pre-scale these again.
 assert(foreground.translate("A1",{33,0,-48}) && foreground.translate("A2",{0,0,71}));
 assert(foreground.translate("B",{0,0,0}) && foreground.translate("C",{0,0,0}));
 foreground.recalculate();compare(foreground,"romfs/harbor-tutorial-stg_00_0.vertices");
 compare(base,"romfs/harbor-tutorial-bti_01.vertices");
 assert(visible(foreground)==foreground.vertices());uint32_t covered=0;
 for(const char* name:{"A1","A2","B","C"}){
  assert(foreground.flag(name,Hidden,true));const auto hidden=foreground.vertices()-visible(foreground);assert(hidden);covered+=hidden;
  assert(foreground.flag(name,Hidden,false));assert(visible(foreground)==foreground.vertices());
 }
 assert(covered==foreground.vertices());
 assert(foreground.flag(foreground.root(),Secondary,true));
 for(const char* name:{"A1","A2","B","C"})assert(*foreground.flags(name)&Secondary);
 assert(base.flag("escape_wall",Hidden,true) && base.flag("spotlight",Hidden,true));
 assert(!foreground.flags("invented-group") && !foreground.flag("invented-group",Hidden,true));
 std::cout<<"original Harbor foreground144/base108 node owners, exact matching template positions and real group visibility PASS; no source MapGetWork/SceneReady receipt\n";
}
