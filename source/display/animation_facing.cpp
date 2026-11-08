#include "animation_facing.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
bool load(const AnimationFacingServices &s, uint32_t a, float &f) {
  if (!s.word)
    return false;
  auto w = s.word(s.user, a);
  if (!w)
    return false;
  std::memcpy(&f, &*w, 4);
  return true;
}
bool word(const AnimationFacingServices &s, uint32_t a, uint32_t &w) {
  if (!s.word)
    return false;
  auto v = s.word(s.user, a);
  if (!v)
    return false;
  w = *v;
  return true;
}
bool store(const AnimationFacingServices &s, uint32_t a, float f) {
  return s.store && s.store(s.user, a, bits(f));
}
// Guest addresses/field offsets are hexadecimal. Pose fields:70 incoming-90,
// 74 retained angle,78 front/rear sign,7c target. Live source literals:
// fb24=1,fb28=0,fb2c=degrees->radians;fb6c=-1,fb70=-90,
// fb74=57.29579162597656 radians->degrees (not a substituted 180/pi),
// fb78=360,fb7c=-360,fb80=90,fb84=270,fb88=-270,
// fb8c=180,fb90=-180,fb94=.1; all are in 80410000 source storage.
// Same source graph at all three camera samples. Every reload is deliberate.
// camGetPtr8000f2fc(4), then targetX18/eyeXc/targetZ20/eyeZ14;
// atan2(negative deltaX,negative deltaZ)802725a0 narrows double BEFORE
// degree-factor loads. Captured calls survive holds without repeated queries.
bool heading(const AnimationFacingServices &s, AnimationHeadingCursor &c,
             float &out) {
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
  }
  const auto &p = c.owner;
  auto call = [&](uint32_t fn) {
    if (!c.pending) {
      c.call = {};
      c.call.function = fn;
      c.call.argument = 4;
      c.call.scalars = {c.fields[0], c.fields[2]};
      c.pending = true;
    }
    if (!p.call || !p.call(p.user, c.call))
      return false;
    c.pending = false;
    return true;
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (!call(0x8000f2fc))
        return false;
      c.camera = c.call.word;
      ++c.phase;
      break;
    case 1:
      if (!load(p, c.camera + 0x18, c.fields[0]))
        return false;
      ++c.phase;
      break;
    case 2:
      if (!load(p, c.camera + 0xc, c.fields[1]))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!load(p, c.camera + 0x20, c.fields[2]))
        return false;
      ++c.phase;
      break;
    case 4:
      if (!load(p, c.camera + 0x14, c.fields[3]))
        return false;
      c.fields[0] = -(c.fields[0] - c.fields[1]);
      c.fields[2] = -(c.fields[2] - c.fields[3]);
      ++c.phase;
      break;
    case 5:
      if (!call(0x802725a0))
        return false;
      c.value = float(c.call.number);
      ++c.phase;
      break;
    case 6:
      if (!load(p, 0x8041fb74, c.factor))
        return false;
      ++c.phase;
      break;
    case 7:
      if (!load(p, 0x8041fb78, c.turn))
        return false;
      ++c.phase;
      break;
    case 8:
      if (!load(p, 0x8041fb7c, c.bound))
        return false;
      c.value = c.factor * c.value;
      ++c.phase;
      break;
    // Repeated +360 below-360,then-360 at>=360,then one+360 if negative.
    // Each backedge yields natively; no fmod, finite clamp or total loop cap.
    case 9:
      if (c.value < c.bound) {
        c.value += c.turn;
        return false;
      }
      ++c.phase;
      break;
    case 10:
      if (!load(p, 0x8041fb78, c.turn))
        return false;
      ++c.phase;
      break;
    case 11:
      if (c.value >= c.turn) {
        c.value -= c.turn;
        return false;
      }
      ++c.phase;
      break;
    case 12:
      if (!load(p, 0x8041fb28, c.bound))
        return false;
      if (c.value < c.bound)
        c.value += c.turn;
      out = c.value;
      c = {};
      return true;
    default:
      return false;
    }
}
// Difference subtracts after loading C; rear target adds after that same load.
// Difference/rear-target wrap omits heading's final negative->positive add.
// Retain operands and reached literal reloads, including nonprogressing infinities.
bool signedWrap(float left, float right, bool add,
                const AnimationFacingServices &s, AnimationSignedWrapCursor &c,
                float &out) {
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.left = left;
    c.right = right;
    c.add = add;
  }
  const auto &p = c.owner;
  for (;;)
    switch (c.phase) {
    case 0:
      if (!load(p, 0x8041fb78, c.turn))
        return false;
      c.value = c.add ? c.left + c.right : c.left - c.right;
      ++c.phase;
      break;
    case 1:
      if (!load(p, 0x8041fb7c, c.bound))
        return false;
      ++c.phase;
      break;
    case 2:
      if (c.value < c.bound) {
        c.value += c.turn;
        return false;
      }
      ++c.phase;
      break;
    case 3:
      if (!load(p, 0x8041fb78, c.turn))
        return false;
      ++c.phase;
      break;
    case 4:
      if (c.value >= c.turn) {
        c.value -= c.turn;
        return false;
      }
      out = c.value;
      c = {};
      return true;
    default:
      return false;
    }
}
// Main tail only classifies sign; Inner also updates angle/matrices. Both
// use pose flag8, but Main must run after successful full evaluation upstream.
bool run(AnimationFacingCursor &c, BattleActMatrix *output) {
  const auto &p = c.owner;
  auto matrix = [&](uint32_t fn) {
    if (!c.pending) {
      c.math = {};
      c.math.function = fn;
      // SDK axis 121 is ASCII 'y'. Requests retain copied matrix operands;
      // Concat8029dd64 returns model*operand, not a fresh guest stack matrix.
      c.math.axis = 121;
      c.math.radians = c.angle;
      if (fn == 0x8029dd64) {
        c.math.left = c.model;
        c.math.right = c.operand;
      } else if (fn == 0x8029e2b0)
        c.math.xyz = {c.literal, c.literal, c.other};
      c.pending = true;
    }
    if (!p.matrix || !p.matrix(p.user, c.math))
      return false;
    c.pending = false;
    if (fn == 0x8029dd64)
      c.model = c.math.output;
    else
      c.operand = c.math.output;
    return true;
  };
  uint32_t flags = 0;
  for (;;)
    switch (c.phase) {
    case 0:
      if (!word(p, c.pose, flags))
        return false;
      c.phase = (flags & 8) ? (c.inner ? 1 : 3) : (c.inner ? 60 : 80);
      break;
    // Inner flag8 first stores incoming+(-90) to70 BEFORE camera work.
    // Main instead uses fresh74; Inner reloads70 after the first heading.
    case 1:
      if (!load(p, 0x8041fb70, c.literal))
        return false;
      c.angle = c.incoming + c.literal;
      ++c.phase;
      break;
    case 2:
      if (!store(p, c.pose + 0x70, c.angle))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!heading(p, c.heading, c.angle))
        return false;
      ++c.phase;
      break;
    case 4:
      if (!load(p, c.pose + (c.inner ? 0x70 : 0x74), c.other))
        return false;
      ++c.phase;
      break;
    case 5:
      if (!signedWrap(c.angle, c.other, false, p, c.wrap, c.angle))
        return false;
      ++c.phase;
      break;
    // Rear iff difference in (90,270] OR[-270,-90); boundaries±90 are
    // front,±270 rear. Ordered NaN comparisons fall through to front.
    case 6:
      if (!load(p, 0x8041fb80, c.literal))
        return false;
      c.phase = c.angle > c.literal ? 7 : 8;
      break;
    case 7:
      if (!load(p, 0x8041fb84, c.literal))
        return false;
      c.phase = c.angle <= c.literal ? 11 : 8;
      break;
    case 8:
      if (!load(p, 0x8041fb70, c.literal))
        return false;
      c.phase = c.angle < c.literal ? 9 : 10;
      break;
    case 9:
      if (!load(p, 0x8041fb88, c.literal))
        return false;
      c.phase = c.angle >= c.literal ? 11 : 10;
      break;
    case 10:
      c.rear = false;
      c.phase = c.inner ? 15 : 12;
      break;
    case 11:
      c.rear = true;
      c.phase = c.inner ? 15 : 12;
      break;
    case 12:
      if (!load(p, c.rear ? 0x8041fb6c : 0x8041fb24, c.literal))
        return false;
      ++c.phase;
      break;
    case 13:
      if (!store(p, c.pose + 0x78, c.literal))
        return false;
      c.phase = 80;
      break;
    // BOTH front/rear request a second fresh camera heading. Rear adds 180
    // and wraps; do not reuse the first sample or infer a shortest-arc target.
    case 15:
      if (!heading(p, c.heading, c.angle))
        return false;
      c.phase = c.rear ? 16 : 18;
      break;
    case 16:
      if (!load(p, 0x8041fb8c, c.literal))
        return false;
      ++c.phase;
      break;
    case 17:
      if (!signedWrap(c.literal, c.angle, true, p, c.wrap, c.angle))
        return false;
      ++c.phase;
      break;
    case 18:
      if (!store(p, c.pose + 0x7c, c.angle))
        return false;
      ++c.phase;
      break;
    // Unwrap74 around fresh target7c via repeated ±360 until difference is
    // within ±180. Each body rereads74 before store; source writes survive holds.
    case 19:
      if (!load(p, 0x8041fb78, c.turn))
        return false;
      ++c.phase;
      break;
    case 20:
      if (!load(p, 0x8041fb8c, c.literal))
        return false;
      ++c.phase;
      break;
    case 21:
      if (!load(p, c.pose + 0x7c, c.target))
        return false;
      ++c.phase;
      break;
    case 22:
      if (!load(p, c.pose + 0x74, c.old))
        return false;
      c.difference = c.target - c.old;
      c.phase = c.difference > c.literal ? 23 : 25;
      break;
    case 23:
      if (!load(p, c.pose + 0x74, c.old))
        return false;
      c.old += c.turn;
      ++c.phase;
      break;
    case 24:
      if (!store(p, c.pose + 0x74, c.old))
        return false;
      c.phase = 21;
      return false;
    case 25:
      if (!load(p, 0x8041fb78, c.turn))
        return false;
      ++c.phase;
      break;
    case 26:
      if (!load(p, 0x8041fb90, c.literal))
        return false;
      ++c.phase;
      break;
    case 27:
      if (!load(p, c.pose + 0x7c, c.target))
        return false;
      ++c.phase;
      break;
    case 28:
      if (!load(p, c.pose + 0x74, c.old))
        return false;
      c.difference = c.target - c.old;
      c.phase = c.difference < c.literal ? 29 : 31;
      break;
    case 29:
      if (!load(p, c.pose + 0x74, c.old))
        return false;
      c.old -= c.turn;
      ++c.phase;
      break;
    case 30:
      if (!store(p, c.pose + 0x74, c.old))
        return false;
      c.phase = 27;
      return false;
    // On final unwrap exit retain target/old/difference across flag100:
    // snap to target if set,otherwise FMA(.1,difference,old) once per admitted
    // Inner invocation. No tick duration or once-per-render guard is inserted.
    case 31:
      if (!word(p, c.pose, flags))
        return false;
      if (flags & 0x100) {
        c.old = c.target;
        c.phase = 33;
      } else
        ++c.phase;
      break;
    case 32:
      if (!load(p, 0x8041fb94, c.literal))
        return false;
      c.old = std::fma(c.literal, c.difference, c.old);
      ++c.phase;
      break;
    case 33:
      if (!store(p, c.pose + 0x74, c.old))
        return false;
      ++c.phase;
      break;
    // Fresh74 -> RotRad8029dff0 then right Concat. Load source-1 before
    // fresh78; exact equality applies Scale8029e2b0(1,1,-1) Z reflection.
    case 34:
      if (!load(p, c.pose + 0x74, c.angle))
        return false;
      ++c.phase;
      break;
    case 35:
      if (!load(p, 0x8041fb28, c.literal))
        return false;
      c.phase = c.angle != c.literal ? 36 : 40;
      break;
    case 36:
      if (!load(p, 0x8041fb2c, c.literal))
        return false;
      c.angle = c.literal * c.angle;
      ++c.phase;
      break;
    case 37:
      if (!matrix(0x8029dff0))
        return false;
      ++c.phase;
      break;
    case 38:
      if (!matrix(0x8029dd64))
        return false;
      c.phase = 40;
      break;
    case 40:
      if (!load(p, 0x8041fb6c, c.other))
        return false;
      ++c.phase;
      break;
    case 41:
      if (!load(p, c.pose + 0x78, c.literal))
        return false;
      c.phase = c.literal == c.other ? 42 : 70;
      break;
    case 42:
      if (!load(p, 0x8041fb24, c.literal))
        return false;
      ++c.phase;
      break;
    case 43:
      if (!matrix(0x8029e2b0))
        return false;
      ++c.phase;
      break;
    case 44:
      if (!matrix(0x8029dd64))
        return false;
      c.phase = 70;
      break;
    // Flag8 clear skips camera/pose-angle/reflection work, rotates incoming
    // angle if !=ZERO, then joins common uniform scale. NaN follows != gates.
    case 60:
      if (!load(p, 0x8041fb28, c.literal))
        return false;
      c.phase = c.incoming != c.literal ? 61 : 70;
      break;
    case 61:
      if (!load(p, 0x8041fb2c, c.literal))
        return false;
      c.angle = c.literal * c.incoming;
      ++c.phase;
      break;
    case 62:
      if (!matrix(0x8029dff0))
        return false;
      ++c.phase;
      break;
    case 63:
      if (!matrix(0x8029dd64))
        return false;
      c.phase = 70;
      break;
    // Always read live ONE even if supplied scale is 1. Rotation/reflection
    // precede uniform Scale(S,S,S). Only successful completion publishes out.
    case 70:
      if (!load(p, 0x8041fb24, c.literal))
        return false;
      if (c.scale == c.literal)
        c.phase = 80;
      else {
        c.literal = c.scale;
        c.other = c.scale;
        ++c.phase;
      }
      break;
    case 71:
      if (!matrix(0x8029e2b0))
        return false;
      ++c.phase;
      break;
    case 72:
      if (!matrix(0x8029dd64))
        return false;
      c.phase = 80;
      break;
    case 80:
      if (output)
        *output = c.model;
      c = {};
      return true;
    default:
      return false;
    }
}
} // namespace
// Public child captures request/providers only at start (and rejects mixing
// Main/Inner mode). Caller must keep SAME operands/providers while active;
// these helpers do not themselves fingerprint changed pose/matrix/scalar inputs.
bool advanceAnimationMainFacing(uint32_t pose, const AnimationFacingServices &s,
                                AnimationFacingCursor &c) {
  if (c.active && c.inner)
    return false;
  if (!c.active) {
    c = {};
    c.active = true;
    c.pose = pose;
    c.owner = s;
  }
  return run(c, nullptr);
}
bool advanceAnimationInnerFacing(uint32_t pose, float incoming, float scale,
                                 const BattleActMatrix &model,
                                 const AnimationFacingServices &s,
                                 AnimationFacingCursor &c,
                                 BattleActMatrix &out) {
  if (c.active && !c.inner)
    return false;
  if (!c.active) {
    c = {};
    c.active = true;
    c.inner = true;
    c.pose = pose;
    c.incoming = incoming;
    c.scale = scale;
    c.model = model;
    c.owner = s;
  }
  return run(c, &out);
}
} // namespace ttyd
