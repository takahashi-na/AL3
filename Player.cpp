#include "Player.h"
#include "ViewProjection.h"
#include "TextureManager.h"

// コンストラクタ
Player::Player()
{

};

// デストラクタ
Player::~Player()
{ 
	delete modelPlayer_; 
};

// 初期化
void Initialize(ViewProjection view) 
{
	ViewProjection viewProjection_; 

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// インプットクラス
	Input_ = Input::GetInstance();
}

// 更新
void Player::Update()
{
	// 変換行列を更新
	worldTransformPlayer_.matworld_ = MakeAffineMatrix(
	    worldTransformPlayer.scale_, 
		worldTransformPlayer.rotation_,
	    worldTransformPlayer_.transform_);
	// 変換行列を定数バッファに転送
	worldTransformPlayer_.Transfermatrix();
};

// 3D表示
void Player::Draw3D()
{
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
};