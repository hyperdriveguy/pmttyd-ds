#include "battle_pose_pool.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
using namespace ttyd;
namespace {
std::shared_ptr<const ActorAnimationPack> fixture(bool hiddenDefault=false){
    FILE* f=std::tmpfile();assert(f);
    auto put=[&](const auto& value){assert(std::fwrite(&value,sizeof(value),1,f)==1);};
    const std::array<char,8> magic{{'P','M','A','N','I','M','1',0}};put(magic);put(uint32_t(2));
    for(unsigned index=0;index<2;++index){
        std::array<char,32> name{};std::strcpy(name.data(),index?"walk":"default");put(name);put(uint32_t(index));put(0.f);put(10.f);
        put(std::array<uint32_t,6>{{3,24,1,3,1,2}});
        put(std::array<ActorVector,3>{{{{0,0,0}},{{1,0,0}},{{0,1,0}}}});
        std::array<float,24> nodes{};nodes[3]=nodes[4]=nodes[5]=1;put(nodes);
        put(ActorGroup{-1,-1,0,0,hiddenDefault && index==0?0u:1u});
        put(std::array<ActorVertexBinding,3>{{{0,0,0,0,0},{0,1,0,1,0},{0,2,0,0,1}}});
        ActorTextureBinding texture{};texture.cells.fill(0xffff);texture.cells[0]=0;put(texture);
        put(0.f);put(std::array<uint32_t,3>{{0,0,0}});
        put(10.f);put(std::array<uint32_t,3>{{1,0,0}});put(ActorSparseUpdate{0,16,0,0,0});
    }
    std::rewind(f);auto bank=std::make_shared<ActorAnimationPack>();assert(bank->read(f));std::fclose(f);return bank;
}
std::shared_ptr<const BattlePoseResource> resource(const std::shared_ptr<const ActorAnimationPack>& bank,const std::string& model="model",const std::string& texture="texture"){
    auto r=std::make_shared<BattlePoseResource>();r->modelName=model;r->textureName=texture;r->names={"default","unconverted","walk"};r->bank=bank;return r;
}
void stateAndTime(){
    const auto bank=fixture();BattlePosePool pool;const auto r=resource(bank);
    const auto id=pool.enter(r,1,{17,100});assert(id && *id==0);auto* p=pool.pose(*id);assert(p && p->timestamp()==100);
    assert(p->word(0)==1 && p->word(4)==0 && p->word(8)==0 && p->word(0xc)==1 && p->word(0x14)==0 && p->word(0x90)==0xffffffff);
    assert(p->scalar(0x44)==1 && p->scalar(0xf8)==1 && p->word(0xf0)==0xffffffff && !p->shape.vertices.empty());
    for(unsigned row=0;row<3;++row)for(unsigned column=0;column<4;++column)assert(p->scalar(0x124+16*row+4*column)==(row==column?1:0));
    assert(pool.setRate(*id,.5f) && p->timestamp()==100 && p->word(0x14)==0);
    assert(pool.selectName(*id,"walk",false,{900,200}) && p->word(0x14)==2 && p->timestamp()==200 && p->scalar(0x44)==.5f);
    p->word(0x3c,7);p->scalar(0x40,3);p->word(0x80,9);p->scalar(0x84,.25f);
    assert(pool.selectName(*id,"walk",false,{0,300}) && p->timestamp()==200 && p->word(0x3c)==7 && p->word(0x80)==9);
    assert(pool.selectName(*id,"walk",true,{0,300}) && p->timestamp()==300 && p->word(0x3c)==0xffffffff && p->scalar(0x40)==0 && p->word(0x80)==0 && p->scalar(0x84)==0);
    assert(pool.evaluate(*id,{123,400}) && p->scalar(0x20)==3 && p->shape.selectedTime==3);
    assert(p->scalar(0x24)==3 && p->scalar(0x84)==.3f && p->word(0x28)==0 && p->word(0x2c)==1 && p->scalar(0x30)==.3f);
    assert(p->word(0x3c)==0 && p->scalar(0x40)==.3f && p->cache.positions[0][0]==0 && p->shape.positions[0][0]==.3f);
    const auto positions=p->shape.positions;assert(pool.evaluate(*id,{0,400}) && p->shape.positions==positions && p->scalar(0x20)==3);
    assert(pool.setRate(*id,2) && p->timestamp()==300 && pool.evaluate(*id,{0,400}) && p->scalar(0x20)==12 && p->shape.selectedTime==2);
    assert(p->scalar(0x24)==2 && p->scalar(0x84)==1.2f && p->scalar(0x30)==.2f);
    assert(pool.selectName(*id,"unconverted",false,{0,500}) && p->word(0x14)==1 && !pool.evaluate(*id,{0,600}) && p->word(0x14)==1);
    assert(pool.selectName(*id,"missing",false,{0,700}) && p->word(0x14)==0 && pool.evaluate(*id,{0,700}));
    assert(pool.selectIndex(*id,0x7fffffff,true,{0,800}) && p->word(0x14)==0 && p->timestamp()==800);
    const auto timestamp=p->timestamp();assert(pool.setRate(*id,-1) && p->scalar(0x44)==-1 && p->timestamp()==timestamp);
    assert(pool.setRate(*id,std::numeric_limits<float>::quiet_NaN()) && std::isnan(p->scalar(0x44)) && p->timestamp()==timestamp && !pool.evaluate(*id,{0,900}));
    assert(!pool.setRate(-1,1) && !pool.setRate(256,1));
    p->word(0xec,0xf);assert(pool.clearEffect(*id) && p->word(0xec)==0xe);
    p->word(0x90,1);assert(!pool.clearEffect(*id) && p->word(0xec)==0xe);p->word(0,3);
    assert(pool.selectIndex(*id,0,true,{0,1000}) && p->word(0)==7);p->word(0x90,0xffffffff);
    const auto field=pool.enter(r,0,{UINT64_MAX-10,55});assert(field && pool.pose(*field)->timestamp()==UINT64_MAX-10 && pool.pose(*field)->archiveType==1);
    assert(pool.evaluate(*field,{5,0}) && std::fabs(pool.pose(*field)->scalar(0x20)-.96f)<.00001f);
    const auto mode2=pool.enter(r,2,{11,22});assert(mode2 && pool.pose(*mode2)->timestamp()==22 && pool.pose(*mode2)->archiveType==0);
}
void discreteTimeline(){
    auto r=std::make_shared<BattlePoseResource>(*resource(fixture()));
    r->modelFlags=9;r->discrete={{{0,0x12345678}},{{0,5}},{{0,7},{2,11},{8,13}}};
    BattlePosePool pool;auto id=pool.enter(r,1,{});assert(id);
    assert(pool.testTranslucent(*id)==true && !pool.testTranslucent(-1));
    assert(pool.main(*id,{0,0}) && pool.pose(*id)->word(0x3c)==0 && pool.pose(*id)->scalar(0x40)==0);
    pool.outputSelector=1;assert(!pool.main(*id,{0,0}));pool.outputSelector=0;
    pool.pose(*id)->word(0,9);assert(!pool.main(*id,{0,0}));pool.pose(*id)->word(0,1);
    assert(pool.evaluate(*id,{0,0}) && pool.pose(*id)->word(0x80)==0x12345678 && pool.pose(*id)->word(0x34)==0 && pool.pose(*id)->word(0x38)==0);
    assert(pool.selectName(*id,"walk",true,{0,0}));
    assert(pool.evaluate(*id,{0,50}));auto* p=pool.pose(*id);
    assert(p->scalar(0x24)==3 && p->word(0x28)==0 && p->word(0x2c)==1);
    assert(p->word(0x34)==1 && p->word(0x38)==2 && p->word(0x80)==11);
    assert(pool.evaluate(*id,{0,150}) && p->word(0x34)==2 && p->word(0x38)==3 && p->word(0x80)==13);
    assert(pool.evaluate(*id,{0,200}) && p->scalar(0x24)==2 && p->word(0x34)==1 && p->word(0x80)==11);
    auto empty=std::make_shared<BattlePoseResource>(*r);empty->discrete[0].clear();BattlePosePool missing;
    auto absent=missing.enter(empty,1,{});assert(absent && !missing.evaluate(*absent,{0,0}));
    assert(missing.pose(*absent)->word(0x28)==0 && missing.pose(*absent)->word(0x34)==0xffffffff && missing.pose(*absent)->word(0x38)==0xffffffff);
}
void lifetimeAndCapacity(){
    const auto bank=fixture();BattlePosePool pool;const auto a=resource(bank),b=resource(bank,"other","texture");
    auto first=pool.enter(a,1,{}),second=pool.enter(a,1,{}),third=pool.enter(b,1,{});assert(first && second && third);
    assert(pool.models()[0].references==2 && pool.models()[1].references==1 && pool.textures()[0].references==2);
    assert(pool.release(*first) && pool.models()[0].references==1 && pool.textures()[0].references==2 && !pool.pose(*first) && !pool.release(*first));
    assert(pool.slot(*first)->word(0xc)==1 && pool.slot(*first)->word(0xf0)==0xffffffff);
    assert(pool.release(*second) && pool.models()[0].flags==0 && pool.textures()[0].references==1);
    auto reused=pool.enter(a,1,{0,77});assert(reused && *reused==0 && pool.models()[0].references==1 && pool.textures()[0].references==2);
    auto* p=pool.pose(*reused);p->word(0,0x41);assert(!pool.release(*reused) && pool.models()[0].references==1);p->word(0,1);
    p->word(4,1);p->word(8,1);assert(pool.release(*reused) && pool.pose(*reused) && p->word(8)==0 && pool.models()[0].references==1);
    assert(pool.release(*reused) && !pool.pose(*reused) && pool.models()[0].flags==0 && pool.textures()[0].references==1);
    assert(pool.release(*third) && pool.textures()[0].flags==0 && !pool.textures()[0].resource);
    BattlePosePool full;
    for(unsigned i=0;i<256;++i){auto id=full.enter(a,1,{});assert(id && *id==int32_t(i));}
    assert(full.models()[0].references==256 && full.textures()[0].references==1 && !full.enter(a,1,{}));
    assert(full.release(17));auto hole=full.enter(a,1,{});assert(hole && *hole==17);
    BattlePosePool models;
    for(unsigned i=0;i<64;++i)assert(models.enter(resource(bank,"model"+std::to_string(i),"texture"+std::to_string(i)),1,{}));
    assert(!models.enter(resource(bank,"extra","extra"),1,{}));
    assert(models.release(9));auto hole2=models.enter(resource(bank,"extra","extra"),1,{});assert(hole2 && *hole2==9 && models.models()[9].references==1);
    auto bad=std::make_shared<BattlePoseResource>(*a);bad->names[0]="unconverted";BattlePosePool rejected;
    assert(!rejected.enter(bad,1,{}) && !rejected.slot(0) && rejected.models()[0].flags==0 && rejected.textures()[0].references==0);
    BattlePosePool hidden;auto hiddenId=hidden.enter(resource(fixture(true)),1,{});
    assert(hiddenId && hidden.pose(*hiddenId)->shape.vertices.empty() && !hidden.pose(*hiddenId)->shape.positions.empty());
    assert(hidden.selectName(*hiddenId,"walk",true,{0,100}) && hidden.evaluate(*hiddenId,{0,100}) && !hidden.pose(*hiddenId)->shape.vertices.empty());
}
void npcLeafState(){
    const auto r=resource(fixture());BattlePosePool pool;
    const char* names[]={"tst_vivi","c_vivian","c_maririn","c_majyorin","other"};
    const uint32_t types[]={1,1,2,3,0},pointers[]={0x80415da0,0x80415da4,0x80415da8,0x80415dac,0x80415dc4};
    for(unsigned i=0;i<5;++i){const auto id=pool.enter(r,0,{},names[i]);assert(id && pool.vivianType(*id)==int32_t(types[i]) && pool.pose(*id)->word(0x120)==pointers[i]);}
    assert(pool.models()[0].references==5);auto* p=pool.pose(0);p->word(0,0x201);p->word(0xec,0x80);assert(pool.paperOn(0) && p->word(0)==0x209 && pool.materialLightFlagOn(0,3) && p->word(0xec)==0x83 && pool.vivianType(0)==1);
    assert(!pool.height(0) && !pool.radius(0) && !pool.paperOn(-1) && !pool.materialLightFlagOn(256,2) && !pool.vivianType(-1));
    p->word(0,1);assert(pool.release(0));const auto reused=pool.enter(r,0,{});assert(reused && *reused==0 && pool.vivianType(0)==0 && pool.pose(0)->word(0x120)==0x80415dc4);
}
void original(){
    auto mario=loadBattlePoseResource("romfs/trio-attack-mario.pose-directory.bin","romfs/trio-attack-mario.pmanim");
    auto partner=loadBattlePoseResource("romfs/trio-attack-goombella.pose-directory.bin","romfs/trio-attack-goombella.pmanim");
    assert(mario && partner && mario->names.size()==85 && partner->names.size()==32);
    assert(mario->modelFlags==1 && partner->modelFlags==1 && mario->discrete.size()==85 && partner->discrete.size()==32);
    assert(loadBattlePoseResource("romfs/trio-attack-mario.pose-directory.bin","romfs/trio-attack-mario.pmanim")==mario);
    BattlePosePool pool;
    for(unsigned i=0;i<3;++i){auto id=pool.enter(mario,1,{0,50});assert(id && *id==int32_t(i) && !pool.pose(*id)->shape.vertices.empty());}
    auto id=pool.enter(partner,1,{0,50});assert(id && *id==3 && pool.models()[0].references==3 && pool.models()[1].references==1 && pool.textures()[0].references==1 && pool.textures()[1].references==1);
    assert(pool.testTranslucent(0)==false && pool.testTranslucent(3)==false);
    assert(pool.selectName(0,"M_J_1D",false,{0,60}) && pool.pose(0)->word(0x14)==7 && pool.evaluate(0,{0,160}));
    assert(pool.selectName(1,"KRN_H_1",true,{0,60}) && pool.pose(1)->word(0x14)==0 && pool.evaluate(1,{0,160}));
    assert(pool.selectName(2,"M_S_3",true,{0,60}) && pool.pose(2)->word(0x14)==2 && !pool.evaluate(2,{0,160}));
}
}
int main(int argc,char**){stateAndTime();discreteTimeline();lifetimeAndCapacity();npcLeafState();if(argc>1)original();std::cout<<"battle pose pool passed\n";}
