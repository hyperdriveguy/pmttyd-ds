#include "field_object_collision.hpp"
#include "game.hpp"
#include "head_collision.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
bool near(float a,float b){return std::fabs(a-b)<.000001f;}
}
int main() {
    assert(loadFieldActorCatalog("romfs"));const auto& assets=fieldActorCatalog();
    std::vector<FieldObjectTriangle> triangles;
    buildFieldObjectCollision(assets,0,8,0,0,triangles);assert(triangles.size()==12);
    const auto& actor=assets.actors()[3];const auto& model=assets.models()[actor.model];
    assert(actor.sourceName=="mobj_save" && model.hitMode==1);
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
    std::cout<<"Original bbox cube and native head contact verified\n";
}
