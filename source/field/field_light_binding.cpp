#include "field_light_binding.hpp"

namespace ttyd {
namespace {
int32_t signedWord(uint32_t value) {
    return value<=INT32_MAX ? int32_t(value) : int32_t(int64_t(value)-0x100000000LL);
}
bool compatibleProviders(const FieldLightBindServices& old,const FieldLightBindServices& next) {
    return old.user==next.user && (!old.word || old.word==next.word) &&
        (!old.value || old.value==next.value) && (!old.lookup || old.lookup==next.lookup) &&
        (!old.sdk || old.sdk==next.sdk);
}
}
std::optional<int32_t> fieldLightBindEvent(EventEntry& event,int32_t first,
    const FieldLightBindServices& services,FieldLightBindCursor& cursor) {
    using Phase=FieldLightBindPhase;
    using namespace FieldLightSource;
    constexpr uint32_t WordBytes=sizeof(uint32_t);
    if(cursor.active() && (cursor.event!=&event || cursor.id!=event.id ||
        !compatibleProviders(cursor.owner,services)))return {};
    if(!cursor.active()) {
        cursor.event=&event;
        cursor.id=event.id;
        cursor.args=event.arguments;
        cursor.first=first!=0;
        cursor.phase=Phase::ReadLightArgument;
    }
    // Capture first before provider admission. USER_FUNC retries report first0
    // even when initialization has not reached its first child yet.
    cursor.owner=services;
    if(!services.word || !services.value || !services.lookup || !services.sdk)return {};
    auto advance=[&](Phase phase) {
        cursor.phase=phase;
        cursor.index=0;
        cursor.queued=false;
    };
    auto sdk=[&](uint32_t function,uint32_t matrix,uint32_t source,uint32_t destination,
        const std::array<uint32_t,3>& vector,const std::array<uint32_t,3>& other) {
        if(!cursor.queued) {
            cursor.request={};
            cursor.request.function=function;
            cursor.request.matrix=matrix;
            cursor.request.source=source;
            cursor.request.destination=destination;
            cursor.request.vector=vector;
            cursor.request.other=other;
            cursor.request.localMatrix=cursor.matrix;
            cursor.queued=true;
        }
        return services.sdk(services.user,cursor.request);
    };
    for(;;)switch(cursor.phase) {
    case Phase::ReadLightArgument:
        if(!services.word(services.user,cursor.args,cursor.raw))return {};
        advance(Phase::ResolveLightArgument);break;
    case Phase::ResolveLightArgument: {
        int32_t value=0;
        if(!services.value(services.user,event,signedWord(cursor.raw),value))return {};
        cursor.lightName=uint32_t(value);advance(Phase::ReadMapArgument);break;
    }
    case Phase::ReadMapArgument:
        if(!services.word(services.user,cursor.args+WordBytes,cursor.raw))return {};
        advance(Phase::ResolveMapArgument);break;
    case Phase::ResolveMapArgument: {
        int32_t value=0;
        if(!services.value(services.user,event,signedWord(cursor.raw),value))return {};
        cursor.mapName=uint32_t(value);advance(Phase::ReadModeArgument);break;
    }
    case Phase::ReadModeArgument:
        if(!services.word(services.user,cursor.args+2*WordBytes,cursor.raw))return {};
        advance(Phase::ResolveModeArgument);break;
    case Phase::ResolveModeArgument: {
        int32_t value=0;
        if(!services.value(services.user,event,signedWord(cursor.raw),value))return {};
        cursor.mode=uint32_t(value);advance(Phase::FindLight);break;
    }
    case Phase::FindLight:
        if(!services.lookup(services.user,FindLight,cursor.lightName,cursor.light))return {};
        advance(Phase::FindMap);break;
    case Phase::FindMap:
        if(!services.lookup(services.user,FindMapObject,cursor.mapName,cursor.map))return {};
        advance(cursor.first?Phase::ReadInitialOrigin:Phase::ReadScratch);break;
    case Phase::ReadInitialOrigin:
        while(cursor.index<cursor.vector.size()) {
            if(!services.word(services.user,InitialOrigin+WordBytes*cursor.index,cursor.vector[cursor.index]))return {};
            ++cursor.index;
        }
        advance(Phase::TransformInitialOrigin);break;
    case Phase::TransformInitialOrigin:
        if(!sdk(MatrixVector,cursor.map+MapBaseMatrix,0,0,cursor.vector,{}))return {};
        cursor.vector=cursor.request.output;advance(Phase::SubtractReference);break;
    case Phase::SubtractReference:
        if(!sdk(VectorSubtract,LocalMatrix,cursor.light+LightReferencePosition,0,cursor.vector,{}))return {};
        cursor.vector=cursor.request.output;advance(Phase::StoreScratch);break;
    case Phase::StoreScratch:
        // Original scratch E78/7c/80 aliases these semantic timer fields.
        // Retain float bits and original XYZ store order; never convert values.
        event.waitCount=signedWord(cursor.vector[0]);
        event.waitStart=(uint64_t(cursor.vector[1])<<32)|uint32_t(event.waitStart);
        event.waitStart=(event.waitStart&0xffffffff00000000ULL)|cursor.vector[2];
        advance(Phase::ReadScratch);break;
    case Phase::ReadScratch:
        cursor.vector={uint32_t(event.waitCount),uint32_t(event.waitStart>>32),uint32_t(event.waitStart)};
        advance(cursor.mode==DynamicMode?Phase::TransformOffset:
            cursor.mode==TranslationMode?Phase::ReadTranslation:Phase::ReadCurrentOrigin);break;
    case Phase::ReadTranslation:
        while(cursor.index<cursor.translation.size()) {
            if(!services.word(services.user,cursor.map+MapTranslationX+MatrixRowBytes*cursor.index,cursor.translation[cursor.index]))return {};
            ++cursor.index;
        }
        advance(Phase::BuildTranslation);break;
    case Phase::BuildTranslation:
        if(!sdk(TranslationMatrix,LocalMatrix,0,0,cursor.translation,{}))return {};
        cursor.matrix=cursor.request.outputMatrix;advance(Phase::TransformOffset);break;
    case Phase::TransformOffset:
        if(!sdk(MatrixVector,cursor.mode==DynamicMode?cursor.map+MapDynamicMatrix:LocalMatrix,0,0,cursor.vector,{}))return {};
        cursor.vector=cursor.request.output;advance(Phase::ReadCurrentOrigin);break;
    case Phase::ReadCurrentOrigin:
        while(cursor.index<cursor.origin.size()) {
            if(!services.word(services.user,CurrentOrigin+WordBytes*cursor.index,cursor.origin[cursor.index]))return {};
            ++cursor.index;
        }
        advance(Phase::TransformCurrentOrigin);break;
    case Phase::TransformCurrentOrigin:
        if(!sdk(MatrixVector,cursor.map+MapBaseMatrix,0,0,cursor.origin,{}))return {};
        cursor.origin=cursor.request.output;advance(Phase::PublishPosition);break;
    case Phase::PublishPosition:
        if(!sdk(VectorAdd,LocalMatrix,0,cursor.light+LightCurrentPosition,cursor.origin,cursor.vector))return {};
        cursor={};return 0;
    default:return {};
    }
}
}
