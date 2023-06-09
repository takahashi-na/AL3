#pragma once
#include "ViewProjection.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化
	void Initialize(ViewProjection ViewProjection);

	// 更新
	void Update();

	// 2D背景描画
	void Draw3D();

private:
	Input* input_ = nullptr;

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
};
