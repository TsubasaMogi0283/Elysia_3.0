[![DebugBuild](https://github.com/TsubasaMogi0283/Ellysia_3.0/actions/workflows/DebugBuild.yml/badge.svg?branch=master)](https://github.com/TsubasaMogi0283/Ellysia_3.0/actions/workflows/DebugBuild.yml)

[![ReleaseBuild](https://github.com/TsubasaMogi0283/Ellysia_3.0/actions/workflows/ReleaseBuild.yml/badge.svg?branch=master)](https://github.com/TsubasaMogi0283/Ellysia_3.0/actions/workflows/ReleaseBuild.yml)

# 作品

タイトル: 静寂の霊園  
制作:茂木 翼  

<br>

## ゲームについて

キーボードまたはコントローラーを使って遊ぶ<span style="color: #dc143c; ">**3Dホラーアクションゲーム**</span>です。徘徊するお化けに襲われないよう気を付けながら、ステージ上にある鍵を探し脱出を目指します。
お化けはプレイヤーが持つライトで照らすことで倒すことが出来ます。



![静寂の霊園_プレイ2](https://github.com/user-attachments/assets/05c114d1-279a-4c94-8006-91e9f91ad70f)
https://youtu.be/Biu44fPONeg

<br>


2. 頑張ったところ
    * **レベルエディタ**  
        Blenderのアドオン開発をしました。モデルのファイル名を入力と配置をします。必要であれば衝突判定も追加できます。  
        その他にオーディオも追加できます。衝突判定も追加すればそこに対応した音が流れます。

        ![BlenderAudioSetting](https://github.com/user-attachments/assets/3d5ce673-13a4-40f1-acd9-92e2c968d49d)


    * **衝突判定**  
        特に敵同士の衝突判定を頑張りました。ただ座標と半径を取得するのではなく、進行方向に同じ敵がいたら止まる、いなくなったらまた進むというものを実装しました。

        ![EnemyCollisionSystem](https://github.com/user-attachments/assets/2fb3fd33-74b2-4188-b487-529a4da8de3f)



<br>
<br>

# Elysia Engine

このエンジンは私がDirectX12を使い制作したものになります。

<br>

## 機能

* スプライト、3Dモデル、パーティクルの描画  
* スキニングアニメーションに対応した3Dモデル
* キーボード、コントローラのボタン対応  
* 音の機能
* ポストエフェクト



## 特徴

音楽制作している所からオーディオ面を頑張りたいと思い力を入れました。  
XAudio2を使い再生と停止の基本機能に加え、ループの細かい設定や様々なエフェクトを加えることが出来ます。



1. 対応ファイル
    * Wave
    * MP3

    MP3は**MediaFundation**を使い読み込んでいます。  
    以下の関数を使って読み込みをします。拡張子によって自動的に振り分けられます。
    

    ```c++
    uint32_t Audio::Load(const std::string& fileName){

	    //一度読み込んだものは２度読み込まず返すだけ
    	if (Audio::GetInstance()->audioInformation_.find(fileName) != Audio::GetInstance()->audioInformation_.end()) {  
    		return Audio::GetInstance()->audioInformation_[fileName].handle;
    	}

    	//拡張子を探す
    	size_t dotPosition = fileName.find('.');
    	std::string extension = {};
    	if (dotPosition != std::string::npos) {
    		//「/」から最後まで
    		extension = fileName.substr(dotPosition + 1);
    	}


    	//返す値
    	uint32_t result = 0u;

    	//wavの場合
    	if (extension == "wav") {
    		return result = LoadWave(fileName);
    	}
    	//mp3の場合
    	else if (extension == "mp3") {
    		return result = LoadMP3(fileName);
    	}


    	//無かったら0uを返す
    	return result;

    }

    ```



2. ループ機能について
    1回だけまたはずっと繰り返すものと指定回数分ループする2つの再生があります。  
    
    しかし<span style="color: #00fa9a; ">XAUDIO2_BUFFER</span>にあるLoopCountが個人的に分かりずらいと感じました。
    
    <span style="color: #7b68ee; ">XAUDIO2_LOOP_INFINITE</span>はずっとループ、
    <span style="color: #7b68ee; ">XAUDIO2_NO_LOOP_REGION</span>は1回だけ流れます。
    中身の値はそれぞれ255,0となっています。
    このことから直接何回ループさせたいと数を入力すると1回多くなってしまい直感的に分かりずらいです。
    分かりやすくするために指定回数分再生する関数の中では「xAudio2Buffer.LoopCount = loopCount - 1;」としています。




<br>



## 使用した外部ライブラリ
* DirectXTex
* nlohmann-json
* Assimp
* ImGui
