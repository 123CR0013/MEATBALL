#include "DxLib.h"
#include "fps.h"

// フレームレートを60に固定する
// 参考サイト: 
// https://dixq.net/rp/43.html
// https://dixq.net/forum/viewtopic.php?t=4295 


//fps
#define FRAME 60

//fpsのカウンタ、60フレームに1回基準となる時刻を記録する変数
int fps_count, count0t;
//平均を計算するため60回の1周時間を記録
int f[FRAME];
//平均fps
double ave;

//FLAME fps になるようにfpsを計算・制御
void fps_wait() {
    int term, i, gnt;
    static int t = 0;
    if (fps_count == 0) {//60フレームの1回目なら
        if (t == 0)//完全に最初ならまたない
            term = 0;
        else//前回記録した時間を元に計算
            term = count0t + 1000 - GetNowCount();
    }
    else    //待つべき時間=現在あるべき時刻-現在の時刻
        term = (int)(count0t + fps_count * (1000.0 / FRAME)) - GetNowCount();

    if (term > 0)//待つべき時間だけ待つ
        Sleep(term);

    gnt = GetNowCount();

    if (fps_count == 0)//60フレームに1度基準を作る
        count0t = gnt;
    f[fps_count] = gnt - t;//１周した時間を記録
    t = gnt;
    //平均計算
    if (fps_count == FRAME - 1) {
        ave = 0;
        for (i = 0; i < FRAME; i++)
            ave += f[i];
        ave /= FRAME;
    }
    fps_count = (++fps_count) % FRAME;
}

//x,yの位置にfpsを表示
void draw_fps(int x, int y) {
    if (ave != 0) {
        DrawFormatString(x, y, GetColor(255, 255, 255), "[%.1f]", 1000 / ave);
    }
}
