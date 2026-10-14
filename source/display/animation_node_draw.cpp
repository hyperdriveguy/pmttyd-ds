#include "animation_node_draw.hpp"
#include <cassert>
namespace ttyd {
bool resumeAnimationNodeDraw(const std::array<float,24>& n,bool joint,const std::array<float,3>& parent,std::vector<BattleActMatrix>& stack,void* user,bool (*matrix)(void*,BattleAudienceMatrixRequest&),AnimationNodeCursor& c){
    if(stack.empty())return false;
    if(!c.active){
        c.active=true;c.requestPending=false;c.step=0;c.pending=0;c.count=0;
        auto op=[&](uint32_t fn,unsigned target,unsigned left=0,unsigned right=0){AnimationNodeOperation r;r.function=fn;r.target=target;r.left=left;r.right=right;assert(c.count<c.operations.size());c.operations[c.count++]=r;};
        auto translation=[&](unsigned target,std::array<float,3> v){op(0x8029e230,target);c.operations[c.count-1].xyz=v;};
        auto scale=[&](unsigned target,std::array<float,3> v){op(0x8029e2b0,target);c.operations[c.count-1].xyz=v;};
        auto concat=[&](unsigned target,unsigned left,unsigned right){op(0x8029dd64,target,left,right);};
        auto triple=[&](unsigned first){return std::array<float,3>{n[first],n[first+1],n[first+2]};};
        auto nonzero=[](const std::array<float,3>& v){return v[0]!=0 || v[1]!=0 || v[2]!=0;};
        auto jointGate=[](const std::array<float,3>& v){return v[0]!=1 || v[1]!=1 || v[2]==1;};
        const auto t=triple(0),s=triple(3);bool haveT=nonzero(t),haveS=false,haveQ=false,haveR=false;
        if(haveT)translation(0,t);
        if(joint && jointGate(parent)){
            std::array<float,3> q{1,1,1};if(parent[0]!=0 && parent[1]!=0 && parent[2]!=0)for(unsigned i=0;i<3;++i)q[i]=1.f/parent[i];scale(2,q);haveQ=true;
        }
        haveS=joint?jointGate(s):s[0]!=1 || s[1]!=1 || s[2]!=1;
        if(haveS)scale(1,s);
        auto pivot=[&](unsigned component,unsigned first,unsigned offset,bool& present){
            const auto p=triple(first),d=triple(offset);if(!nonzero(p))return;
            if(present){translation(4,{-p[0],-p[1],-p[2]});translation(5,p);translation(6,d);concat(7,6,5);concat(8,7,component);concat(component,8,4);}
            else if(nonzero(d)){translation(component,d);present=true;}
        };
        if(!joint)pivot(1,15,21,haveS);
        op(0x8029dd04,3); // Always, including zero rotations.
        auto rotations=[&](unsigned first,float multiplier){for(int axis=2;axis>=0;--axis){const float angle=multiplier==1?n[first+axis]:n[first+axis]*multiplier;if(angle!=0){op(0x8029dff0,10);c.operations[c.count-1].axis=120+axis;c.operations[c.count-1].radians=angle*.01745329238474369f;concat(3,3,10);haveR=true;}}};
        if(joint)rotations(9,1);
        rotations(6,2);
        if(!joint)pivot(3,12,18,haveR);
        op(0,0); // Publish incremented modeling level BEFORE mask dispatch.
        std::array<unsigned,4> components{};unsigned componentCount=0;if(haveT)components[componentCount++]=0;if(haveQ)components[componentCount++]=2;if(haveR)components[componentCount++]=3;if(haveS)components[componentCount++]=1;
        unsigned result=9;
        if(!componentCount)op(0x8029dd04,result);
        else {result=components[0];for(size_t i=1;i<componentCount;++i){concat(9,result,components[i]);result=9;}}
        op(0x8029dd64,11,12,result); //12 denotes the freshly published previous stack matrix.
    }
    while(c.step<c.count){
        const auto& op=c.operations[c.step];
        if(!op.function){stack.emplace_back();++c.step;continue;}
        if(!c.requestPending){c.request={};c.request.function=op.function;c.request.axis=op.axis;c.request.radians=op.radians;c.request.xyz=op.xyz;
            if(op.function==0x8029dd64){c.request.left=op.left==12?stack[stack.size()-2]:c.matrices[op.left];c.request.right=c.matrices[op.right];}
            c.requestPending=true;
        }
        c.pending=c.request.function;if(!matrix || !matrix(user,c.request))return false;
        c.matrices[op.target]=c.request.output;c.requestPending=false;++c.step;
    }
    stack.back()=c.matrices[11];c.active=false;c.pending=0;return true;
}
}
