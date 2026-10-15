#include "hit_n64_texture_resources.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace ttyd;
namespace {
bool read(HitN64TextureResources& r,const std::vector<uint8_t>& b){auto* f=std::tmpfile();assert(f);assert(std::fwrite(b.data(),b.size(),1,f)==1);std::rewind(f);const bool ok=r.read(f);std::fclose(f);return ok;}
}
int main(int argc,char** argv){
    assert(!HitN64TextureResources::load("unavailable-original-hit-texture"));
    HitN64TextureResources bad;assert(!bad.read(nullptr));
    NativeHitN64Texture native;assert(!native.ready() && !native.generation() && !native.resource());
    assert(!native.load("missing-hit-meta","missing-hit-image"));assert(!native.current({},0));assert(!native.bind(0,{},0));native.release();
    const char* path=argc>1?argv[1]:"romfs/hit-n64-texture74.bin";auto* file=std::fopen(path,"rb");
    if(!file){std::cout<<"PASS unavailable native/resource admission; private original metadata skipped\n";return 0;}
    std::vector<uint8_t> b(476);assert(std::fread(b.data(),b.size(),1,file)==1 && std::fgetc(file)==EOF);std::fclose(file);
    HitN64TextureResources r;assert(read(r,b) && r.ready());assert(r.descriptor()[0]==0 && r.descriptor()[1]==128 && r.descriptor()[3]==32);
    assert(r.word(0x802fb0b8)==0x43300000 && r.word(0x80419888)==0x0d0d0d0d && r.word(0x80425458)==0x40a00000);
    assert(r.byte(0x80419888)==13 && !r.word(0x80419a40) && !r.word(0x804296e0) && !r.byte(0x802fb0d0) && !r.word(0xffffffff));
    auto actual=HitN64TextureResources::load(path);assert(actual && actual->vertices()==r.vertices());
    for(unsigned offset:{0u,8u,12u,16u,20u,55u,56u,335u,336u,384u,436u,475u}){auto changed=b;changed[offset]^=1;assert(!read(r,changed) && !r.ready());}
    auto shortFile=b;shortFile.pop_back();assert(!read(r,shortFile));auto longFile=b;longFile.push_back(0);assert(!read(r,longFile));
    assert(read(r,b));assert(!native.current(actual,1));
    std::cout<<"PASS original Hittexture74 descriptor/vertex identity, malformed companion rejection, truthful native residency\n";
}
