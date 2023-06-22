#include "Beam.h"
#include "Player.h"

// コンストラクタ
Beam::Beam() {}

// デストラクタ
Beam::~Beam() 
{ 
	delete modelBeam_; // ビーム
}

// 初期化
void Beam::Initialize(ViewProjection view,Player* player) 
{
	// メンバ変数に代入
	viewProjection_ = view;

	// ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformBeam_.Initialize();

	player_ = player;
	input_ = Input::GetInstance();
}

// 更新
void Beam::Update() 
{
	// 移動
	Move();

	// 発生
	Born();

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

void Beam::Move() 
{
	// 奥へ移動
	if (aliveFlag_ == 0)
		return;
	worldTransformBeam_.rotation_.x += 0.1f;
	worldTransformBeam_.translation_.z += 0.3f;

	// 画面外に行ったら
	if (worldTransformBeam_.translation_.z >= 40) {
		aliveFlag_ = 0;
	}
}

void Beam::Born() 
{
	if (aliveFlag_ == 0) 
	{
		if (input_->TriggerKey(DIK_SPACE)) 
		{
			aliveFlag_ = 1;
			worldTransformBeam_.translation_.x = player_->GetX();
			worldTransformBeam_.translation_.z = player_->GetZ();
		}
	}
}