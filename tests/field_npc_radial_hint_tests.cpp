#include "camera_road_assets.hpp"
#include "field_actors.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main() {
  assert(loadFieldActorCatalog("romfs") && loadCameraRoadAssets("romfs"));
  assert(loadCollision("romfs/gor_02.walls") &&
         loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
  const auto key = 0x2b0a9f83u;
  const FieldActorRecord *actor = nullptr;
  for (const auto &a : fieldActorCatalog().actors())
    if (a.key == key)
      actor = &a;
  assert(actor && actor->wander.enabled && !actor->eventName.empty());
  const auto name = actor->eventName;
  Progress saved;
  saved.room = 2;
  saved.originalStory = 8;
  saved.position = {-3.4f, 1.3f};
  auto game = std::make_unique<Game>();
  game->restore(saved);
  for (unsigned n = 0; n < 360; ++n)
    game->tick({});
  assert(game->fieldNpcRadialHint() == key &&
         game->fieldNpcContact() ==
             0); // Radial-only distance; no side collision.
  const auto *stopped = game->fieldNpcState(key);
  assert(stopped && stopped->paused);
  // A native skipped scan retains the real prior publication, even when a
  // subsequent placement moves the source actor outside the previous query.
  game->fieldKeysOff = true;
  assert(game->setFieldNpcPosition(
      name, {saved.position.x * 100, 0, saved.position.z * 100 + 30}));
  // Explicit event-input fixture: unpause one regular visit with source inputs
  // pointing toward Mario. Production candidate stop/start is tested
  // separately.
  auto *npc = const_cast<FieldNpcInstance *>(game->fieldNpcState(key));
  assert(npc);
  npc->paused = false;
  npc->body.state &= ~0x20000000u;
  npc->wander.phase = 1;
  npc->wander.count = 10;
  npc->wander.anchor = npc->body.root;
  npc->movement.moveHeading = npc->movement.yaw = 0;
  npc->movement.dt = .01f;
  npc->movement.wall = 0;
  npc->presentation.flags = 0;
  const auto before = npc->body.root;
  game->tick({});
  npc = const_cast<FieldNpcInstance *>(game->fieldNpcState(key));
  assert(npc);
  assert(game->fieldNpcRadialHint() == key && game->fieldNpcContact() == 0 &&
         npc->wander.count == 9);
  assert(
      npc->movement.moveHeading == 180 &&
      npc->body.root.z >
          before.z); // Consumed radial hint; collision0 cannot cause this turn.
  // A completed scan with no query-admitted actor publishes zero. It does not
  // invent a candidate-loss resume for a source-skipped actor.
  assert(game->setFieldNpcFlags(name, 2, false));
  game->fieldKeysOff = false;
  game->tick({});
  assert(game->fieldNpcRadialHint() == 0);
  assert(game->setFieldNpcFlags(name, 2, true));
  assert(game->setFieldNpcPosition(name, {-370, 0, 130}));
  for (unsigned n = 0; n < 60 && !game->fieldNpcRadialHint(); ++n)
    game->tick({});
  assert(game->fieldNpcRadialHint() == key);
  // Catalog replacement retires native actor instances, so its old native key
  // cannot survive as a pointer-like hint even if the next scan is skipped.
  game->fieldKeysOff = true;
  assert(loadFieldActorCatalog("romfs"));
  game->tick({});
  assert(game->fieldNpcRadialHint() == 0);
  game->restore(saved);
  assert(game->fieldNpcRadialHint() ==
         0); // Transient, not serialized checkpoint state.
}
