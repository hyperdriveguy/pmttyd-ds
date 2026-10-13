#include "message_assets.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unistd.h>
using namespace ttyd;
namespace {
std::vector<uint8_t> bytes(const char* p,size_t n){return {p,p+n};}
void save(const char* path,const std::vector<uint8_t>& b){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(b.data()),b.size());assert(f.good());}
}
int main(int argc,char** argv) {
    if(argc==2) {
        MessageResource original;assert(original.load(argv[1]));assert(original.size()==328);
        for(int i=14;i<=33;++i){char key[24];std::snprintf(key,sizeof(key),"mac_0_%03d",i);assert(original.find(key));}
        std::cout<<"Original harbor:328 pairs,20 opening IDs readable with tokens retained\n";return 0;
    }
    MessageResource r;
    constexpr char valid[]="z\0Z<k><p>\0a\0A\x80\0m\0M\0\0\0";
    assert(r.assign(bytes(valid,sizeof(valid)-1)) && r.size()==3);
    assert(std::strcmp(r.find("z"),"Z<k><p>")==0 && uint8_t(r.find("a")[1])==0x80);
    assert(!r.find("missing") && !r.find("A") && !r.find(nullptr));
    const char* retained=r.find("z");
    for(auto malformed: {std::vector<uint8_t>{},bytes("key",3),bytes("a\0",2),bytes("a\0\0",3),bytes("a\0b\0",4),bytes("a\0b\0\0x",6)}) {
        assert(!r.assign(malformed) && r.find("z")==retained);
    }
    constexpr char duplicate[]="a\0one\0a\0two\0\0";
    assert(r.assign(bytes(duplicate,sizeof(duplicate)-1)) && r.size()==2 && r.ambiguous("a") && !r.find("a"));
    assert(r.assign({0,0}) && r.size()==0);
    char path[]="/tmp/pm-messages-XXXXXX";const int fd=mkstemp(path);assert(fd>=0);close(fd);
    MessageAssets slots;constexpr char global[]="same\0global\0only\0fallback\0\0";
    constexpr char room[]="same\0room\0\0";
    save(path,bytes(global,sizeof(global)-1));assert(slots.loadGlobal(path));
    save(path,bytes(room,sizeof(room)-1));assert(slots.loadRoom(path));
    assert(std::strcmp(slots.find("same"),"room")==0 && std::strcmp(slots.find("only"),"fallback")==0);
    save(path,{});assert(!slots.loadRoom(path) && std::strcmp(slots.find("same"),"room")==0);
    save(path,bytes(duplicate,sizeof(duplicate)-1));assert(slots.loadRoom(path) && !slots.find("a"));
    assert(!slots.loadRoom(nullptr));unlink(path);assert(!slots.loadGlobal(path));
    std::cout<<"message resource tests passed\n";
}
