#include "camera_depth.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
int main(){
    CameraDepthSnapshot c;for(unsigned i=0;i<3;++i)c.view[i][i]=1;for(unsigned i=0;i<4;++i)c.projection[i][i]=1;
    assert(cameraQueueDepth(c,{0,0,0})==-5000);
    assert(cameraQueueDepth(c,{0,0,2})==-10000);
    const float zero=cameraQueueDepth(c,{0,0,-2});assert(zero==0 && std::signbit(zero));
    assert(std::isnan(cameraQueueDepth(c,{0,0,std::numeric_limits<float>::quiet_NaN()})));
    c.projection[3][3]=2;assert(cameraDepthProject(c,{2,4,1})==CameraDepthPoint({1,2,.5f}));
    assert(cameraQueueDepth(c,{0,0,1})==-7500); // Homogeneous divide, not raw clipZ.
    c.view[0]={1,2,3,4};assert(cameraDepthView(c,{1,2,3})[0]==18);
    auto perspective=makeCameraDepthSnapshot({0,0,10},{0,0,0},{0,1,0},25,1,1,100);
    assert(perspective && std::fabs(cameraQueueDepth(*perspective,{0,0,9}))<.01f);
    assert(std::fabs(cameraQueueDepth(*perspective,{0,0,-90})+5000)<.01f);
    assert(!makeCameraDepthSnapshot({0,0,0},{0,0,0},{0,1,0},25,1,1,100));
}
