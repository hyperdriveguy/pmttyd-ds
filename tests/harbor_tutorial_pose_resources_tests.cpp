#include "harbor_tutorial_pose_resources.hpp"
#include "native_file_cache.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(int argc,char** argv){
    assert(argc==2);const std::string root=std::string(argv[1])+"/";
    assert(!HarborTutorialPoseResources::load(nullptr));assert(!HarborTutorialPoseResources::admit({},{},{}));
    auto files=NativeFileCache::create();assert(files && stagePrologueAnimationFiles(*files,root.c_str()));
    assert(files->contains("a/c_kanbu",5) && files->contains("a/c_kanbu-",4));
    const auto resources=HarborTutorialPoseResources::load(root.c_str());assert(resources);
    const auto mario=resources->find("b_mario"), crump=resources->find("c_kanbu"), helper=resources->find("c_pkr");
    assert(mario && crump && helper && !resources->find("c_kanbu_ibt") && !resources->find(""));
    assert(crump->names.size()==20 && crump->names.front()=="Z_1");
    assert(loadBattlePoseResource((root+"trio-attack-mario.pose-directory.bin").c_str(),(root+"trio-attack-mario.pmanim").c_str())==mario);
    assert(loadBattlePoseResource((root+"trio-attack-goombella.pose-directory.bin").c_str(),(root+"trio-attack-goombella.pmanim").c_str())==helper);
    auto bad=std::make_shared<BattlePoseResource>(*crump);bad->draw.reset();assert(!HarborTutorialPoseResources::admit(mario,bad,helper));
    bad=std::make_shared<BattlePoseResource>(*crump);bad->textures.reset();assert(!HarborTutorialPoseResources::admit(mario,bad,helper));
    bad=std::make_shared<BattlePoseResource>(*crump);bad->names[0]="S_1";assert(!HarborTutorialPoseResources::admit(mario,bad,helper));
    bad=std::make_shared<BattlePoseResource>(*crump);bad->modelFlags.reset();assert(!HarborTutorialPoseResources::admit(mario,bad,helper));
    BattlePosePool pool;std::vector<int32_t> handles;
    // Detached real CPU pose allocations, not fabricated entry indices. The
    // production process still must admit its source constructor/heap children.
    for(const char* model:{"b_mario","b_mario","b_mario","c_kanbu","c_pkr"}){
        auto resource=resources->find(model);auto handle=pool.enter(resource,1,{0,0});assert(handle);
        for(auto previous:handles)assert(previous!=*handle);
        handles.push_back(*handle);
        const auto* pose=pool.pose(*handle);assert(pose && pose->resource==resource && pose->word(0x14)==0);
        assert(pool.evaluate(*handle,{0,0}));
    }
    for(const char* name:{"S_1","R_2","R_1","A_1A","A_1B","A_1C","D_1"}){
        assert(pool.selectName(handles[3],name,false,{0,0}));assert(pool.evaluate(handles[3],{0,0}));
    }
    assert(!crump->bank->pose("unsupported-pose"));
    std::cout<<"PASS actual three Mario/Crump/helper CPU resources, distinct pose allocations, source descriptor0 and combat coverage\n";
}
