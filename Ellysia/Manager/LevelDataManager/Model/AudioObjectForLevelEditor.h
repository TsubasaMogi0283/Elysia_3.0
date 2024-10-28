#pragma once
#include <memory>
#include <cstdint>

#include "IObjectForLevelEditor.h"
#include "AudioObjectForLevelEditorCollider.h"


#include <Audio.h>

struct Camera;
struct Material;
struct DirectionalLight;
struct PointLight;
struct SpotLight;

//オーディオ用のオブジェクト
class AudioObjectForLevelEditor :public IObjectForLevelEditor {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioObjectForLevelEditor()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const uint32_t& modelhandle, const Vector3& position) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// オーディオのファイルパスを入力
	/// </summary>
	/// <param name="filePath"></param>
	void SetAudio(const std::string& filePath);

#pragma region 描画

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="camera">カメラ</param>
	/// <param name="directionalLight">平行光源</param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="pointLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight);



private:
	//オーディオ
	std::unique_ptr<Audio>audio_ = nullptr;
	//ハンドル
	uint32_t audioHandle_ = 0u;

	std::unique_ptr<AudioObjectForLevelEditorCollider> audioObjectForLevelEditorCollider_ = nullptr;

};

