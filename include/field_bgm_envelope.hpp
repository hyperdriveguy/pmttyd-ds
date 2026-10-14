#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
// Source GP-rate duration conversion with explicit positive/signed16 admission.
// Signed32 overflow and nonpositive narrowed durations are refused. Source
// low16 wrapping is retained; no clamp supplies a guessed tick.
std::optional<int16_t> fieldBgmDuration(int32_t milliseconds,int32_t gpRate);
struct FieldBgmPublication {bool published=false,detach=false;uint8_t volume=0;uint16_t tailMilliseconds=0;};
// Finite original fresh primary Off201 OR field secondary3800/a0 envelopes.
// Each main() is an admitted psndBGMMain call, not a rendering frame.
class FieldBgmEnvelope {
public:
 void bind(uint8_t base,int8_t globalPercentage=100,bool bypassGlobal=true);
 void backendVacant(){assigned_=false;}
 int32_t check() const{return assigned_?0:-1;}
 bool silent() const{return assigned_ && secondary_==8;}
 bool off201(int16_t duration);
 bool mute3800(int16_t duration,bool fast=false);
 bool resumeA0(int16_t duration,bool fast=false);
 std::optional<FieldBgmPublication> main();
 uint8_t primaryState() const{return primary_;}
 uint8_t secondaryState() const{return secondary_;}
 int16_t primaryRemaining() const{return primaryRemaining_;}
 int16_t secondaryRemaining() const{return secondaryRemaining_;}
private:
 bool assigned_=false,bypassGlobal_=true,fast_=false;
 uint8_t base_=0,primary_=0,secondary_=0;
 int8_t global_=100;
 int16_t primaryDuration_=0,primaryRemaining_=0,secondaryDuration_=0,secondaryRemaining_=0;
};
struct FieldStreamFadePublication {bool published=false,stop=false;uint8_t volume=0;};
// Clean source SoundSS flag20 tail, float subtraction and prezero stop gate.
// Combined flags40/10 and source IRQ/device ownership remain external.
class FieldStreamFade {
public:
 bool begin(float retainedVolume,uint16_t milliseconds,int32_t gpRate);
 FieldStreamFadePublication main(bool sourceGate=true);
 bool active() const{return active_;}
 float volume() const{return volume_;}
 float decrement() const{return decrement_;}
 void clear(){active_=false;}
private:
 bool active_=false;float volume_=0,decrement_=0;
};
}
