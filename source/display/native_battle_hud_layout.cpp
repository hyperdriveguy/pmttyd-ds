#include "native_battle_hud_layout.hpp"
#include <algorithm>
namespace ttyd {
namespace {
constexpr uint16_t HpBanner=472,MarioPortrait=427,FractionSlash=480;
constexpr uint16_t FpBanner=473,FlowerSymbol=441;
constexpr uint16_t ApBanner=474,StarPowerSymbol=442;
constexpr uint16_t CounterBanner=475,StarPointSymbol=443,CoinSymbol=444;
constexpr uint16_t MultiplyMark=478,FirstLargeDigit=481;
constexpr unsigned DigitIconStride=2;
constexpr int NativeDecimalLimit=999999;
constexpr float SourceToNative=.5f;
// Existing centered320px HP/FP region within the400px top screen.
constexpr float NativeHorizontalOffset=48;
constexpr float TopResourceBaseline=20;
constexpr float LargeDigitPitch=10,SmallDigitPitch=8;
} // namespace
std::optional<NativeBattleHudPlan>planNativeBattleHud(const NativeBattleHudReadings&r){
 if(r.sourceStatus&&r.sourceStatus->gaugeCount>r.sourceStatus->gauge.size())return {};
 NativeBattleHudPlan p;
 auto icon=[&](uint32_t id,float x,float bottom,float scale=1.f){
  p.icons[p.count++]={id,x,bottom,scale};
 };
 auto number=[&](int value,float right,float bottom,bool small){
  if(value<0)return;
  value=std::min(value,NativeDecimalLimit);
  do{icon(FirstLargeDigit+DigitIconStride*(value%10)+(small?1:0),right,bottom);
   right-=small?SmallDigitPitch:LargeDigitPitch;value/=10;}while(value);
 };
 icon(HpBanner,102,TopResourceBaseline);icon(MarioPortrait,67,TopResourceBaseline);
 icon(FractionSlash,110,17);
 number(r.hp,99,TopResourceBaseline,false);number(r.maxHp,127,TopResourceBaseline,true);
 if(r.maxFp>0){
  icon(FpBanner,192,TopResourceBaseline);icon(FlowerSymbol,158,TopResourceBaseline);
  icon(FractionSlash,200.5f,17);
  number(r.fp,189,TopResourceBaseline,false);number(r.maxFp,217,TopResourceBaseline,true);
 }
 if(r.sourceStatus&&r.sourceStatus->partnerPortrait){
  icon(HpBanner,102,40);icon(*r.sourceStatus->partnerPortrait,67,40);
  icon(FractionSlash,110,37);
  number(r.sourceStatus->values[NativeBattleStatusSnapshot::PartnerHp],99,40,false);
  number(r.sourceStatus->values[NativeBattleStatusSnapshot::PartnerMaxHp],127,40,true);
 }
 if(r.sourceStatus&&r.sourceStatus->gaugeCount){
  // Original8036edd0 rows11/12: (360,76,474,1)/(220,80,442,.9).
  icon(ApBanner,NativeHorizontalOffset+360*SourceToNative,76*SourceToNative);
  icon(StarPowerSymbol,NativeHorizontalOffset+220*SourceToNative,80*SourceToNative,.9f);
  // StatusWinDisp8013cef4..cf10 passes gauge origin(260,83);
  // retained gauge rows supply their original X/Y offsets (full-star Y12).
  constexpr float GaugeSourceX=260,GaugeSourceBottom=83;
  for(unsigned i=0;i<r.sourceStatus->gaugeCount;++i){
   const auto&q=r.sourceStatus->gauge[i];
   icon(q.icon,NativeHorizontalOffset+(GaugeSourceX+q.x)*SourceToNative,
        (GaugeSourceBottom-q.y)*SourceToNative);
  }
 }
 if(r.coins>=0||r.starPoints>=0){
  // Preserve existing compact horizontal counter reflow. Original source row
  // y40 shares the same bottom baseline20 as the two resource banners.
  icon(CounterBanner,335,TopResourceBaseline);
  if(r.starPoints>=0){icon(StarPointSymbol,289,TopResourceBaseline);
   icon(MultiplyMark,301,TopResourceBaseline);number(r.starPoints,321,TopResourceBaseline,false);}
  if(r.coins>=0){icon(CoinSymbol,339,TopResourceBaseline);
   icon(MultiplyMark,351,TopResourceBaseline);number(r.coins,378,TopResourceBaseline,false);}
 }
 return p;
}
}
