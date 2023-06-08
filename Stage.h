#pragma once
#include"Sprite.h"

class Stage 
{
public:
	// コンストラクタ
	Stage();

	// デストラクタ
	~Stage();

	// 初期化
	void Inisialize(ViewProjection view);

	ViewProjection viewProjection_;

	// 更新
	void Update();

	// 2D背景描画
	void Draw2DFar();

private:
	// BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;
};
