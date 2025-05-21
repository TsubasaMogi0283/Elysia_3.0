#include "Emitter.h"


void Emitter::Update(){
	frequencyTime += DELTA_TIME;

	//最初に1回だけ出す
	if (isFirstRelease_ == false) {
		//出し終わったことを示す
		isFirstRelease_ = true;

	}

	//そのあと循環
	//時間経過
	//頻度より大きいかつまだ生成が停止されていない
	if (frequency <=frequencyTime && isStopGenerate_ == false) {
		//もう一度最初から
		frequencyTime = 0.0f;
	}

}
