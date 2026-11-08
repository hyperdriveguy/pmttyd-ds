#include "field_light_registry.hpp"
namespace ttyd {
namespace {
using namespace FieldLightRegistrySource;
constexpr uint32_t WordBytes=sizeof(uint32_t), VectorBytes=3*WordBytes;
constexpr uint16_t PresetFlags=0x1040;
int32_t signedWord(uint32_t value){return value<=INT32_MAX?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
bool compatible(const FieldLightRegistryServices& a,const FieldLightRegistryServices& b){
#define PIN(member) (!a.member || a.member==b.member)
 return a.user==b.user && PIN(group) && PIN(word) && PIN(half) && PIN(store) && PIN(storeHalf) && PIN(allocate) && PIN(clear) && PIN(copy) && PIN(compare) && PIN(format) && PIN(scale) && PIN(multiply);
#undef PIN
}
bool admit(FieldLightOperation operation,const FieldLightRegistryServices& s,FieldLightRegistryCursor& c){
 if(c.operation!=FieldLightOperation::Idle && (c.operation!=operation || !compatible(c.owner,s)))return false;
 c.operation=operation;c.owner=s;return true;
}
struct Preset {uint32_t destination,source,name,color,cutoff;bool scaled;};
constexpr std::array<Preset,4> PresetOrder{{
 {Presets+3*RecordBytes,Vectors,0x8041f8c4,Color,One,false},
 {Presets+2*RecordBytes,Vectors+6*WordBytes,0x802bf938,Color+WordBytes,One,false},
 {Presets+RecordBytes,Vectors+12*WordBytes,0x802bf940,Color+2*WordBytes,One,true},
 {Presets,Vectors+18*WordBytes,0x802bf94c,Color+3*WordBytes,Ten,false}
}};
enum InitPhase:uint32_t {Count0,Allocate0,Pointer0,Count1,Allocate1,Pointer1,BeginPreset,CopyPreset,ReadScale,ReadVectors,ReadColor,ReadRadius,ReadCutoff,StoreVectors,StoreColor,StoreRadius,StoreCutoff,ScalePreset,TransformPreset,NextPreset};
enum ResetPhase:uint32_t {ResetGroup,ResetCount,ResetPointer,ResetClear};
enum FindPhase:uint32_t {FindGroup,FindPointer,FindCount,FindFlags,FindCompare,FindAdvance};
enum EntryPhase:uint32_t {EntryGroup,EntryPointer,ScanCount,ScanFlags,ScanCompare,ScanAdvance,ScanResult,FormatName,CandidatePointer,FreeCount,FreePointer,FreeFlags,FreeAdvance,ClearRecord,CopyName,PublishFields,ReadActive,StoreActive,ReadType,StoreType,ReadScene,StoreScene,ReturnIndex};
enum PublishStep:uint32_t {ReadPositionX,ReadPositionY,ReadEntryColor,StorePositionX,StorePositionY,ReadAttenuationX,ReadPositionZ,ReadAttenuationY,StorePositionZ,ReadAttenuationZ,ReadVectorX,ReadVectorY,StoreVectorX,StoreVectorY,ReadVectorZ,StoreVectorZ,ReadReferenceX,ReadReferenceY,StoreReferenceX,StoreReferenceY,ReadReferenceZ,StoreReferenceZ,StoreEntryColor,StoreScalar0,StoreScalar1,StoreAttenuationX,StoreAttenuationY,StoreAttenuationZ,PublicationDone};
constexpr std::array<uint16_t,4> TypeFlags{{Ambient,Point,Directional,Spot}};
}
bool initializeFieldLights(const FieldLightRegistryServices& s,FieldLightRegistryCursor& c){
 if(!admit(FieldLightOperation::Initialize,s,c))return false;
 for(;;){
  const auto& p=PresetOrder[c.preset];
  switch(c.phase){
  case Count0:if(!s.store || !s.store(s.user,Work,Capacity))return false;c.phase=Allocate0;break;
  case Allocate0:if(!s.allocate || !s.allocate(s.user,0,Capacity*RecordBytes,c.pointer))return false;c.phase=Pointer0;break;
  case Pointer0:if(!s.store(s.user,Work+WordBytes,c.pointer))return false;c.phase=Count1;break;
  case Count1:if(!s.store(s.user,Work+DescriptorBytes,Capacity))return false;c.phase=Allocate1;break;
  case Allocate1:if(!s.allocate(s.user,0,Capacity*RecordBytes,c.pointer))return false;c.phase=Pointer1;break;
  case Pointer1:if(!s.store(s.user,Work+DescriptorBytes+WordBytes,c.pointer))return false;c.phase=BeginPreset;break;
  case BeginPreset:if(!s.storeHalf || !s.storeHalf(s.user,p.destination,PresetFlags))return false;c.phase=CopyPreset;break;
  case CopyPreset:if(!s.copy || !s.copy(s.user,p.destination+Name,p.name))return false;c.phase=ReadScale;break;
  case ReadScale:if(p.scaled && (!s.word || !s.word(s.user,CraftScale,c.scale)))return false;c.step=0;c.phase=ReadVectors;break;
  case ReadVectors:
   while(c.step<c.values.size()){if(!s.word || !s.word(s.user,p.source+WordBytes*c.step,c.values[c.step]))return false;++c.step;}
   c.phase=ReadColor;break;
  case ReadColor:if(!s.word(s.user,p.color,c.color))return false;c.phase=ReadRadius;break;
  case ReadRadius:if(!s.word(s.user,Zero,c.radius))return false;c.phase=ReadCutoff;break;
  case ReadCutoff:if(!s.word(s.user,p.cutoff,c.cutoff))return false;c.step=0;c.phase=StoreVectors;break;
  case StoreVectors:
   while(c.step<c.values.size()){if(!s.store(s.user,p.destination+Position+WordBytes*c.step,c.values[c.step]))return false;++c.step;}
   c.phase=StoreColor;break;
  case StoreColor:if(!s.store(s.user,p.destination+Rgba,c.color))return false;c.phase=StoreRadius;break;
  case StoreRadius:if(!s.store(s.user,p.destination+Scalar0,c.radius))return false;c.phase=StoreCutoff;break;
  case StoreCutoff:if(!s.store(s.user,p.destination+Scalar1,c.cutoff))return false;c.phase=p.scaled?ScalePreset:NextPreset;break;
  case ScalePreset:if(!s.scale || !s.scale(s.user,c.scale,c.matrix))return false;c.phase=TransformPreset;break;
  case TransformPreset:if(!s.multiply || !s.multiply(s.user,c.matrix,p.destination+Position,p.destination+Position))return false;c.phase=NextPreset;break;
  case NextPreset:if(++c.preset==PresetOrder.size()){c={};return true;}c.phase=BeginPreset;break;
  default:return false;
  }
 }
}
bool resetFieldLights(const FieldLightRegistryServices& s,FieldLightRegistryCursor& c){
 if(!admit(FieldLightOperation::Reset,s,c))return false;
 for(;;)switch(c.phase){
 case ResetGroup:if(!s.group || !s.group(s.user,c.group))return false;c.descriptor=Work+(c.group==1?DescriptorBytes:0);c.phase=ResetCount;break;
 case ResetCount:if(!s.word || !s.word(s.user,c.descriptor,c.count))return false;c.phase=ResetPointer;break;
 case ResetPointer:if(!s.word(s.user,c.descriptor+WordBytes,c.pointer))return false;c.phase=ResetClear;break;
 case ResetClear:if(!s.clear || !s.clear(s.user,c.pointer,c.count*RecordBytes))return false;c={};return true;
 default:return false;
 }
}
std::optional<uint32_t> findFieldLight(uint32_t name,const FieldLightRegistryServices& s,FieldLightRegistryCursor& c){
 const bool fresh=c.operation==FieldLightOperation::Idle;
 if(!admit(FieldLightOperation::Find,s,c))return {};
 if(fresh)c.name=name;
 for(;;)switch(c.phase){
 case FindGroup:if(!s.group || !s.group(s.user,c.group))return {};c.descriptor=Work+(c.group==1?DescriptorBytes:0);c.phase=FindPointer;break;
 case FindPointer:if(!s.word || !s.word(s.user,c.descriptor+WordBytes,c.pointer))return {};c.index=0;c.phase=FindCount;break;
 case FindCount:if(!s.word(s.user,c.descriptor,c.count))return {};if(signedWord(c.index)>=signedWord(c.count)){c={};return uint32_t(0);}c.phase=FindFlags;break;
 case FindFlags:if(!s.half || !s.half(s.user,c.pointer,c.flags))return {};c.phase=(c.flags&Active)?FindCompare:FindAdvance;break;
 case FindCompare:
  if(!s.compare || !s.compare(s.user,c.name,c.pointer+Name,c.comparison))return {};
  if(!c.comparison){const auto pointer=c.pointer;c={};return pointer;}c.phase=FindAdvance;break;
 case FindAdvance:++c.index;c.pointer+=RecordBytes;c.phase=FindCount;break;
 default:return {};
 }
}
std::optional<int32_t> enterFieldLight(const FieldLightEntryArguments& arguments,const FieldLightRegistryServices& s,FieldLightRegistryCursor& c){
 const bool fresh=c.operation==FieldLightOperation::Idle;
 if(!admit(FieldLightOperation::Enter,s,c))return {};
 if(fresh){c.arguments=arguments;c.name=arguments.name;}
 const auto& a=c.arguments;
 for(;;)switch(c.phase){
 case EntryGroup:if(!s.group || !s.group(s.user,c.group))return {};c.descriptor=Work+(c.group==1?DescriptorBytes:0);c.phase=EntryPointer;break;
 case EntryPointer:if(!s.word || !s.word(s.user,c.descriptor+WordBytes,c.pointer))return {};c.index=0;c.step=0;c.phase=ScanCount;break;
 case ScanCount:if(!s.word(s.user,c.descriptor,c.count))return {};c.phase=signedWord(c.index)<signedWord(c.count)?ScanFlags:ScanResult;break;
 case ScanFlags:if(!s.half || !s.half(s.user,c.pointer,c.flags))return {};c.phase=(c.flags&Active)?ScanCompare:ScanAdvance;break;
 case ScanCompare:if(!s.compare || !s.compare(s.user,c.pointer+Name,c.name,c.comparison))return {};c.phase=c.comparison?ScanAdvance:ScanResult;break;
 case ScanAdvance:++c.index;c.pointer+=RecordBytes;c.phase=ScanCount;break;
 case ScanResult:
  if(c.index>=Capacity){c.phase=FreeCount;break;}
  if(!c.step)c.suffix=0;else ++c.suffix;
  c.phase=FormatName;break;
 case FormatName:if(!s.format || !s.format(s.user,a.scratch,DuplicateFormat,c.name,signedWord(c.suffix)))return {};c.phase=CandidatePointer;break;
 case CandidatePointer:if(!s.word(s.user,c.descriptor+WordBytes,c.pointer))return {};c.name=a.scratch;c.index=0;c.step=1;c.phase=ScanCount;break;
 case FreeCount:if(!s.word(s.user,c.descriptor,c.count))return {};c.phase=FreePointer;break;
 case FreePointer:if(!s.word(s.user,c.descriptor+WordBytes,c.pointer))return {};c.index=0;c.phase=signedWord(c.count)>0?FreeFlags:ClearRecord;break;
 case FreeFlags:if(!s.half(s.user,c.pointer,c.flags))return {};c.phase=(c.flags&Active)?FreeAdvance:ClearRecord;break;
 case FreeAdvance:++c.index;c.pointer+=RecordBytes;c.phase=c.index<c.count?FreeFlags:ClearRecord;break;
 case ClearRecord:if(!s.clear || !s.clear(s.user,c.pointer,RecordBytes))return {};c.phase=CopyName;break;
 case CopyName:if(!s.copy || !s.copy(s.user,c.pointer+Name,c.name))return {};c.step=0;c.phase=PublishFields;break;
 case PublishFields: {
  const uint32_t coefficients=Attenuation+a.attenuation*VectorBytes;
  // Keep every reached read/store in original order. Current/reference reads
  // remain separate even if an admitted input aliases this destination.
  switch(c.step){
  case ReadPositionX:if(!s.word(s.user,a.position,c.values[0]))return {};break;
  case ReadPositionY:if(!s.word(s.user,a.position+WordBytes,c.values[1]))return {};break;
  case ReadEntryColor:if(!s.word(s.user,a.color,c.color))return {};break;
  case StorePositionX:if(!s.store || !s.store(s.user,c.pointer+Position,c.values[0]))return {};break;
  case StorePositionY:if(!s.store(s.user,c.pointer+Position+WordBytes,c.values[1]))return {};break;
  case ReadAttenuationX:if(!s.word(s.user,coefficients,c.attenuation[0]))return {};break;
  case ReadPositionZ:if(!s.word(s.user,a.position+2*WordBytes,c.values[2]))return {};break;
  case ReadAttenuationY:if(!s.word(s.user,coefficients+WordBytes,c.attenuation[1]))return {};break;
  case StorePositionZ:if(!s.store(s.user,c.pointer+Position+2*WordBytes,c.values[2]))return {};break;
  case ReadAttenuationZ:if(!s.word(s.user,coefficients+2*WordBytes,c.attenuation[2]))return {};break;
  case ReadVectorX:if(!s.word(s.user,a.vector,c.values[3]))return {};break;
  case ReadVectorY:if(!s.word(s.user,a.vector+WordBytes,c.values[4]))return {};break;
  case StoreVectorX:if(!s.store(s.user,c.pointer+Vector,c.values[3]))return {};break;
  case StoreVectorY:if(!s.store(s.user,c.pointer+Vector+WordBytes,c.values[4]))return {};break;
  case ReadVectorZ:if(!s.word(s.user,a.vector+2*WordBytes,c.values[5]))return {};break;
  case StoreVectorZ:if(!s.store(s.user,c.pointer+Vector+2*WordBytes,c.values[5]))return {};break;
  case ReadReferenceX:if(!s.word(s.user,a.position,c.values[0]))return {};break;
  case ReadReferenceY:if(!s.word(s.user,a.position+WordBytes,c.values[1]))return {};break;
  case StoreReferenceX:if(!s.store(s.user,c.pointer+Reference,c.values[0]))return {};break;
  case StoreReferenceY:if(!s.store(s.user,c.pointer+Reference+WordBytes,c.values[1]))return {};break;
  case ReadReferenceZ:if(!s.word(s.user,a.position+2*WordBytes,c.values[2]))return {};break;
  case StoreReferenceZ:if(!s.store(s.user,c.pointer+Reference+2*WordBytes,c.values[2]))return {};break;
  case StoreEntryColor:if(!s.store(s.user,c.pointer+Rgba,c.color))return {};break;
  case StoreScalar0:if(!s.store(s.user,c.pointer+Scalar0,a.scalars[0]))return {};break;
  case StoreScalar1:if(!s.store(s.user,c.pointer+Scalar1,a.scalars[1]))return {};break;
  case StoreAttenuationX:if(!s.store(s.user,c.pointer+Coefficients,c.attenuation[0]))return {};break;
  case StoreAttenuationY:if(!s.store(s.user,c.pointer+Coefficients+WordBytes,c.attenuation[1]))return {};break;
  case StoreAttenuationZ:if(!s.store(s.user,c.pointer+Coefficients+2*WordBytes,c.attenuation[2]))return {};break;
  default:c.phase=ReadActive;continue;
  }
  ++c.step;break;
 }
 case ReadActive:if(!s.half(s.user,c.pointer,c.flags))return {};c.flags|=Active;c.phase=StoreActive;break;
 case StoreActive:if(!s.storeHalf || !s.storeHalf(s.user,c.pointer,c.flags))return {};c.phase=a.type<TypeFlags.size()?ReadType:(a.flags&2)?ReadScene:ReturnIndex;break;
 case ReadType:if(!s.half(s.user,c.pointer,c.flags))return {};c.flags|=TypeFlags[a.type];c.phase=StoreType;break;
 case StoreType:if(!s.storeHalf(s.user,c.pointer,c.flags))return {};c.phase=(a.flags&2)?ReadScene:ReturnIndex;break;
 case ReadScene:if(!s.half(s.user,c.pointer,c.flags))return {};c.flags|=Scene;c.phase=StoreScene;break;
 case StoreScene:if(!s.storeHalf(s.user,c.pointer,c.flags))return {};c.phase=ReturnIndex;break;
 case ReturnIndex:{const auto index=signedWord(c.index);c={};return index;}
 default:return {};
 }
}
}
