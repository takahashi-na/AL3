#include "Stage.h"
#include "TextureManager.h"

// コンストラクタ
Stage::Stage() {}

// デストラクタ
Stage::~Stage() 
{ 
	delete spriteBG_;
	delete modelStage_;
}

// 初期化
void Stage::Initialize(ViewProjection view)
{
	// BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// メンバー変数に代入
	viewProjection_ = view;

	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();
}

// 更新
void Stage::Update() {}

// 2D背景描画
void Stage::Draw2DFar() 
{ 
	spriteBG_->Draw(); 
}

// 3D描画
void Stage::Draw3D() 
{
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
}
