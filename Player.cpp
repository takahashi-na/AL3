#include "Player.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Input.h"

// コンストラクタ
Player::Player()
{

};

// デストラクタ
Player::~Player()
{ 
	delete modelPlayer_; 
}

// 初期化
void Player::Initialize(ViewProjection view) 
{
	// メンバー変数に代入
	viewProjection_ = view;

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// 右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	// 左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4);
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4);

	// インプットクラス
	input_ = Input::GetInstance();
}

// 更新
void Player::Update()
{
	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, 
		worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
};

// 3D表示
void Player::Draw3D()
{
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
};