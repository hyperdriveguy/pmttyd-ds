#include "platform.hpp"
#include <algorithm>
#include <cstring>
namespace ttyd {
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
 if(!translucent){const auto& scene=game.openingDeparture.scene;
  departureObjects.apply(scene.x,scene.y,scene.wakeX,scene.yaw,scene.rock,scene.parts,[&](uint32_t index,const OpeningShipObjects::Point& p){auto& v=departureRoom.vertices[index];v.x=p[0];v.y=p[1];v.z=p[2];});
  GSPGPU_FlushDataCache(departureRoom.vertices,size_t(departureRoom.vertexCount)*sizeof(Vertex));
 }
 // The field world has already published its live projection/view transform.
 drawMap(false,game,translucent,&departureRoom,(1u<<1)|(1u<<13));
}
}
