#include "opening_harbor_jump.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main(){
    OpeningHarborJump jump;assert(jump.begin({0,0,0},{0,0,0},1800,100,0x80004));
    assert(jump.flags==4 && jump.active && jump.coefficient==-400 && jump.linear==400);
    assert(jump.advance(900)==0 && jump.position[1]==100);
    assert(jump.advance(1799)==0 && jump.active && jump.position[1]>0);
    jump.flags|=0x40;assert(jump.advance(1800)==2 && !jump.active && jump.flags==0x80044);
    assert((jump.position==OpeningHarborJump::Point{0,0,0}));assert(jump.advance(2100)==2);
    assert(jump.begin({100,20,-30},{-50,80,30},1800,100,0x24));jump.advance(900);
    assert((jump.position==OpeningHarborJump::Point{25,150,0}));
    OpeningHarborJump sparse,dense;assert(sparse.begin({12,7,30},{-19,1,2},1800,100));dense=sparse;
    sparse.advance(1350);for(unsigned i=0;i<=1350;++i)dense.advance(i);
    assert(sparse.position==dense.position);
    assert(jump.begin({1,2,3},{4,5,6},0,100));assert(jump.advance(0)==2 && jump.position==jump.target);
    assert(!jump.begin({NAN,0,0},{0,0,0},1800,100));
}
