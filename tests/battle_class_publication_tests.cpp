#include "battle_class_publication.hpp"
#include <cassert>
using namespace ttyd;
int main() {
  BattleClassAssets assets;
  assert(assets.load("romfs/battle-class-surfaces.bin",
                     "romfs/battle-class-icons.bin"));
  BattleClassBackend source;
  source.assets = &assets;
  source.quads.push_back({BattleClassTexture::Surfaces, {}, 0});
  source.quads[0].vertices[0].x = 42;
  source.label = [](uint32_t) -> const char * { assert(false); return nullptr; };
  source.random = [](uint32_t) -> uint32_t { assert(false); return 0; };
  source.messageIcon = [](uint32_t) -> const MessageIconFrame * {
    assert(false); return nullptr;
  };
  BattleClassPublication publication;
  auto owner = std::make_shared<const uint8_t>(0);
  BattleClassPublicationKey first{owner, 0x1234, 1};
  assert(!publication.publish(first, source));
  publication.bind(owner, first.allocation);
  assert(!publication.consumeAdmission());
  auto invalid = first;
  invalid.sequence = 0;
  assert(!publication.publish(invalid, source));
  invalid = first;
  invalid.allocation++;
  assert(!publication.publish(invalid, source));
  assert(publication.publish(first, source));
  auto held = publication.batch();
  assert(held && held->assets == &assets && held->complete());
  assert(!held->font && !held->label && !held->random && !held->messageIcon);
  source.quads[0].vertices[0].x = 77;
  assert(held->quads[0].vertices[0].x == 42);
  assert(!publication.consumeAdmission()); // Failed/unattempted draw is no admission.
  assert(publication.admit(first));
  assert(publication.consumeAdmission()->sequence == 1);
  assert(publication.admit(first)); // Repeated rendering cannot repeat consumption.
  assert(!publication.consumeAdmission());
  publication.bind(owner, first.allocation);
  assert(!publication.publish(first, source)); // Same lifetime cannot restart.
  auto second = first;
  second.sequence = 2;
  assert(publication.publish(second, source));
  assert(publication.batch()->quads[0].vertices[0].x == 77);
  assert(!publication.admit(first)); // Delayed old draw completion.
  assert(held->quads[0].vertices[0].x == 42); // Borrow survives replacement.
  auto secondHeld = publication.batch();
  auto third = second;
  third.sequence = 3;
  BattleActMatrix identity{};
  identity[0] = identity[5] = identity[10] = 1;
  assert(source.request({BattleClassRasterService::LoadMatrix, {0}, {}, identity}));
  assert(source.request({BattleClassRasterService::TextureObject, {0x1234, 1}, {}, {}}));
  assert(source.request({BattleClassRasterService::LoadTexture, {1, 0}, {}, {}}));
  assert(source.request({BattleClassRasterService::Begin, {128, 0, 4}, {}, {}}));
  assert(!source.complete());
  assert(!publication.publish(third, source)); // Partial primitive cannot replace.
  assert(publication.batch() == secondHeld && publication.key().sequence == 2);
  assert(publication.admit(second));
  assert(publication.consumeAdmission()->sequence == 2);
  auto replacement = std::make_shared<const uint8_t>(0);
  publication.bind(replacement, first.allocation); // Same guest address, new owner.
  assert(!publication.batch() && !publication.admit(second));
  assert(!publication.consumeAdmission());
  source.begin();
  source.quads.push_back({BattleClassTexture::Surfaces, {}, 0});
  BattleClassPublicationKey fresh{replacement, first.allocation, 1};
  assert(!publication.publish(first, source));
  assert(publication.publish(fresh, source));
  publication.retire();
  assert(!publication.batch() && !publication.admit(fresh));
  assert(!publication.consumeAdmission());
  publication.bind(replacement, first.allocation);
  source.assets = nullptr;
  assert(!publication.publish(fresh, source));
}
