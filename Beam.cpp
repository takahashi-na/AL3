#include "Beam.h"

// コンストラクタ
Beam::Beam() {}

// デストラクタ
Beam::~Beam() 
{ 
	delete modelBeam_; // ビーム
}

// 初期化
void Beam::Initialize(ViewProjection view) 
{
	// メンバ変数に代入
	viewProjection_ = view;

	// ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformBeam_.Initialize();
}

// 更新
void Beam::Update() 
{
	// 変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_,
		worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	// 変換行列を定数バッファに転送
	worldTransformBeam_.TransferMatrix();
}

// 3D描画
void Beam::Draw3D() 
{
	modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_); }

void Beam::Move() {}

void Beam::Born() {}
