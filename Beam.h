#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"

#include "Player.h"

class Beam 
{
public:
	// コンストラクタ
	Beam();

	// デストラクタ
	~Beam();

	// 初期化
	void Initialize(ViewProjection view,Player* player);

	// 更新
	void Update();

	// 3D背景描画
	void Draw3D();

	// 移動
	void Move();

	// 発生（発射）
	void Born();

	// 衝突処理
	void Hit() { aliveFlag_ = 0; }

	// 存在フラグ
	int aliveFlag_ = 0;

	// X座標の獲得
	float GetX() { return worldTransformBeam_.translation_.x; }

	// Z座標の獲得
	float GetZ() { return worldTransformBeam_.translation_.z; }

private:
	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	// インプットクラス
	Input* input_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
};
