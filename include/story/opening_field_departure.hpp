#pragma once
#include "opening_harbor_scene.hpp"
namespace ttyd {
// The arrival parent releases at three seconds. Its existing child retains its
// original absolute clock independently of ordinary field input owners.
struct OpeningFieldDeparture {
 OpeningHarborScene scene;
 uint64_t fieldAnchor=0,sourceAnchor=0,lastFieldTick=0;
 bool active=false;
 bool begin(const OpeningHarborScene& source,uint64_t fieldTick) {
  if(!source.storyPublished || source.complete || source.departureAt==UINT64_MAX || !source.paperDone)return false;
  scene=source;fieldAnchor=lastFieldTick=fieldTick;sourceAnchor=source.now;active=true;return true;
 }
 void advance(uint64_t fieldTick,uint32_t room) {
  if(!active || fieldTick<lastFieldTick)return;
  lastFieldTick=fieldTick;
  if(room!=1){active=false;return;} // Original map unload ends the local child.
  scene.advance(sourceAnchor+fieldTick-fieldAnchor);
  if(scene.complete)active=false;
 }
};
}
