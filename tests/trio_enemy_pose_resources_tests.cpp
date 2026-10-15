#include "trio_enemy_pose_resources.hpp"
#include "animation_texture.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>
using namespace ttyd;
int main(int argc,char** argv){
    assert(!TrioEnemyPoseResources::admit({}));
    assert(!TrioEnemyPoseResources::load("missing-enemy-directory","missing-enemy-pack"));
    assert(!TrioEnemyPoseResources::admit(std::make_shared<BattlePoseResource>()));
    const std::string prefix=argc>1?argv[1]:"romfs/trio-enemy-kuribo";
    FILE* file=std::fopen((prefix+".pose-directory.bin").c_str(),"rb");
    if(!file){std::cout<<"PASS unavailable enemy resources reject; local original resource checks skipped\n";return 0;}
    std::fclose(file);
    const auto bank=TrioEnemyPoseResources::load((prefix+".pose-directory.bin").c_str(),(prefix+".pmanim").c_str());assert(bank);
    const auto& r=bank->resource();assert(r->names.size()==45 && r->names.front()=="KUR_Z_2" && r->textures->count()==14);
    for(const char* name:{"KUR_S_1","TGK_S_1","PTK_S_1"})assert(bank->find("c_kuribo",name)==r);
    assert(!bank->find("c_kuribo_n","KUR_S_1") && !bank->find("c_kuribo","S_1") && !bank->find("c_kuribo",""));
    auto bad=std::make_shared<BattlePoseResource>(*r);bad->draw.reset();assert(!TrioEnemyPoseResources::admit(bad));
    bad=std::make_shared<BattlePoseResource>(*r);bad->textures.reset();assert(!TrioEnemyPoseResources::admit(bad));
    bad=std::make_shared<BattlePoseResource>(*r);bad->names[0]="KUR_S_1";assert(!TrioEnemyPoseResources::admit(bad));
    bad=std::make_shared<BattlePoseResource>(*r);bad->modelFlags.reset();assert(!TrioEnemyPoseResources::admit(bad));
    bad=std::make_shared<BattlePoseResource>(*r);bad->discrete.pop_back();assert(!TrioEnemyPoseResources::admit(bad));
    BattlePosePool pool;
    for(const char* name:{"KUR_S_1","TGK_S_1","PTK_S_1"}){
        const auto handle=pool.enter(bank->find("c_kuribo",name),1,{0,0});assert(handle);
        assert(pool.selectName(*handle,name,false,{0,0}));const auto* p=pool.pose(*handle);assert(p && r->names[p->word(0x14)]==name && p->resource==r);
        // Detached pose evaluation is CPU proof, never sourceShape/GPU admission.
        assert(pool.evaluate(*handle,{0,0}));
    }
    std::cout<<"PASS original45-descriptor enemy bank, shared model/three initial poses and missing/mismatched metadata rejection\n";
}
