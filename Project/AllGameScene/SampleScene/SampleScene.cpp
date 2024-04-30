#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>

#include "ModelManager.h"
/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	
	//GLTF2.0
	//「GLTF Separate(.gltf+bin+Texture)」、「オリジナルを保持」で
	modelHandle =ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf",true);

	//後々AnimationManagerを作ってここで読み込みたい
	//animationHandle = Animation::GetInstance()->LoadAnimationFile("Resources/CG4/AnimatedCube", "AnimatedCube.gltf");

	skeleton_ = CreateSkeleton(ModelManager::GetInstance()->GetModelData(modelHandle).rootNode);

	Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	model_.reset(Model::Create(modelHandle));
	
	Matrix4x4 localMatrix = ModelManager::GetInstance()->GetModelData(modelHandle).rootNode.localMatrix;

	worldTransform_.Initialize(true, localMatrix);
	camera_.Initialize();

	
#pragma region 後々クラス化する
	const std::string fullpath = "Resources/LevelData/TL1Test.json";
	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);

	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	//objectかどうか
	assert(deserialized.is_object());
	//namaeのキーワードがあるかどうか
	assert(deserialized.contains("name"));
	//nameはstring型かどうか
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =deserialized["name"].get<std::string>();
	//正しいレベルデータファイルはチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスlタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		//各オブジェクトに必ずtypeが入っているよ
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESHの場合
		if (type.compare("MESH") == 0) {
			//要素追加
			//emplace_backというとvectorだね！
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			//Blenderと軸の方向が違うので注意！

			//  自作エンジン      Blender
			//		x		←		y
			//		y		←		z
			//		z		←		-x
			//
			//回転
			//進行方向に向かって...
			//     左回り		右回り
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//コライダーの読み込み

		}


		//再帰関数
		//Python側でやったやつを参考にしてね
		if (object.contains("children")) {

		}


	}

#pragma endregion


}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	

	Matrix4x4 localMatrix = model_->GetAnimationLocalMatrix();
	worldTransform_.Update(localMatrix);
	camera_.Update();


	animationTime_ += 1.0f/60.0f;
	Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	ApplyAnimation(skeleton_, animation, animationTime_);
	SkeletonUpdate(skeleton_);

#ifdef _DEBUG
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_.rotate_.x, -30.0f, 30.0f);
	ImGui::End();

#endif

	AdjustmentItems::GetInstance()->SaveFile(GroupName);
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	//AnimationManagerを作った方が良いかも引数を増やすの嫌だ。
	Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	model_->Draw(worldTransform_, camera_, animation);
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}