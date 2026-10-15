#include "native_field_animation.hpp"
#include "field_party_pose.hpp"
#include "field_pose_pool.hpp"
#include "game.hpp"
#include <cassert>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
uint8_t byte(EventAddressSpace& m,uint32_t a){uint8_t v=0;assert(m.readByte(a,v));return v;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};return p;}
}
void runNativeAnimationReleaseFixture(const char* root){
 assert(loadFieldPlayerPoseAssets(root));auto resources=fieldPlayerPoseResources();
 auto cache=NativeFileCache::create();auto native=NativeFieldAnimation::create(root,cache,platform());assert(native);auto pool=native->pool();auto& m=native->process().vm().memory;
 assert(!pool->release(-1) && !pool->release(0) && !pool->release(256));
 // c_pkr poses share one MODEL, whose one TPL reference is independent.
 const auto first=pool->enter(resources[3],2,{}),second=pool->enter(resources[3],2,{});assert(first && second && *first==0 && *second==1);
 auto* p=pool->pose(*first);auto* q=pool->pose(*second);assert(p && q && p->sourceBuffers && q->sourceBuffers);
 const auto modelIndex=p->word(0x10),textureIndex=pool->models()[modelIndex].texture();
 const auto modelFile=pool->models()[modelIndex].file(),textureFile=pool->textures()[textureIndex].file();
 assert(pool->models()[modelIndex].references()==2 && pool->textures()[textureIndex].references()==1 && half(m,modelFile+2)==1 && half(m,textureFile+2)==1);
 assert(native->tick(false) && pool->selectName(*first,"PKR_W_1",true,{}) && pool->main(*first,{}));
 auto oldReader=p->sourceBuffers;const auto before=p->state;const auto oldGeneration=p->nativeGeneration;
 FieldPosePool field(pool);assert(field.release(*first));assert(!pool->pose(*first) && !p->sourceBuffers && !p->resource && p->shape.vertices.empty());
 for(unsigned i=4;i<before.size();++i)assert(p->state[i]==before[i]);
 assert(!oldReader->node(*p,0) && pool->models()[modelIndex].references()==1 && pool->textures()[textureIndex].references()==1 && byte(m,modelFile)==1 && byte(m,textureFile)==1 && !pool->release(*first));
 // A real unavailable file owner after model.flags=0 must resume raw cursor,
 // without typed re-admission, repeated decrements or premature sidecar death.
 assert(m.writeWord(NativeFileCache::Global,0));auto survivor=q->sourceBuffers;
 assert(!pool->release(*second) && native->releasePending()==0x8018a260);
 assert(!pool->pose(*second) && !pool->cacheMetadataReady() && q->word(0)==1 && q->sourceBuffers==survivor && q->resource);
 assert(pool->models()[modelIndex].references()==0 && pool->textures()[textureIndex].references()==0 && pool->textures()[textureIndex].flags()==1);
 const auto held=q->state;assert(!pool->release(*second) && q->state==held && !pool->enter(resources[3],2,{}));
 assert(!pool->release(*first) && m.writeWord(NativeFileCache::Global,NativeFileCache::Manager));
 assert(pool->release(*second) && !native->releasePending() && pool->cacheMetadataReady() && !q->sourceBuffers && !q->resource);
 assert(!pool->models()[modelIndex].flags() && !pool->textures()[textureIndex].flags() && pool->models()[modelIndex].file()==modelFile && pool->textures()[textureIndex].file()==textureFile);
 assert(byte(m,modelFile)==2 && half(m,modelFile+2)==0 && byte(m,textureFile)==2 && half(m,textureFile+2)==0);
 const auto modelPayload=word(m,word(m,modelFile+0xa0));const auto relocated=word(m,modelPayload+0x1ac);
 // Warm source acquisition reuses actual files without a second relocation.
 for(unsigned i=0;i<40;++i){const auto h=pool->enter(resources[3],2,{});assert(h && *h==0);p=pool->pose(*h);assert(p && p->nativeGeneration>oldGeneration && p->sourceBuffers && !oldReader->node(*p,0));
  assert(pool->models()[p->word(0x10)].file()==modelFile && word(m,modelPayload+0x1ac)==relocated && half(m,modelFile+2)==1 && half(m,textureFile+2)==1);
  assert(pool->selectName(*h,"PKR_W_1",true,{}) && native->tick(false) && pool->main(*h,{}) && field.release(*h) && pool->cacheMetadataReady());
 }
 // Distinct MODEL banks a_mario/e_mario share TPL by model, not by pose.
 const auto front=pool->enter(resources[0],2,{}),effect=pool->enter(resources[2],2,{});assert(front && effect);
 const auto frontModel=pool->pose(*front)->word(0x10),effectModel=pool->pose(*effect)->word(0x10);assert(frontModel!=effectModel);
 const auto sharedTexture=pool->models()[frontModel].texture();assert(sharedTexture==pool->models()[effectModel].texture() && pool->textures()[sharedTexture].references()==2);
 const auto sharedFile=pool->textures()[sharedTexture].file();assert(pool->release(*front) && pool->textures()[sharedTexture].references()==1 && byte(m,sharedFile)==1);
 assert(pool->release(*effect) && !pool->textures()[sharedTexture].flags() && byte(m,sharedFile)==2 && pool->cacheMetadataReady());
 // Borrowed early return preserves the source lifetime and both readers.
 const auto h=pool->enter(resources[3],2,{});assert(h);p=pool->pose(*h);auto live=p->sourceBuffers;p->word(4,1);p->word(8,1);
 assert(pool->release(*h) && pool->pose(*h)==p && p->word(8)==0 && p->sourceBuffers==live);
 assert(pool->release(*h) && !pool->pose(*h) && p->word(8)==0xffffffff && !p->sourceBuffers);
 // Detached copies do not inherit the production release callback or file owner.
 const auto copiedHandle=pool->enter(resources[3],2,{});assert(copiedHandle);BattlePosePool copied(*pool);assert(copied.release(*copiedHandle) && pool->pose(*copiedHandle));assert(pool->release(*copiedHandle));
}
void runNativeAnimationSmartReleaseHold(const char* root){
 auto native=NativeFieldAnimation::create(root,NativeFileCache::create(),platform());assert(native);auto pool=native->pool();const auto h=pool->enter(fieldPlayerPoseResources()[3],2,{});assert(h);auto* p=pool->pose(*h);p->word(0,p->word(0)|0x40);p->word(0x15c,0x81234560);
 assert(!pool->release(*h) && native->releasePending()==0x8002f878 && p->word(0)==0x41 && p->sourceBuffers && pool->models()[p->word(0x10)].references()==1);
 // Changing flags cannot turn an already captured real child into a noop.
 p->word(0,1);assert(!pool->release(*h) && native->releasePending()==0x8002f878 && p->sourceBuffers);
}
void runNativeAnimationBattleRetirement(const char* root){
 assert(loadProfessorStoryAssets(root) && loadUndergroundStoryAssets(root) && loadTrioBattleAssets(root) && loadTrioAttackAssets(root));
 auto game=std::make_unique<Game>(false);assert(game->initializeNativeFieldAnimation(root,fieldAnimationFileCache(),platform()));
 auto& parties=const_cast<FieldPartyRegistry&>(game->fieldPartyState());const auto native=parties.nativeAnimation;const auto pool=native->pool();auto& m=native->process().vm().memory;
 ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);
 // Original encounter fixture, not a claim to have run the full field story.
 assert(game->trioAttack.begin(menu,scene,10,{},nullptr,nullptr,&parties));assert(game->trioAttack.battlePosePool()==pool.get());
 std::vector<std::pair<uint32_t,uint32_t>> files;
 for(unsigned i=3;i<=10;++i){const auto* p=pool->pose(int32_t(i));assert(p && p->sourceBuffers && p->word(0xc)==1);const auto& model=pool->models()[p->word(0x10)];files.emplace_back(model.file(),pool->textures()[model.texture()].file());}
 for(unsigned i=7;i<=9;++i)assert(pool->pose(int32_t(i))->resource->modelName=="c_kuribo");
 auto escaped=pool->pose(7)->sourceBuffers;const auto generation=pool->pose(7)->nativeGeneration;
 // Production destructor dispatches real source Release through the shared
 // namespace. Mario's three ordinary owners and their texture references live.
 game->trioAttack=TrioAttack{};for(unsigned i=3;i<=10;++i)assert(!pool->pose(int32_t(i)) && !pool->slot(i)->sourceBuffers);
 assert(pool->cacheMetadataReady() && pool->pose(0) && pool->pose(1) && pool->pose(2) && !native->releasePending());
 for(auto pair:files){assert(byte(m,pair.first)==2 && half(m,pair.first+2)==0);}
 // Enemy warm re-entry gives a new lifetime; the old reader cannot adopt it.
 const auto resource=loadBattlePoseResource((std::string(root)+"/trio-enemy-kuribo.pose-directory.bin").c_str(),(std::string(root)+"/trio-enemy-kuribo.pmanim").c_str());assert(resource);
 const auto h=pool->enter(resource,1,{});assert(h && *h==3 && pool->pose(*h)->nativeGeneration>generation && !escaped->node(*pool->pose(*h),0));
 assert(pool->selectName(*h,"KUR_S_1",true,{}) && pool->main(*h,{}) && pool->release(*h) && native->tick(false));
 game->tick({});assert(game->nativeMarioPose() && game->nativeMarioPose()->sourceBuffers);
}
int main(){runNativeAnimationReleaseFixture("romfs");runNativeAnimationSmartReleaseHold("romfs");runNativeAnimationBattleRetirement("romfs");}
