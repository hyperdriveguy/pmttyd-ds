#pragma once
#include <memory>
namespace ttyd {
class Game;class Renderer;class Audio;
// Main-loop composition of the actual native battle, GPU and stream owners.
// Runs outside FrameBegin and only on accepted simulation visits.
class NativeHarborEndingOwner {
public:
 NativeHarborEndingOwner();
 ~NativeHarborEndingOwner();
 NativeHarborEndingOwner(const NativeHarborEndingOwner&)=delete;
 NativeHarborEndingOwner& operator=(const NativeHarborEndingOwner&)=delete;
 bool advance(Game&,Renderer&,Audio&);
 bool updateAudio(Game&,Audio&);
private:
 struct State;std::shared_ptr<State>state_;
};
}
