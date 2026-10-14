#pragma once
#include <array>
#include <cstdint>
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace ttyd {
struct FieldMapFlagGeometry;
bool loadFieldMapFlagAssets(const char* root);
// Native event-owned flag overlay on original map names/hierarchy. These are
// not original MapObj/HitObj ABI records or a replacement for transform services.
class FieldMapFlags {
public:
 void invalidate();
 void synchronize(uint32_t room);
 bool ready(uint32_t vertices) const;
 uint64_t epoch() const{return epoch_;}
 bool mapFlag(const std::string& name,uint32_t mask,bool on);
 bool hitEnabled(const std::string& name,bool enabled);
 bool hitFlags(const std::string& name,uint16_t mask,bool on);
 std::optional<uint32_t> hitSourceNode(const std::string& name) const; // Name lookup excludes80, not1.
 uint32_t room() const;
 std::optional<uint32_t> mapFlags(const std::string& name) const;
 std::optional<bool> hitEnabled(const std::string& name) const;
 const std::vector<uint32_t>& disabledHits() const{return disabled_;}
 uint32_t vertices() const;
 // Templated visitor keeps field draw submission free of std::function allocations.
 template<class Draw> void visibleRanges(uint32_t first,uint32_t count,const Draw& draw) const {
  const auto size=vertices();if(!ready(size) || first>size || count>size-first)return;
  refreshVisibility();const auto end=first+count;
  auto it=std::lower_bound(visible_.begin(),visible_.end(),first,[](const auto& r,uint32_t v){return r[0]+r[1]<=v;});
  for(;it!=visible_.end() && (*it)[0]<end;++it){const auto a=std::max(first,(*it)[0]),b=std::min(end,(*it)[0]+(*it)[1]);if(b>a)draw(a,b-a);}
 }
private:
 std::shared_ptr<const FieldMapFlagGeometry> geometry_;
 uint32_t room_=UINT32_MAX;
 uint64_t generation_=0,epoch_=0;
 std::vector<uint32_t> flags_,disabled_;
 std::vector<uint16_t> hitFlags_;
 mutable std::vector<std::array<uint32_t,2>> visible_;
 mutable bool dirty_=true;
 int find(const std::string&) const;
 void refreshVisibility() const;
};
}
