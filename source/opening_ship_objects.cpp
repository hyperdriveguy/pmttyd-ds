#include "opening_ship_objects.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
namespace ttyd {
namespace {
using P=OpeningShipObjects::Point;
P world(const std::array<float,12>& m,P p){P q{};for(unsigned r=0;r<3;++r)q[r]=m[r*4]*p[0]+m[r*4+1]*p[1]+m[r*4+2]*p[2]+m[r*4+3];return q;}
struct Rotation{unsigned axis;float c,s;};
Rotation rotation(unsigned axis,float degrees){const float a=degrees*.01745329238474369f;return {axis,std::cos(a),std::sin(a)};}
P rotate(P p,const Rotation& r){const unsigned i=(r.axis+1)%3,j=(r.axis+2)%3;const float x=p[i],y=p[j];p[i]=r.c*x-r.s*y;p[j]=r.s*x+r.c*y;return p;}
std::array<float,12> invert(const std::array<float,12>& m){
 const float a=m[0],b=m[1],c=m[2],d=m[4],e=m[5],f=m[6],g=m[8],h=m[9],i=m[10];
 const float det=a*(e*i-f*h)-b*(d*i-f*g)+c*(d*h-e*g);
 std::array<float,12> result{{(e*i-f*h)/det,(c*h-b*i)/det,(b*f-c*e)/det,0,
 (f*g-d*i)/det,(a*i-c*g)/det,(c*d-a*f)/det,0,
 (d*h-e*g)/det,(b*g-a*h)/det,(a*e-b*d)/det,0}};
 for(unsigned r=0;r<3;++r)result[r*4+3]=-(result[r*4]*m[3]+result[r*4+1]*m[7]+result[r*4+2]*m[11]);
 return result;
}
}
bool OpeningShipObjects::load(const char* path,uint32_t vertexCount){
 FILE* f=path?std::fopen(path,"rb"):nullptr;if(!f)return false;char magic[8];uint32_t vertices=0,count=0;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMHOBJ02",8) && std::fread(&vertices,4,1,f)==1 && vertices==vertexCount && std::fread(&count,4,1,f)==1 && count==7;
 std::vector<Object> pending;
 for(uint32_t n=0;ok && n<count;++n){uint32_t tag=0,size=0;Object o{};std::map<uint32_t,bool> used;
  ok=std::fread(&tag,4,1,f)==1 && tag==n && std::fread(&size,4,1,f)==1 && size && size<=vertices && std::fread(o.world.data(),4,12,f)==12;
  for(float v:o.world)ok=ok && std::isfinite(v);
  const auto& m=o.world;const float det=m[0]*(m[5]*m[10]-m[6]*m[9])-m[1]*(m[4]*m[10]-m[6]*m[8])+m[2]*(m[4]*m[9]-m[5]*m[8]);ok=ok && std::isfinite(det) && det!=0;
  if(ok)o.bindings.resize(size);
  for(auto& b:o.bindings){ok=ok && std::fread(&b.index,4,1,f)==1 && b.index<vertices && used.emplace(b.index,true).second && std::fread(b.local.data(),4,3,f)==3 && std::fread(b.original.data(),4,3,f)==3;for(float v:b.local)ok=ok && std::isfinite(v);for(float v:b.original)ok=ok && std::isfinite(v);if(!ok)break;}
  if(ok)o.inverse=invert(o.world);
  pending.push_back(std::move(o));
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(ok){objects=std::move(pending);childPositions.resize(vertices);hasChild.assign(vertices,0);for(unsigned n=2;n<7;++n)for(const auto& b:objects[n].bindings)hasChild[b.index]=1;}
 return ok;
}
bool OpeningShipObjects::remapVertices(const std::vector<uint32_t>& indices,uint32_t count) {
 if(objects.size()!=7 || !count)return false;
 for(const auto& o:objects)for(const auto& b:o.bindings)if(b.index>=indices.size() || indices[b.index]>=count)return false;
 for(auto& o:objects)for(auto& b:o.bindings)b.index=indices[b.index];
 childPositions=std::vector<Point>(count);hasChild=std::vector<uint8_t>(count,0);
 for(unsigned n=2;n<7;++n)for(const auto& b:objects[n].bindings)hasChild[b.index]=1;
 return true;
}
void OpeningShipObjects::apply(float x,float y,float wakeX,float yaw,float rock,const std::array<float,4>& parts,const Writer& write) const {
 applyMatrices(x,y,wakeX,yaw,rock,parts,write,0,0,yaw);
}
void OpeningShipObjects::applyVoyage(const Point& position,float yaw,float rock,const std::array<float,4>& parts,const Writer& write) const {
 applyMatrices(position[0],position[1],position[0],yaw,rock,parts,write,position[2],position[1],0);
}
void OpeningShipObjects::applyMatrices(float x,float y,float wakeX,float yaw,float rock,const std::array<float,4>& parts,const Writer& write,float z,float wakeY,float wakeYaw) const {
 if(objects.size()!=7 || !write)return;
 constexpr unsigned axes[]={0,2,2,1,0},part[]={0,1,1,2,3};
 std::array<Rotation,5> childRotations;for(unsigned n=0;n<5;++n)childRotations[n]=rotation(axes[n],parts[part[n]]);
 const Rotation rootRock=rotation(2,rock),rootYaw=rotation(1,yaw),wakeRotation=rotation(1,wakeYaw);
 for(unsigned n=2;n<7;++n)for(const auto& b:objects[n].bindings)childPositions[b.index]=world(objects[n].world,rotate(b.local,childRotations[n-2]));
 for(unsigned n=0;n<2;++n){const auto& o=objects[n];for(const auto& b:o.bindings){P p=b.local;
  if(!n){if(hasChild[b.index])p=world(o.inverse,childPositions[b.index]);p=rotate(p,rootRock);}
  p=rotate(p,n?wakeRotation:rootYaw);p[0]+=(n?wakeX:x)/10.f;p[1]+=(n?wakeY:y)/10.f;p[2]+=z/10.f;write(b.index,world(o.world,p));
 }}
}
}
