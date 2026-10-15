#include "fixtures/harbor_class_frame_fixture.hpp"
#include "battle_display_footer.hpp"
int main(){
    assert(ttyd::loadHarborTutorialAssets("romfs"));
    HarborClassFrameFixture f;assert(f.prepare("romfs"));
    ttyd::DisplayQueue queue;ttyd::BattleFooterCursor cursor;
    assert(!f.scene.registerCommandDisplay(queue,cursor) && queue.count()==0);
    auto palette=std::make_shared<ttyd::BattleCommandPalette>();
    palette->state=2;palette->rgba={192,255,201,207};
    assert(f.scene.bindCommandDisplayPalette(palette));
    assert(f.scene.bindCommandDisplayPalette(palette));
    assert(!f.scene.bindCommandDisplayPalette(std::make_shared<ttyd::BattleCommandPalette>()));
    for(unsigned i=0;i<1024;++i)assert(queue.enter(0,0,0,0,0)==ttyd::DisplayAdmission::Registered);
    assert(!f.scene.registerCommandDisplay(queue,cursor));
    assert(cursor.active && palette->rgba[2]==202);
    // Holding the queue never repeats the already completed source mutation.
    assert(!f.scene.registerCommandDisplay(queue,cursor));
    assert(palette->rgba[2]==202);
    queue.reinitialize();assert(f.scene.registerCommandDisplay(queue,cursor));
    assert(!cursor.active && palette->rgba[2]==202 && queue.count()==2);
    assert(f.word(0x80416d20)==0xc0ffcacf);
    const auto* hp=queue.record(0);const auto* menu=queue.record(1);
    assert(hp->camera==8 && hp->drawClass==0 && hp->callback==0x8011fde0 && hp->userdata==0x84000000);
    assert(menu->camera==8 && menu->drawClass==0 && menu->callback==0x8012210c && menu->userdata==0x84000000);
    assert(hp->key==400000.f-902 && menu->key==400000.f-500);
    // Registration itself does not execute DrawMainMenu or mark source ready.
    assert(f.source.byte(f.source.user,f.binding.allocation+0x14)==0);
    queue.reinitialize();assert(f.scene.registerCommandDisplay(queue,cursor));
    assert(palette->rgba[2]==203 && f.word(0x80416d20)==0xc0ffcbcf);
}
