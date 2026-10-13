#include "field_object_collision.hpp"
#include "game.hpp"
#include "head_collision.hpp"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace ttyd;
namespace {
bool near(float a,float b){return std::fabs(a-b)<.000001f;}
}
int main() {
    assert(loadFieldActorCatalog("romfs"));const auto& assets=fieldActorCatalog();
    std::vector<FieldObjectTriangle> triangles;
    buildFieldObjectCollision(assets,0,8,0,0,triangles);assert(triangles.size()==12);
    const auto actor=*std::find_if(assets.actors().begin(),assets.actors().end(),[](const auto& a){return a.sourceName=="mobj_save";});const auto& model=assets.models()[actor.model];
    assert(actor.sourceName=="mobj_save" && model.hitMode==1 && model.originalName=="MOBJ_SaveBlock");
    assert(near(model.hitBounds[0],-1.27f) && near(model.hitBounds[4],2.500000238418579f));
    float minimum=10,maximum=-10;
    for(const auto& triangle:triangles) {
        assert(triangle.owner==actor.key && triangle.attributes==0x80000000 && triangle.cull==1);
        for(unsigned n=0;n<3;++n){minimum=std::min(minimum,triangle.vertices[n*3+1]);maximum=std::max(maximum,triangle.vertices[n*3+1]);}
    }
    assert(near(minimum,.6f) && near(maximum,.85f));
    // Original third strip is the bottom; its two normals point downward.
    for(unsigned i:{4u,5u}) {
        const auto& v=triangles[i].vertices;
        const float ny=(v[5]-v[2])*(v[6]-v[0])-(v[3]-v[0])*(v[8]-v[2]);assert(ny<0);
    }
    clearCollision();
    auto hit=searchFieldHead({1.85f,-.55f},{1,0},.24f,.03f,.1f,nullptr,false,&triangles);
    assert(hit.contact && hit.objectKey==actor.key && hit.node.sourceNode==0 && hit.node.attributes==0x80000000);
    assert(near(hit.correction,headClampedRoot(headContactHeight(.6f))));
    // The original upward ray rejects its far endpoint, even on a solid face.
    auto endpoint=triangles;const float high=.24f+HeadBodyHeight*.5f+HeadBodyHeight+.1f;
    for(auto& tri:endpoint)for(unsigned vertex=0;vertex<3;++vertex)tri.vertices[vertex*3+1]=high;
    auto edge=searchFieldHead({1.85f,-.55f},{1,0},.24f,.03f,.1f,nullptr,false,&endpoint);
    assert(!edge.contact && edge.clearance==-.01f && !edge.objectKey);
    // A near miss must not publish an owner; top faces are back-facing upwards.
    hit=searchFieldHead({1.85f,-.55f},{1,0},.2f,.03f,.1f,nullptr,false,&triangles);
    assert(!hit.contact && !hit.objectKey && hit.clearance>0);
    hit=searchFieldHead({1.85f,-.55f},{1,0},.6f,.03f,.1f,nullptr,false,&triangles);
    assert(!hit.contact && !hit.objectKey);
    hit=searchFieldHead({2.2f,-.55f},{1,0},.24f,.03f,.1f,nullptr,false,&triangles);
    assert(!hit.contact && !hit.objectKey);
    for(auto settings:std::vector<std::array<uint32_t,3>>{{0,341,0},{0,8,0x20},{2,8,0}}) {
        buildFieldObjectCollision(assets,settings[0],int32_t(settings[1]),0,settings[2],triangles);assert(triangles.empty());
    }
    buildFieldObjectCollision(assets,0,std::nullopt,0,0,triangles);assert(triangles.empty());
    assert(loadGround("romfs/map.ground") && loadCollision("romfs/map.walls") && loadHead("romfs/map.head"));
    Game game;Progress saved;saved.room=0;saved.originalStory=8;saved.position={1.85f,-.55f};saved.crumpDefeated=true;
    game.restore(saved);Input jump;jump.confirm=true;jump.jumpHeld=true;game.tick(jump);
    bool contacted=false;
    for(unsigned frame=0;frame<80;++frame) {
        game.tick(Input{});
        if(game.headObjectKey==actor.key){contacted=true;assert(game.headContact.attributes==0x80000000);assert(game.jumpHeight<.24f);}
    }
    assert(contacted && !game.airborne());
    // All twelve source faces remain one primitive. Downward support selects
    // the top; horizontal rays select its sides with independent owner identity.
    buildFieldObjectCollision(assets,0,8,0,0,triangles);assert(triangles[0].ordinarySolid);
    FieldSlope slope;
    auto support=searchFieldSupport({1.85f,-.55f},{1,0},.87f,-.03f,slope,.1f,&triangles);
    assert(support.found && near(support.height,.85f) && support.objectKey==actor.key && support.contact.sourceNode==0);
    support=searchFieldSupport({1.85f,-.55f},{1,0},.5f,-.03f,slope,.1f,&triangles);
    assert(!support.found); // One-sided cube bottom cannot be a floor below it.
    auto wall=fieldWallRay({150,70,-55},{1,0,0},50,&triangles);
    assert(wall.token && wall.objectKey==actor.key && std::fabs(wall.distance-22.300003f)<.00002f);
    assert(wall.attributes==0x80000000 && wall.normal.x<0);
    wall=fieldWallRay({225,70,-55},{-1,0,0},50,&triangles);
    assert(wall.token && wall.objectKey==actor.key && std::fabs(wall.distance-27.300003f)<.00002f && wall.normal.x>0);
    wall=fieldWallRay({185,70,-55},{1,0,0},50,&triangles);assert(!wall.objectKey); // Inside backface.
    auto disabled=triangles;for(auto& tri:disabled)tri.ordinarySolid=false;
    support=searchFieldSupport({1.85f,-.55f},{1,0},.87f,-.03f,slope,.1f,&disabled);assert(!support.found);
    wall=fieldWallRay({150,70,-55},{1,0,0},50,&disabled);assert(!wall.objectKey);
    // Original model names, rather than a blanket bit31 exemption, select
    // ordinary floor/wall admission while leaving head geometry intact.
    std::ifstream packed("romfs/field-actors.pmnpc",std::ios::binary);
    const std::vector<char> catalogBytes((std::istreambuf_iterator<char>(packed)),{});
    for(const char* name:{"MOBJ_HiddenHatenaBlock","MOBJ_HiddenBadgeBlock","MOBJ_Hidden10CountBlock"}) {
        auto bytes=catalogBytes;const size_t modelStride=std::memcmp(bytes.data(),"PMNPC008",8)==0?100:std::memcmp(bytes.data(),"PMNPC005",8)==0 || std::memcmp(bytes.data(),"PMNPC006",8)==0 || std::memcmp(bytes.data(),"PMNPC007",8)==0?100:96;
        const size_t modelName=16+2*modelStride+64;
        std::memset(bytes.data()+modelName,0,32);std::memcpy(bytes.data()+modelName,name,std::strlen(name));
        FILE* stream=std::tmpfile();assert(stream);assert(std::fwrite(bytes.data(),1,bytes.size(),stream)==bytes.size());std::rewind(stream);
        FieldActorAssets hidden;assert(hidden.read(stream));std::fclose(stream);
        std::vector<FieldObjectTriangle> hiddenCube;buildFieldObjectCollision(hidden,0,8,0,0,hiddenCube);
        assert(hiddenCube.size()==12 && !hiddenCube[0].ordinarySolid);
        hit=searchFieldHead({1.85f,-.55f},{1,0},.24f,.03f,.1f,nullptr,false,&hiddenCube);
        assert(hit.contact && hit.objectKey==actor.key);
        support=searchFieldSupport({1.85f,-.55f},{1,0},.87f,-.03f,slope,.1f,&hiddenCube);assert(!support.found);
        wall=fieldWallRay({150,70,-55},{1,0,0},50,&hiddenCube);assert(!wall.objectKey);
    }
    // Restore, standing, complete jump/landing and walking off the real top.
    saved.elevation=.9f;game.restore(saved);
    float checkpointHeight=saved.elevation;assert(game.checkpointGround(checkpointHeight) && near(checkpointHeight,.85f));
    assert(near(game.groundHeight,.85f) && game.groundObjectKey==actor.key);
    game.tick(Input{});assert(near(game.groundHeight,.85f) && game.groundObjectKey==actor.key);
    game.tick(jump);
    for(unsigned frame=0;frame<100;++frame)game.tick(Input{});
    assert(!game.airborne() && near(game.groundHeight,.85f) && game.groundObjectKey==actor.key);
    const uint64_t version=fieldActorCatalogGeneration();assert(loadFieldActorCatalog("romfs"));
    assert(fieldActorCatalogGeneration()==version+1);game.tick(Input{});
    assert(game.groundObjectKey==actor.key);

    Input walk;walk.x=1;
    for(unsigned frame=0;frame<100;++frame)game.tick(walk);
    assert(!game.groundObjectKey && game.groundHeight<.2f);
    game.restore(saved);game.fieldSystemFlags=0x20;game.tick(Input{});
    assert(!game.groundObjectKey);
    std::cout<<"Original bbox cube, head, floor and side collision verified\n";
}
