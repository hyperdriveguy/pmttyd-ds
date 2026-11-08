#include "field_map_light_producer.hpp"
namespace ttyd {
namespace {
using namespace FieldMapLightSource;
constexpr uint32_t WordBytes=sizeof(uint32_t);
constexpr std::array<uint32_t,4> KindNames{{AmbientName,PointName,DirectionalName,SpotName}};
enum Phase:uint32_t {ReadTable,ReadCount,ReadRecord,ReadScale,ScalePosition,ReadKind,CompareKind,ReadMagic,ReadFlags,ReadColor,StoreColor,ReadName,ReadScalar0,ReadScalar1,ReadAttenuation,CreateEntry,Advance};
int32_t signedWord(uint32_t value){return value<=INT32_MAX?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
bool compatible(const FieldMapLightServices& a,const FieldMapLightServices& b){
#define PIN(member) (!a.member || a.member==b.member)
 return a.user==b.user && PIN(word) && PIN(store) && PIN(compare) && PIN(scale) && PIN(entry);
#undef PIN
}
}
bool loadFieldMapLights(uint32_t map,uint32_t position,uint32_t color,uint32_t format,const FieldMapLightServices& s,FieldMapLightCursor& c){
 if(c.active && !compatible(c.owner,s))return false;
 if(!c.active){c.active=true;c.map=map;c.positionTemp=position;c.colorTemp=color;c.formatTemp=format;}
 c.owner=s;
 for(;;)switch(c.phase){
 case ReadTable:if(!s.word || !s.word(s.user,c.map+TableField,c.table))return false;c.phase=ReadCount;break;
 case ReadCount:
  if(!s.word(s.user,c.table,c.count))return false;
  if(signedWord(c.index)>=signedWord(c.count)){c={};return true;}
  c.phase=ReadRecord;break;
 case ReadRecord:if(!s.word(s.user,c.table+WordBytes+c.offset,c.record))return false;c.phase=ReadScale;break;
 case ReadScale:if(!s.word(s.user,PositionScale,c.scale))return false;c.arguments={};c.arguments.type=1;c.phase=ScalePosition;break;
 case ScalePosition:if(!s.scale || !s.scale(s.user,c.record+Position,c.scale,c.positionTemp))return false;c.kindIndex=0;c.phase=ReadKind;break;
 case ReadKind:if(!s.word(s.user,c.record+Kind,c.kind))return false;c.phase=CompareKind;break;
 case CompareKind:
  if(!s.compare || !s.compare(s.user,c.kind,KindNames[c.kindIndex],c.comparison))return false;
  if(!c.comparison){c.arguments.type=c.kindIndex;c.phase=ReadMagic;}
  else if(++c.kindIndex==KindNames.size())c.phase=ReadMagic;
  else c.phase=ReadKind;
  break;
 case ReadMagic:if(!s.word(s.user,c.record+Magic,c.magic))return false;c.phase=c.magic==SupportedFlagsMagic?ReadFlags:ReadColor;break;
 case ReadFlags:if(!s.word(s.user,c.record+Flags,c.arguments.flags))return false;c.phase=ReadColor;break;
 case ReadColor:if(!s.word(s.user,c.record+Color,c.color))return false;c.phase=StoreColor;break;
 case StoreColor:if(!s.store || !s.store(s.user,c.colorTemp,c.color))return false;c.phase=ReadName;break;
 case ReadName:if(!s.word(s.user,c.record,c.arguments.name))return false;c.phase=ReadScalar0;break;
 case ReadScalar0:if(!s.word(s.user,c.record+Scalar0,c.arguments.scalars[0]))return false;c.phase=ReadScalar1;break;
 case ReadScalar1:if(!s.word(s.user,c.record+Scalar1,c.arguments.scalars[1]))return false;c.phase=ReadAttenuation;break;
 case ReadAttenuation:
  if(!s.word(s.user,c.record+Attenuation,c.arguments.attenuation))return false;
  c.arguments.position=c.positionTemp;c.arguments.vector=c.record+Vector;c.arguments.color=c.colorTemp;c.arguments.scratch=c.formatTemp;c.phase=CreateEntry;break;
 case CreateEntry:if(!s.entry || !s.entry(s.user,c.arguments,c.result))return false;c.phase=Advance;break;
 case Advance:++c.index;c.offset+=WordBytes;c.phase=ReadTable;break;
 default:return false;
 }
}
}
