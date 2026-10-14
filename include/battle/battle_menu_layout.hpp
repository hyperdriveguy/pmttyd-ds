#pragma once
namespace ttyd {
// Native vertical reflow of the original weapon/target submenu. Physical
// navigation remains owned by TrioBattleCommands; touch is an optional shortcut.
struct BattleMenuLayout {
    static constexpr int Left=18,Right=302,Top=92,RowHeight=30;
    static int touchedRow(int x,int y,int count) {
        if(x<Left || x>=Right || y<Top || y>=Top+count*RowHeight)return -1;
        return (y-Top)/RowHeight;
    }
};
}
