#include "field_pipe.hpp"
#include "game.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
namespace ttyd {
static std::vector<FieldPipe> pipes;
// PMPIPE1 private LE catalog has exactly2 records, 48 bytes each. Validate
// source/target rooms, nonzero node IDs, case6/10 and finite endpoints before
// replacing the retained table; direction is source metadata, not an enum ordinal.
bool loadFieldPipes(const char* path) {
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    char magic[8];uint32_t count=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMPIPE1",8) && std::fread(&count,4,1,f)==1 && count==2;
    std::vector<FieldPipe> next(ok?count:0);
    ok=ok && std::fread(next.data(),sizeof(FieldPipe),count,f)==count && std::fgetc(f)==EOF;std::fclose(f);
    for(const auto& p:next) {
        if(!roomAsset(p.source) || !roomAsset(p.target) || !p.node || !p.arrivalNode || (p.caseType!=6 && p.caseType!=10))ok=false;
        for(float v:{p.x,p.y,p.z,p.arrivalX,p.arrivalY,p.arrivalZ})if(!std::isfinite(v))ok=false;
    }
    if(ok)pipes=std::move(next);
    return ok;
}
uint32_t Game::pipeTarget() const {return pipeRequest>=0 && size_t(pipeRequest)<pipes.size()?pipes[pipeRequest].target:UINT32_MAX;}
// Exact contact admission and known story precede the fade/room request.
// Target5 early underground envelope excludes story>=20; source 2 departure
// delegates unchanged professor state and flag 1207 rather than inventing a scene.
bool Game::tryFieldPipe(int8_t mainY) {
    if(pipeRequest>=0 || warpRequest>=0 || !progress.originalStory || airborne() || professorStory.running() || fieldDoor.active())return false;
    for(size_t n=0;n<pipes.size();++n) {
        const auto& p=pipes[n];if(p.source!=progress.room || !fieldPipeAdmitted(p,groundContact.sourceNode,wallContact.sourceNode,mainY,roadAdmission))continue;
        // Later badge-entry callback and full underground story are separate owners.
        if(p.target==5 && *progress.originalStory>=20)return false;
        if(p.source==2) {
            if(!professorStory.departThroughPipe(ticks*1000/60,*progress.originalStory,progress.originalFlag1207))return false;
            progress.originalFlag1207=professorStory.professorDeparted;
        }
        pipeRequest=int(n);saveRequested=false;fade.startOut();return true;
    }
    return false;
}
// Stage a Progress copy and target arrival probe before restore. Native
// horizontal offsets+.20/-.40 are source+20/-40 units; this is not the full
// mot_dokan animation/entry-clock or GPU room-residency transaction.
bool Game::finishPipeWarp() {
    if(pipeRequest<0 || size_t(pipeRequest)>=pipes.size())return false;
    const auto p=pipes[pipeRequest];Progress next=progress;next.room=p.target;next.interiorMask=0;
    // Source horizontal direction6 arrival: endX=center+20, startX=center-40.
    // Vertical direction9 end is the hit center. Exact mot_dokan paper motion,
    // source ground ray and entry clock remain adapters.
    next.position={p.arrivalX+(p.target==5?.20f:0),p.arrivalZ};
    float height=p.arrivalY;
    const Vec2 rayPoint{p.arrivalX-(p.target==5?.40f:0),p.arrivalZ};
    if(p.target==5 && !roadStartGround(rayPoint,p.arrivalY,height))return false;
    next.elevation=height;restore(next);pipeRequest=-1;saveRequested=true;return true;
}
void Game::cancelPipeWarp(){pipeRequest=-1;saveRequested=false;fade.startIn();}
static_assert(sizeof(FieldPipe)==48,"Original pipe metadata record");
}
