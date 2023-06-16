#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"

class Enemy 
{
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化
	void Initialize(ViewProjection view);

	// 更新
	void Update();

	// 2D背景描画
	void Draw3D();

	// 移動
	void Move();

	// 発生（発射）
	void Born();

	// 存在フラグ
	int aliveFlag_ = 0;

private:
	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
};
