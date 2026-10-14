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
    CameraDepthSnapshot source;
    for(unsigned i=0;i<3;++i)source.view[i][i]=1;
    for(unsigned i=0;i<4;++i)source.projection[i][i]=1;
    source.view[0][3]=2;source.view[1][3]=3;
    CameraMatrixPublication state;state.rollDegrees=90;state.translation={10,20,30};
    state.flags=0x49; // Disabled drawing does not suppress matrix publication.
    for(unsigned i=0;i<4;++i)state.extra[i][i]=1;
    state.extra[0][0]=2;state.extra[0][3]=7;
    const auto published=publishCameraMatrices(source,state);
    assert(std::fabs(published.view[0][3]-7)<1e-5f);
    assert(std::fabs(published.view[1][3]-22)<1e-5f);
    assert(published.view[2][3]==30);
    assert(published.projection[0][0]==2 && published.projection[0][3]==7);
    assert(state.flags==0x41 && state.extra[0][3]==7);
    const auto second=publishCameraMatrices(published,state);
    assert(second.projection==published.projection); // Extra consumed only once.
    state.flags|=8;
    const auto again=publishCameraMatrices(published,state);
    assert(again.projection[0][0]==4 && again.projection[0][3]==21);
    // Projection source order rounds far*near before multiplying reciprocal.
    const auto lens=makeCameraDepthSnapshot({0,0,10},{0,0,0},{0,1,0},25,608.f/480.f,100,10000);
    assert(lens && lens->projection[2][3]==(1.f/(10000.f-100.f))*(-(10000.f*100.f)));
}
