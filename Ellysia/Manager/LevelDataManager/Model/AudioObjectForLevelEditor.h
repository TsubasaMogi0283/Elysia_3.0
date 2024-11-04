#pragma once
#include <memory>
#include <cstdint>

#include "IObjectForLevelEditor.h"
#include "AudioObjectForLevelEditorCollider.h"
#include "Audio.h"

struct Camera;
struct Material;
struct DirectionalLight;
struct PointLight;
struct SpotLight;

/// <summary>
/// オーディオ用のオブジェクト
/// </summary>
class AudioObjectForLevelEditor :public IObjectForLevelEditor {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioObjectForLevelEditor()=default;


	/// <summary>
	/// オーディオのハンドルを設定
	/// 初期化より先でやってね
	/// </summary>
	inline void SetAudio(uint32_t audioHandle) {
		this->audioHandle_ = audioHandle;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelhandle"></param>
	/// <param name="transform"></param>
	void Initialize(const uint32_t& modelhandle, const Transform& transform) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	

#pragma region 描画

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="pointLight"></param>
	void Draw(const Camera& camera,const Material& material,const PointLight& pointLight)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const Material& material,const SpotLight& spotLight)override;



	//オーディオ
private:
	Audio* audio_ = nullptr;
	//ハンドル
	uint32_t audioHandle_ = 0u;
	//当たり判定
	std::unique_ptr<AudioObjectForLevelEditorCollider> audioObjectForLevelEditorCollider_ = nullptr;

};