#include "field_camera_publication.hpp"
#include "field_camera.hpp"
#include "game.hpp"
#include "trio_attack.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main(){
    auto bank=std::make_shared<CameraManager>();bank->initializeBoot(608,480);auto* camera=bank->camera(4);
    assert(!camera->viewPublished);
    auto field=adaptedFieldCameraFrame(1,2,3);field.yaw=42;
    camera->publication.flags=8;camera->publication.rollDegrees=12;camera->publication.translation={3,4,5};camera->aspect=1.7f;
    assert(publishNativeFieldCamera(*bank,field));
    assert(camera->viewPublished && camera->eye[0]==100 && camera->eye[1]==field.eye.y*100 && camera->at[2]==field.at.z*100);
    assert(camera->fov==field.fov && camera->nearPlane==field.nearPlane*100 && camera->farPlane==field.farPlane*100 && camera->aspect==1.7f && camera->yaw==42);
    assert(camera->publication.flags==8 && camera->publication.rollDegrees==12 && camera->publication.translation==CameraDepthPoint({3,4,5}));
    const auto published=camera->matrices;
    // Same physical point produces the same normalized clip position in native
    // and source engine units, including the translated field view.
    CameraDepthSnapshot native;native.view=field.view;const auto& p=field.projection;
    native.projection={{{p[0],0,p[1],0},{0,p[2],p[3],0},{0,0,p[4],p[5]},{0,0,-1,0}}};
    const auto a=cameraDepthProject(native,cameraDepthView(native,{1,2,0})),b=cameraDepthProject(published,cameraDepthView(published,{100,200,0}));
    for(unsigned axis=0;axis<3;++axis)assert(std::fabs(a[axis]-b[axis])<1e-5f);
    auto invalid=field;invalid.view={};assert(!publishNativeFieldCamera(*bank,invalid) && camera->matrices.view==published.view);
    camera->mode=2;field.fov=31;assert(!publishNativeFieldCamera(*bank,field) && camera->fov!=31);camera->mode=0;
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs") && loadTrioAttackAssets("romfs"));
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);
    TrioAttack battle;assert(battle.begin(menu,scene,10,{},nullptr,nullptr,nullptr,nullptr,{},{},{},{},{},bank));
    assert(battle.cameras()==bank.get() && bank->camera(4)->mode==2 && battle.cameraDepthSnapshot());
    assert(battle.cameraDepthSnapshot()->view==published.view && battle.cameraDepthSnapshot()->projection==published.projection && !battle.cameraHistoryComplete());
    Game game;game.restore(game.progress);game.tick({});assert(game.cameraManager->camera(4)->viewPublished);
    auto* retained=game.cameraManager->camera(4);retained->mode=2;retained->fov=33;const auto view=retained->matrices.view;
    game.tick({});assert(retained->fov==33 && retained->matrices.view==view);
}
