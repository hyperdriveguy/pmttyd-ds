#include "battle_audience_backend.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
namespace {
using Point=BattleAudiencePoint;
std::array<float,3> transform(const BattleActMatrix& m,const std::array<float,3>& p){
    std::array<float,3> out{};for(unsigned row=0;row<3;++row){const auto i=row*4;
        const float xy=std::fma(m[i+1],p[1],m[i]*p[0]);const float xyz=std::fma(m[i+2],p[2],xy);out[row]=xyz+m[i+3];}
    return out;
}
float coord(const Point& p,unsigned axis){return axis==0?p.u:p.v;}
Point interpolate(const Point& a,const Point& b,float t){return {a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,a.z+(b.z-a.z)*t,a.u+(b.u-a.u)*t,a.v+(b.v-a.v)*t};}
using Polygon=std::vector<Point>;
Polygon cut(const Polygon& input,unsigned axis,float bound,bool upper){
    Polygon result;if(input.empty())return result;auto previous=input.back();bool inside=upper?coord(previous,axis)>=bound:coord(previous,axis)<=bound;
    for(const auto& current:input){const bool next=upper?coord(current,axis)>=bound:coord(current,axis)<=bound;
        if(next!=inside){auto p=interpolate(previous,current,(bound-coord(previous,axis))/(coord(current,axis)-coord(previous,axis)));if(axis==0)p.u=bound;else p.v=bound;result.push_back(p);}
        if(next)result.push_back(current);previous=current;inside=next;
    }return result;
}
}
void bindAudienceMathAdapters(BattleAudienceDrawServices& s){
    s.scale=[](void*,float x,float y,float z)->std::optional<BattleActMatrix>{return BattleActMatrix{x,0,0,0,0,y,0,0,0,0,z,0};};
    s.vector=[](void*,const BattleActMatrix& m,const std::array<float,3>& p)->std::optional<std::array<float,3>>{return transform(m,p);};
    s.concat=[](void*,const BattleActMatrix& a,const BattleActMatrix& b)->std::optional<BattleActMatrix>{BattleActMatrix out{};
        for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col){const auto i=row*4;
            const float xy=std::fma(a[i+1],b[4+col],a[i]*b[col]);const float xyz=std::fma(a[i+2],b[8+col],xy);out[i+col]=col==3?xyz+a[i+3]:xyz;}return out;};
    s.sine=[](void*,double v)->std::optional<double>{if(!std::isfinite(v))return {};return std::sin(v);};
    s.cosine=[](void*,double v)->std::optional<double>{if(!std::isfinite(v))return {};return std::cos(v);};
    s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};
    s.truncate=[](void*,float v)->std::optional<int32_t>{if(!std::isfinite(v) || v>=2147483648.f || v<-2147483648.f)return {};return int32_t(v);};
}
bool BattleAudienceBackend::request(const BattleAudienceGXRequest& r){
    const auto& v=r.arguments;std::array<int32_t,7> expected{};
    switch(r.function){
    case 0x802b5c8c:case 0x802b4520:case 0x802b7880:case 0x802ba2fc:break;
    case 0x802b3c08:if(v[0]!=9 && v[0]!=13)return false;expected={v[0],1};break;
    case 0x802b4558:if(v[1]!=9 && v[1]!=13)return false;expected={0,v[1],1,4,0};break;
    case 0x802b6f3c:case 0x802b5084:case 0x802b8fcc:expected={1};break;
    case 0x802b6f78:expected={4,0,1,0,0,2,2};break;
    case 0x802b4e04:expected={0,1,4,60,0,125};break;
    case 0x802b89e8:case 0x802b8a50:expected={0,0,0,0,1,0};break;
    case 0x802b8960:expected={0,15,12,8,10};break;
    case 0x802b89a4:expected={0,7,5,4,7};break;
    case 0x802b8e30:expected={0,0,0,4};break;
    case 0x802b6e54:expected={4,v[1]};break;
    case 0x802b5a6c:expected={0x80,0,4};break;
    default:return false;
    }
    if(v!=expected)return false;
    requests.push_back(r);return true;
}
bool BattleAudienceBackend::emitQuad(const BattleAudienceQuad& q){
    if(!assets)return false;const auto* region=assets->frame(q.texture.bank,q.texture.index);
    if(!region || q.texture.width!=region->width || q.texture.height!=region->height)return false;
    for(float value:q.matrix)if(!std::isfinite(value))return false;
    auto points=q.points;
    for(auto& p:points){for(float value:{p.x,p.y,p.z,p.u,p.v})if(!std::isfinite(value))return false;
        const auto xyz=transform(q.matrix,{p.x,p.y,p.z});p.x=xyz[0];p.y=xyz[1];p.z=xyz[2];}
    std::vector<BattleAudienceVertex> staged;
    for(const auto indices:{std::array<unsigned,3>{0,1,2},{0,2,3}}){
        std::vector<Polygon> pieces{{points[indices[0]],points[indices[1]],points[indices[2]]}};
        // Piecewise UV clamping, preserving the whole original geometry. Clamp
        // at vertices alone would change interpolation inside the source image.
        for(unsigned axis=0;axis<2;++axis)for(float bound:{0.f,1.f}){
            std::vector<Polygon> next;
            for(const auto& polygon:pieces){float lo=coord(polygon[0],axis),hi=lo;for(const auto& p:polygon){lo=std::min(lo,coord(p,axis));hi=std::max(hi,coord(p,axis));}
                if(lo<bound && hi>bound){auto below=cut(polygon,axis,bound,false),above=cut(polygon,axis,bound,true);if(below.size()>=3)next.push_back(std::move(below));if(above.size()>=3)next.push_back(std::move(above));}
                else next.push_back(polygon);
            }pieces=std::move(next);
        }
        for(const auto& polygon:pieces)for(unsigned i=1;i+1<polygon.size();++i)for(const auto& p:{polygon[0],polygon[i],polygon[i+1]}){
            const float u=std::clamp(p.u,0.f,1.f),v=std::clamp(p.v,0.f,1.f);
            staged.push_back({p.x,p.y,p.z,region->uv[0]+(region->uv[2]-region->uv[0])*u,
                region->uv[1]+(region->uv[3]-region->uv[1])*v,q.rgba});
        }
    }
    vertices.insert(vertices.end(),staged.begin(),staged.end());return true;
}
}
