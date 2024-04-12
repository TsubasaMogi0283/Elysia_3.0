#pragma once

const float BPM = 150.0f;

const float SECOND = 60.0f;

//今回は2.5
const float TIMES = BPM / SECOND;

const float TIMES2 = 1 / TIMES;

//1拍は↓の時間かかる
//今回は24だった
const float BEAT1 = SECOND *TIMES2;

//１小節は4掛けるだけで良い
const float BAR1 = BEAT1 * 4;


//float bpm = 150.0;
//float second = 60.0f;
//float times = bpm / second;
//float times2 = (1 / times);
//float beat = second * times2;
//float bar = beat * 4;

//second / (1 / times);



//こういう計算になった理由

//まずBPM:60だった場合timeは60で1拍である
//BPM:120だった場合timeは30で１拍

//もしかしたら反比例の関係ではないだろうか
//BPMが２倍になるとtimeは(1/2)倍になるから
//反例が見つからないので、この仮定は正しいと言えるかも

//例
//60*x=120			time=60*(1/2)
//   x=2				=30


//実際にBPM:150でやってみると
//60*x=150          time=60*(1/x)
//   x=2.5				=24



