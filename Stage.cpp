#include "Stage.h"

// コンストラクタ
Stage::Stage() {}

// デストラクタ
Stage::~Stage() 
{
	delete spriteBG_;    // BG 
	delete modelStage_;  // ステージ
}

// 初期化
void Stage::Initialize(ViewProjection view)
{
	// メンバ変数に代入
	viewProjection_ = view;

	// BG
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	// ステージの位置を変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	// 変換行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, 
		worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);

	// 変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();
}

// 更新
void Stage::Update() {}

// 2D背景描画
void Stage::Draw2DFar() 
{ 
	spriteBG_->Draw(); 
}

void Stage::Draw3D() 
{
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
}
