#include "platform.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
namespace ttyd {
// Move retained ship/wake geometry into the field owner after story publish.
// Event groups 1(ship)/13(wake) are packed in layer bits 26..31; cutaway bits
//8..23 must be zero. UINT32_MAX marks absent old->compact vertex mappings.
// Duplicate/overlapping ranges fail before any GPU texture handles move.
bool Renderer::prepareOpeningDeparture(const OpeningHarborScene& scene,uint64_t fieldTick) {
 if(departureRoom.vertices)return true;
 if(!scene.storyPublished || scene.complete || !scene.paperDone || frontMapName!="opening-gor_00" || !frontRoom.vertices)return false;
 C3D_FrameSync(); // No queued texture or vertex pointer may survive ownership transfer.
 Room pending;std::vector<uint32_t> indices(frontRoom.vertexCount,UINT32_MAX),originals;
 std::vector<uint32_t> textures;
 for(auto batch:frontRoom.batches) {
  const auto group=batch.layer>>26;if(group!=1 && group!=13)continue;
  if(!batch.count || batch.start>frontRoom.vertexCount || batch.count>frontRoom.vertexCount-batch.start || ((batch.layer>>8)&65535))return false;
  const uint32_t first=uint32_t(originals.size());
  for(uint32_t i=batch.start;i<batch.start+batch.count;++i){if(indices[i]!=UINT32_MAX)return false;indices[i]=uint32_t(originals.size());originals.push_back(i);}
  batch.start=first;
  if(batch.texture>=0) {
   if(uint32_t(batch.texture)>=frontRoom.textures.size())return false;
   auto found=std::find(textures.begin(),textures.end(),uint32_t(batch.texture));
   if(found==textures.end()){textures.push_back(uint32_t(batch.texture));batch.texture=int32_t(textures.size()-1);}
   else batch.texture=int32_t(found-textures.begin());
  }
  pending.batches.push_back(batch);
 }
 if(originals.empty())return false;
 OpeningShipObjects objects=frontShipObjects;
 if(!objects.remapVertices(indices,uint32_t(originals.size())))return false;
 pending.vertexCount=uint32_t(originals.size());pending.vertices=static_cast<Vertex*>(linearAlloc(size_t(pending.vertexCount)*sizeof(Vertex)));
 if(!pending.vertices)return false;
 for(uint32_t i=0;i<pending.vertexCount;++i)pending.vertices[i]=frontRoom.vertices[originals[i]];
 // Preserve source map animation phase while rebasing to the field clock. Only
 // bindings in the compact original ship/wake ranges retain their clip data.
 const uint64_t phase=frontRoom.animationStart==UINT64_MAX?0:scene.now-frontRoom.animationStart;
 for(const auto& old:frontRoom.uvTracks) {
  UVTrack track;
  for(auto binding:old.bindings)if(binding.vertex<indices.size() && indices[binding.vertex]!=UINT32_MAX){binding.vertex=indices[binding.vertex];track.bindings.push_back(binding);}
  if(!track.bindings.empty()){track.frames=old.frames;std::rotate(track.frames.begin(),track.frames.begin()+phase%track.frames.size(),track.frames.end());pending.uvTracks.push_back(std::move(track));}
 }
 for(const auto& old:frontRoom.meshTracks) {
  MeshTrack track;
  for(auto binding:old.bindings)if(binding.vertex<indices.size() && indices[binding.vertex]!=UINT32_MAX){binding.vertex=indices[binding.vertex];track.bindings.push_back(binding);}
  if(!track.bindings.empty()){track.frames=old.frames;std::rotate(track.frames.begin(),track.frames.begin()+phase%track.frames.size(),track.frames.end());pending.meshTracks.push_back(std::move(track));}
 }
 pending.animationStart=fieldTick;pending.textures.resize(textures.size());
 // Transfer actual GPU handles only after all validation/allocation succeeds.
 for(size_t i=0;i<textures.size();++i){pending.textures[i]=frontRoom.textures[textures[i]];frontRoom.textures[textures[i]]=Texture{};}
 GSPGPU_FlushDataCache(pending.vertices,size_t(pending.vertexCount)*sizeof(Vertex));
 departureRoom=std::move(pending);departureObjects=std::move(objects);return true;
}
void Renderer::freeOpeningDeparture(){C3D_FrameSync();freeRoom(departureRoom);departureObjects.clear();}
void Renderer::drawOpeningDeparture(const Game& game,bool translucent) {
 if(!game.openingDeparture.active || game.progress.room!=1 || !departureRoom.vertices)return;
 // Opaque pass updates shared CPU vertices once; translucent pass consumes
 // the same prepared geometry. Room1 is the opening Rogueport destination.
 // DEBUG trace cadence30 is simulation ticks, not draw frames.
 if(!translucent){const auto& scene=game.openingDeparture.scene;
  departureObjects.apply(scene.x,scene.y,scene.wakeX,scene.yaw,scene.rock,scene.parts,[&](uint32_t index,const OpeningShipObjects::Point& p){auto& v=departureRoom.vertices[index];v.x=p[0];v.y=p[1];v.z=p[2];});
  GSPGPU_FlushDataCache(departureRoom.vertices,size_t(departureRoom.vertexCount)*sizeof(Vertex));
 }
#ifdef TTYD_DEBUG_OPENING_DEPARTURE
 if(game.ticks%30==0) {
  float xmin=INFINITY,xmax=-INFINITY,ymin=INFINITY,ymax=-INFINITY;uint32_t batches=0,vertices=0;
  for(const auto& batch:departureRoom.batches)if(((batch.layer&255)==2)==translucent) {
   ++batches;vertices+=batch.count;
   for(uint32_t i=batch.start;i<batch.start+batch.count;++i){const auto& v=departureRoom.vertices[i];const auto p=Mtx_MultiplyFVecH(&worldTransform,FVec3_New(v.x,v.y,v.z));if(p.w<=0)continue;xmin=std::min(xmin,p.x/p.w);xmax=std::max(xmax,p.x/p.w);ymin=std::min(ymin,p.y/p.w);ymax=std::max(ymax,p.y/p.w);}
  }
  const auto& s=game.openingDeparture.scene;const auto& c=game.fieldCameraFrame;
  FILE* log=std::fopen("sdmc:/3ds/ttyd-2ds/opening-departure-trace.csv","a");
  if(log){std::fprintf(log,"%llu,%llu,%llu,%d,%u,%u,%zu,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",(unsigned long long)game.ticks,(unsigned long long)s.now,(unsigned long long)(s.now-s.departureAt),int(translucent),vertices,batches,departureRoom.textures.size(),s.x,xmin,xmax,ymin,ymax,c.eye.x,c.eye.y,c.eye.z,c.at.x,c.at.y,c.at.z,c.fov);std::fclose(log);}
 }
#endif
 // The field world has already published its live projection/view transform.
 drawMap(false,game,translucent,&departureRoom,(1u<<1)|(1u<<13));
}
}
