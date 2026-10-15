// Requires the exact shared surfaceLease publication journal.
#include "battle_class_publication.hpp"
#include "battle_class_surface_gpu.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(int argc,char**argv){
 const char*root=argc>1?argv[1]:"romfs";int images[2]{};BattleClassSurfaceTextureServices s;s.user=images;
 s.import=[](void*p,uint32_t i,const char*)->std::optional<BattleClassSurfaceImport>{return BattleClassSurfaceImport{static_cast<int*>(p)+i,256,i?256u:128u,0,1,256,i?256u:128u,false,{0,1,1,0}};};s.release=[](void*,uint32_t,const void*){};
 BattleClassSurfaceGpuResource r;assert(r.load(root,s));auto lease=r.lease();BattleClassBackend backend;backend.assets=lease.assets.get();backend.surfaceLease=lease;
 BattleClassPublication publication;auto allocation=std::make_shared<const uint8_t>(0);BattleClassPublicationKey key{allocation,0x1000,1};publication.bind(allocation,key.allocation);
 assert(publication.publish(key,backend));auto copy=publication.batch();assert(copy&&copy->surfaceLease.current()&&copy->surfaceLease.assets==lease.assets);
 backend.surfaceLease={};assert(copy->surfaceLease.current());assert(publication.admit(key));r.retire();assert(!copy->surfaceLease.current()&&!publication.consumeAdmission()&&!publication.admit(key));
 assert(r.load(root,s));auto fresh=r.lease();backend.assets=fresh.assets.get();backend.surfaceLease=lease;++key.sequence;assert(!publication.publish(key,backend));backend.surfaceLease=fresh;assert(publication.publish(key,backend)&&publication.admit(key)&&publication.consumeAdmission());r.retire();
 std::cout<<"PASS immutable class publication retains exact pair lease and rejects stale admission/consumption\n";
}
