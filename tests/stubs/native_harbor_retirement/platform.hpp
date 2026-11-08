#pragma once
#include "game.hpp"
#include <cassert>
#include <vector>
namespace ttyd {
// TEST GPU boundary; records actual adapter calls, never certifies hardware.
struct RetirementGpu {
 bool frame=false,begin=true,upper=true,lower=true;
 unsigned sync=0,ended=0,cleared=0,selected=0,freed=0;
 std::vector<unsigned> targets;
};
inline RetirementGpu retirementGpu;
constexpr unsigned C3D_FRAME_SYNCDRAW=1,C3D_CLEAR_ALL=3;
inline bool C3D_FrameBegin(unsigned flags){assert(flags==C3D_FRAME_SYNCDRAW&&!retirementGpu.frame);retirementGpu.frame=retirementGpu.begin;return retirementGpu.frame;}
inline void C3D_FrameEnd(unsigned flags){assert(!flags&&retirementGpu.frame);retirementGpu.frame=false;++retirementGpu.ended;}
inline void C3D_FrameSync(){assert(!retirementGpu.frame);++retirementGpu.sync;}
inline void C3D_RenderTargetClear(unsigned target,unsigned flags,uint32_t color,unsigned depth){assert(retirementGpu.frame&&flags==C3D_CLEAR_ALL&&color==0xff&&!depth);++retirementGpu.cleared;retirementGpu.targets.push_back(target);}
inline bool C3D_FrameDrawOn(unsigned target){assert(retirementGpu.frame);++retirementGpu.selected;return target==1?retirementGpu.upper:retirementGpu.lower;}
inline void Tex3DS_TextureFree(unsigned){assert(!retirementGpu.frame);++retirementGpu.freed;}
inline void C3D_TexDelete(unsigned*){assert(!retirementGpu.frame);++retirementGpu.freed;}
class Renderer {
public:
 struct Texture{unsigned imported=0,texture=0;};
 struct CineActor{Texture image;};
 bool c3dReady=true,sourceMapFrameActive_=false,tutorialReady=true,allow=true;
 unsigned top=1,bottom=2,harborBattleBackground=1,harborBattleStage=1,retirements=0;
 HarborTutorial *tutorialStartupOwner=nullptr,*tutorialFadeOwner=nullptr;
 std::array<CineActor,4> tutorialActors{};
 std::array<std::array<int,2>,4> tutorialDrawRanges{};
 struct NativeHarborRetirement {
  const HarborTutorial* facade=nullptr;
  NativeFacadeRegistration::Proof proof;
  std::weak_ptr<const void> runtime;
 } harborRetirement_;
 bool nativeHarborMenuMatches(const std::shared_ptr<const void>&)const{return allow;}
 bool nativeHarborNiceMatches(const std::shared_ptr<const void>&)const{return allow;}
 bool nativeHarborSmokeMatches(const std::shared_ptr<const void>&)const{return allow;}
 bool nativeHarborStarPointMatches(const std::shared_ptr<const void>&)const{return allow;}
 bool nativeHarborRewardsMatch(const std::shared_ptr<const void>&)const{return allow;}
 void retireNativeHarborMenu(){assert(!retirementGpu.frame);++retirements;}
 void retireNativeHarborNice(){assert(!retirementGpu.frame);++retirements;}
 void retireNativeHarborSmoke(){assert(!retirementGpu.frame);++retirements;}
 void retireNativeHarborStarPoint(){assert(!retirementGpu.frame);++retirements;}
 void retireNativeHarborRewards(){assert(!retirementGpu.frame);++retirements;}
 void releaseClassMenuDrawing(){assert(!retirementGpu.frame);}
 void releaseWeaponMenuSurfaces(){assert(!retirementGpu.frame);}
 void releaseAudienceDrawing(){assert(!retirementGpu.frame);}
 void releaseTutorialStartupResources(){assert(!retirementGpu.frame);tutorialStartupOwner=nullptr;}
 void releaseTutorialCurtain(){assert(!retirementGpu.frame);tutorialFadeOwner=nullptr;}
 void freeRoom(unsigned& room){assert(!retirementGpu.frame);room=0;}
 bool retireNativeHarborPresentation(HarborTutorial&,const std::shared_ptr<const void>&);
 bool nativeHarborPresentationRetired(const HarborTutorial&,const std::shared_ptr<const void>&)const;
 bool renderNativeHarborRetirement(const Game&);
};
}
