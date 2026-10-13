#pragma once
#include "actor_mesh.hpp"
#include "actor_surface.hpp"
#include <algorithm>
#include <functional>
#include <limits>
namespace ttyd {
// Portable ordinary domain. Host/ARM float trig and affine composition adapt
// the recovered branches; original paired-single/libm bit equality is separate.
using ActorVector=std::array<float,3>;
struct ActorMatrix {
    std::array<float,16> v{{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}};
    static ActorMatrix translation(const float* p) {ActorMatrix m;for(int i=0;i<3;++i)m.v[i*4+3]=p[i];return m;}
    static ActorMatrix scale(const float* p) {ActorMatrix m;for(int i=0;i<3;++i)m.v[i*4+i]=p[i];return m;}
    ActorMatrix operator*(const ActorMatrix& b) const {
        ActorMatrix out;
        for(int r=0;r<3;++r)for(int c=0;c<4;++c){float sum=0;for(int k=0;k<4;++k)sum+=v[r*4+k]*b.v[k*4+c];out.v[r*4+c]=sum;}
        return out;
    }
    ActorVector point(const ActorVector& p) const {
        ActorVector out{};for(int r=0;r<3;++r)out[r]=v[r*4]*p[0]+v[r*4+1]*p[1]+v[r*4+2]*p[2]+v[r*4+3];return out;
    }
};
inline bool actorNonzero(const float* v){return v[0]!=0 || v[1]!=0 || v[2]!=0;}
inline bool actorJointScaleGate(const float* v){return v[0]!=1 || v[1]!=1 || v[2]==1;}
inline ActorMatrix actorRotation(const float* p,const float* secondary=nullptr) {
    ActorMatrix result;
    auto triple=[&](const float* angles,float multiplier){
        for(int axis=2;axis>=0;--axis)if(angles[axis]!=0) {
            const float radians=(angles[axis]*multiplier)*.01745329238474369f;
            const float s=std::sin(radians),c=std::cos(radians);
            ActorMatrix m;const int a=(axis+1)%3,b=(axis+2)%3;
            m.v[a*4+a]=m.v[b*4+b]=c;m.v[a*4+b]=-s;m.v[b*4+a]=s;result=result*m;
        }
    };
    if(secondary)triple(secondary,1);
    triple(p,2);return result;
}
inline ActorMatrix actorNodeMatrix(const float* n,bool joint,const float* parent) {
    ActorMatrix result;bool have=false;
    auto append=[&](const ActorMatrix& m){if(have)result=result*m;else{result=m;have=true;}};
    if(actorNonzero(n))append(ActorMatrix::translation(n));
    if(joint) {
        if(actorJointScaleGate(parent)) {
            float inverse[3]={1,1,1};
            if(parent[0]!=0 && parent[1]!=0 && parent[2]!=0)for(int i=0;i<3;++i)inverse[i]=1/parent[i];
            append(ActorMatrix::scale(inverse));
        }
        if(actorNonzero(n+6) || actorNonzero(n+9))append(actorRotation(n+6,n+9));
        if(actorJointScaleGate(n+3))append(ActorMatrix::scale(n+3));
    } else {
        ActorMatrix rotation=actorRotation(n+6),scale=ActorMatrix::scale(n+3);
        bool hasRotation=actorNonzero(n+6),hasScale=n[3]!=1 || n[4]!=1 || n[5]!=1;
        auto pivot=[&](ActorMatrix& m,bool& present,const float* p,const float* offset){
            if(!actorNonzero(p))return;
            if(present) {
                const float negative[]={-p[0],-p[1],-p[2]};
                m=((ActorMatrix::translation(offset)*ActorMatrix::translation(p))*m)*ActorMatrix::translation(negative);
            } else if(actorNonzero(offset)){m=ActorMatrix::translation(offset);present=true;}
        };
        pivot(rotation,hasRotation,n+12,n+18);pivot(scale,hasScale,n+15,n+21);
        if(hasRotation)append(rotation);
        if(hasScale)append(scale);
    }
    return result;
}
struct ActorGroup {int32_t sibling,child;uint32_t node,joint,visible;}; // v1/2 boolean; v3 visibility-buffer index.
struct ActorVertexBinding {uint32_t group,position,texture;float u,v;};
struct ActorTextureBinding {uint32_t initial;std::array<uint16_t,256> cells;};
struct ActorSparseUpdate {uint32_t index;int8_t a,b,c;uint8_t padding;};
struct ActorTextureUpdate {uint32_t index;uint8_t delta,padding[3];};
struct ActorTextureShift {float s=0,t=0;};
struct ActorAnimationKey {
    float time=0;std::vector<ActorSparseUpdate> positions,normals,nodes;std::vector<ActorTextureUpdate> textures;
    std::vector<ActorTextureUpdate> visibility; // Original byte deltas, full keys only.
    std::vector<ActorTextureShift> shifts; // PMANIM4, paired with texture records.
};
struct ActorAnimationSample {
    std::vector<ActorVector> positions,normals;
    std::vector<float> nodes;
    std::vector<uint8_t> textures;
    std::vector<uint8_t> visibility;
    std::vector<ActorMeshVertex> vertices;
    std::vector<ActorColor> colors;
    std::vector<ActorSurfaceState> surfaces;
    std::vector<ActorSurfaceBatch> batches;
    float selectedTime=0,fraction=0;size_t last=0,next=0;
    // Retained hierarchy workspace avoids per-draw allocations on ARM11.
    std::vector<ActorMatrix> world;
    std::vector<uint8_t> seen,active;
};
inline float actorNodeDelta(const ActorSparseUpdate& record,float q,float duration) {
    if(record.c<=-89 || record.c>=89)return 0;
    const float q2=q*q,q3=q*q2;
    // Source table stores rounded tan of a rounded angle using word3c8efa33.
    static const auto table=[](){
        std::array<float,178> values{};
        for(int i=0;i<178;++i)values[i]=float(std::tan(double(float(i-89)*.017453288659453392f)));
        return values;
    }();
    const float tin=table[size_t(int(record.b)+89)],tout=table[size_t(int(record.c)+89)];
    const float first=(q3-2*q2+q)*(duration*tin);
    const float sum=std::fma(3*q2-2*q3,float(record.a)*.0625f,first);
    return std::fma(q3-q2,duration*tout,sum);
}
struct ActorAnimationPose {
    std::string name;uint32_t loop=0;float start=0,end=0;
    std::vector<ActorVector> positions,normals;std::vector<float> nodes;
    std::vector<ActorGroup> groups;std::vector<ActorVertexBinding> bindings;
    std::vector<ActorTextureBinding> textures;std::vector<ActorAnimationKey> keys;
    std::vector<uint8_t> visibility; // PMANIM3 bases; Group.visible becomes an index.
    std::vector<ActorSurfaceState> surfaces; // PMANIM4 original texture matrices/wraps.
    std::vector<ActorColor> colors; // One original RGBA per binding.
    std::vector<ActorSurfaceTriangle> triangles;
    bool evaluate(float time,ActorAnimationSample& out) const {
        if(!std::isfinite(time) || keys.empty() || groups.empty())return false;
        const float length=end-start;
        float selected=start;
        if(length!=0 && time>=start)selected=loop?time-std::trunc((time-start)/length)*length:std::min(time,end);
        size_t last=0;while(last+1<keys.size() && keys[last+1].time<=selected)++last;
        size_t next=last+1;if(next==keys.size())next=loop?0:last;
        const float span=last==next?0:(next==0?end-keys[last].time:keys[next].time-keys[last].time);
        const float q=span==0?0:(selected-keys[last].time)/span;
        out.positions=positions;out.normals=normals;out.nodes=nodes;out.visibility=visibility;out.textures.resize(textures.size());
        out.surfaces=surfaces;
        for(size_t i=0;i<textures.size();++i)out.textures[i]=uint8_t(textures[i].initial);
        for(size_t i=0;i<=last;++i) {
            for(const auto& r:keys[i].positions) {
                auto& p=out.positions[r.index];p[0]+=r.a*.0625f;p[1]+=r.b*.0625f;p[2]+=r.c*.0625f;
            }
            for(const auto& r:keys[i].normals) {
                auto& n=out.normals[r.index];n[0]+=r.a*.0625f;n[1]+=r.b*.0625f;n[2]+=r.c*.0625f;
            }
            for(const auto& r:keys[i].nodes)out.nodes[r.index]+=r.a*.0625f;
            for(const auto& r:keys[i].textures)out.textures[r.index]=uint8_t(out.textures[r.index]+r.delta);
            for(size_t j=0;j<keys[i].shifts.size();++j) {
                auto& state=out.surfaces[keys[i].textures[j].index].transform;
                state[0]+=keys[i].shifts[j].s;state[1]+=keys[i].shifts[j].t;
            }
            for(const auto& r:keys[i].visibility)out.visibility[r.index]=uint8_t(out.visibility[r.index]+r.delta);
        }
        if(last<next) {
            for(const auto& r:keys[next].positions) {
                auto& p=out.positions[r.index];const float factor=q*.0625f;
                p[0]=std::fma(float(r.a),factor,p[0]);p[1]=std::fma(float(r.b),factor,p[1]);p[2]=std::fma(float(r.c),factor,p[2]);
            }
            for(const auto& r:keys[next].normals) {
                auto& n=out.normals[r.index];const float factor=q*.0625f;
                n[0]=std::fma(float(r.a),factor,n[0]);n[1]=std::fma(float(r.b),factor,n[1]);n[2]=std::fma(float(r.c),factor,n[2]);
            }
            for(const auto& r:keys[next].nodes)out.nodes[r.index]+=actorNodeDelta(r,q,keys[next].time-keys[last].time);
        }
        out.selectedTime=selected;out.last=last;out.next=next;out.fraction=q;
        auto& world=out.world;world.assign(groups.size(),ActorMatrix{});
        auto& seen=out.seen;seen.assign(groups.size(),0);
        auto& active=out.active;active.assign(groups.size(),0);
        auto walk=[&](auto&& self,int32_t index,const ActorMatrix& incoming,int32_t parent,bool admitted)->bool {
            if(index==-1)return true;
            if(index<0 || size_t(index)>=groups.size() || seen[index])return false;
            seen[index]=1;const auto& g=groups[index];
            const bool visible=visibility.empty()?g.visible!=0:out.visibility[g.visible]!=0;
            active[index]=admitted && visible;
            if(active[index]) {
                const float ones[]={1,1,1};const float* p=ones;
                if(g.joint && parent>=0 && groups[parent].joint)p=out.nodes.data()+groups[parent].node+3;
                world[index]=incoming*actorNodeMatrix(out.nodes.data()+g.node,g.joint,p);
            }
            // Validate the complete graph even when hidden; descendants produce
            // no vertices/matrix work until the ancestor is visible again.
            if(!self(self,g.child,world[index],index,active[index]))return false;
            return self(self,g.sibling,incoming,index,admitted);
        };
        if(!walk(walk,int32_t(groups.size()-1),ActorMatrix{},-1,true))return false;
        out.vertices.clear();out.vertices.reserve(bindings.size());out.colors.clear();out.batches.clear();
        if(!surfaces.empty()) {
            if(colors.size()!=bindings.size() || triangles.size()*3!=bindings.size())return false;
            for(size_t index=0;index<bindings.size();index+=3) {
                const auto& first=bindings[index];if(!seen[first.group])return false;if(!active[first.group])continue;
                const uint16_t cell=textures[first.texture].cells[out.textures[first.texture]];if(cell>=64)return false;
                std::array<ActorSurfaceVertex,3> triangle;
                for(unsigned i=0;i<3;++i) {
                    const auto& b=bindings[index+i];const auto p=world[b.group].point(out.positions[b.position]);
                    const auto uv=actorTexturePoint(out.surfaces[b.texture].transform,b.u,b.v);
                    triangle[i]={{p[0],p[1],p[2],uv[0],uv[1]},colors[index+i]};
                }
                const uint32_t start=uint32_t(out.vertices.size());
                if(!appendActorSurface(triangle,out.surfaces[first.texture],cell,out.vertices,out.colors))return false;
                const uint32_t count=uint32_t(out.vertices.size())-start;
                if(!count)continue;
                const auto& draw=triangles[index/3];
                if(!out.batches.empty() && out.batches.back().pass==draw.pass && out.batches.back().cull==draw.cull)out.batches.back().count+=count;
                else out.batches.push_back({start,count,draw.pass,draw.cull});
            }
            return true;
        }
        for(const auto& b:bindings) {
            if(!seen[b.group])return false;
            if(!active[b.group])continue;
            const uint16_t cell=textures[b.texture].cells[out.textures[b.texture]];if(cell>=64)return false;
            const auto p=world[b.group].point(out.positions[b.position]);
            out.vertices.push_back({p[0],p[1],p[2],(float(cell%8)*128+2+b.u*124)/1024,
                                    1-(float(cell/8)*128+2+b.v*124)/1024});
        }
        return true;
    }
};
class ActorAnimationPack {
public:
    const ActorAnimationPose* pose(const char* name) const {for(const auto& p:poses)if(p.name==name)return &p;return nullptr;}
    bool load(const char* path){FILE* f=std::fopen(path,"rb");if(!f)return false;bool ok=read(f);std::fclose(f);return ok;}
    bool read(FILE* file) {
        auto scalar=[&](auto& value){return std::fread(&value,sizeof(value),1,file)==1;};
        auto vector=[&](auto& values,uint32_t count){values.resize(count);return count==0 || std::fread(values.data(),sizeof(values[0]),count,file)==count;};
        std::array<char,8> magic{};uint32_t count=0;
        if(!scalar(magic))return false;
        const bool version4=std::memcmp(magic.data(),"PMANIM4",8)==0;
        const bool version3=version4 || std::memcmp(magic.data(),"PMANIM3",8)==0;
        const bool version2=version3 || std::memcmp(magic.data(),"PMANIM2",8)==0;
        if((!version2 && std::memcmp(magic.data(),"PMANIM1",8)) || !scalar(count) || !count || count>64)return false;
        std::vector<ActorAnimationPose> pending;
        for(uint32_t poseIndex=0;poseIndex<count;++poseIndex) {
            ActorAnimationPose p;std::array<char,32> name{};std::array<uint32_t,6> sizes{};uint32_t normalCount=0,visibilityCount=0;
            if(!scalar(name) || !name[0] || !std::memchr(name.data(),0,name.size()) || !scalar(p.loop) ||
               !scalar(p.start) || !scalar(p.end) || !scalar(sizes) || (version2 && !scalar(normalCount)) || (version3 && !scalar(visibilityCount)))return false;
            p.name=name.data();for(const auto& prior:pending)if(prior.name==p.name)return false;
            if(p.loop>1 || !std::isfinite(p.start) || !std::isfinite(p.end) || (!version4 && p.start!=0) || p.end<p.start ||
               !sizes[0] || sizes[0]>8192 || !sizes[1] || sizes[1]>8192 || sizes[1]%24 ||
               !sizes[2] || sizes[2]>128 || !sizes[3] || sizes[3]>8192 || sizes[3]%3 ||
               !sizes[4] || sizes[4]>256 || !sizes[5] || sizes[5]>1024 || normalCount>8192 || visibilityCount>8192 || (version3 && !visibilityCount))return false;
            if(!vector(p.positions,sizes[0]) || !vector(p.normals,normalCount) || !vector(p.nodes,sizes[1]) || !vector(p.visibility,visibilityCount) || !vector(p.groups,sizes[2]) ||
               !vector(p.bindings,sizes[3]) || !vector(p.textures,sizes[4]))return false;
            if(version4) {
                if(!vector(p.surfaces,sizes[4]))return false;
                p.colors.resize(sizes[3]);
                for(auto& color:p.colors) {
                    std::array<uint8_t,4> rgba{};if(!scalar(rgba))return false;
                    for(unsigned i=0;i<4;++i)color[i]=float(rgba[i])/255;
                }
                if(!vector(p.triangles,sizes[3]/3))return false;
                for(const auto& state:p.surfaces) {
                    if(state.wrapS>2 || state.wrapT>2)return false;
                    for(float v:state.transform)if(!std::isfinite(v))return false;
                }
                for(const auto& t:p.triangles)if((t.pass!=2 && t.pass!=3) || t.cull>3)return false;
                for(size_t i=0;i<p.bindings.size();i+=3)for(unsigned j=1;j<3;++j)
                    if(p.bindings[i].group!=p.bindings[i+j].group || p.bindings[i].texture!=p.bindings[i+j].texture)return false;
            }
            for(const auto& v:p.positions)for(float x:v)if(!std::isfinite(x))return false;
            for(const auto& v:p.normals)for(float x:v)if(!std::isfinite(x))return false;
            for(float x:p.nodes)if(!std::isfinite(x))return false;
            for(const auto& g:p.groups)if(g.sibling<-1 || g.child<-1 || g.sibling>=int32_t(sizes[2]) ||
                g.child>=int32_t(sizes[2]) || g.node>sizes[1]-24 || g.node%24 || g.joint>1 || (version3?g.visible>=visibilityCount:g.visible>1))return false;
            for(const auto& b:p.bindings)if(b.group>=sizes[2] || b.position>=sizes[0] || b.texture>=sizes[4] ||
                !std::isfinite(b.u) || !std::isfinite(b.v) || (version4?(std::fabs(b.u)>128 || std::fabs(b.v)>128):b.u<0 || b.u>1 || b.v<0 || b.v>1))return false;
            for(const auto& t:p.textures){if(t.initial>255)return false;for(auto cell:t.cells)if(cell>=64 && cell!=0xffff)return false;}
            uint32_t total=0;
            for(uint32_t key=0;key<sizes[5];++key) {
                ActorAnimationKey k;std::array<uint32_t,3> records{};uint32_t normalRecords=0,visibilityRecords=0;
                if(!scalar(k.time) || !scalar(records) || (version2 && !scalar(normalRecords)) || (version3 && !scalar(visibilityRecords)) || !std::isfinite(k.time) ||
                   (key==0?k.time!=p.start:k.time<=p.keys.back().time) || k.time>p.end)return false;
                if(normalRecords>16384 || total>1000000-normalRecords)return false;
                total+=normalRecords;
                if(visibilityRecords>16384 || total>1000000-visibilityRecords)return false;
                total+=visibilityRecords;
                for(auto n:records){if(n>16384 || total>1000000-n)return false;total+=n;}
                if(!vector(k.positions,records[0]) || !vector(k.normals,normalRecords) || !vector(k.nodes,records[1]) || !vector(k.textures,records[2]) || !vector(k.visibility,visibilityRecords))return false;
                if(version4 && !vector(k.shifts,records[2]))return false;
                for(const auto& r:k.shifts)if(!std::isfinite(r.s) || !std::isfinite(r.t))return false;
                for(const auto& r:k.positions)if(r.index>=sizes[0])return false;
                for(const auto& r:k.normals)if(r.index>=normalCount)return false;
                for(const auto& r:k.nodes)if(r.index>=sizes[1] || (r.c>-89 && r.c<89 && (r.b<-89 || r.b>=89)))return false;
                for(const auto& r:k.textures)if(r.index>=sizes[4])return false;
                for(const auto& r:k.visibility)if(r.index>=visibilityCount)return false;
                p.keys.push_back(std::move(k));
            }
            if(p.keys.size()>1 && p.keys.back().time!=p.end)return false;
            ActorAnimationSample sample;
            for(const auto& k:p.keys)if(!p.evaluate(k.time,sample))return false;
            pending.push_back(std::move(p));
        }
        if(std::fgetc(file)!=EOF)return false;
        poses=std::move(pending);return true;
    }
private:std::vector<ActorAnimationPose> poses;
};
static_assert(sizeof(ActorGroup)==20 && sizeof(ActorVertexBinding)==20 && sizeof(ActorTextureBinding)==516 &&
              sizeof(ActorSparseUpdate)==8 && sizeof(ActorTextureUpdate)==8,"Local animation pack layout");
}
