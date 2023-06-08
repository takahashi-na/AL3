#include "Stage.h"
#include "TextureManager.h"
#include "ViewProjection.h"

// コンストラクタ
Stage::Stage() {}

// デストラクタ
Stage::~Stage() 
{ 
	delete spriteBG_;
}

// 初期化
void Stage::Inisialize(ViewProjection view)
{
	// BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// メンバー変数に代入
	viewProjection_ = view;
}

// 更新
void Stage::Update() {}

// 2D背景描画
void Stage::Draw2DFar() 
{ 
	spriteBG_->Draw(); 
}
