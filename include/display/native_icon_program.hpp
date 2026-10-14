#pragma once
#include "source_icon_gx.hpp"
namespace ttyd {
struct NativeIconPlan {
  enum Mode { Normal, Grayscale, Blended } mode = Normal;
  uint32_t material = 0xffffffff, tint = 0xffffffff, blend = 0xffffffff,
           fog = 0xffffffff, cull = 0;
};
// Retained source register owner. Setters update real backend state; complete
// original programs are compiled/published before Begin, not guessed per icon.
class NativeIconProgram {
public:
  NativeIconProgram();
  bool request(const SourceIconGXCall &);
  std::optional<NativeIconPlan> plan() const;

private:
  using Args = std::array<uint32_t, 8>;
  struct Stage {
    std::optional<Args> order, colorOp, alphaOp, colorIn, alphaIn, swap;
  };
  std::array<Stage, 3> stages_{};
  std::array<std::optional<Args>, 4> swaps_{};
  std::array<std::optional<uint32_t>, 3> colors_{};
  std::array<std::optional<Args>, 2> channels_{};
  std::optional<uint32_t> material_, ambient_, fog_, cull_, chans_, count_,
      generators_;
  std::array<bool, 3> descriptors_{};
  std::array<bool, 3> formats_{};
  bool fogOff_ = false, coord_ = false;
};
} // namespace ttyd
