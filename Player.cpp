﻿#include "Player.h"

// コンストラクタ
Player::Player() {}

// デストラクタ
Player::~Player() 
{ 
	delete modelPlayer_;  // プレイヤー
}

// 初期化
void Player::Initialize(ViewProjection view)
{
	// メンバ変数に代入
	viewProjection_ = view;

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

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
}

// 3D描画
void Player::Draw3D() 
{
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}
