#include "Enemy.h"

// コンストラクタ
Enemy::Enemy() {}

// デストラクタ
Enemy::~Enemy() 
{
	delete modelEnemy_; 
}

void Enemy::Initialize(ViewProjection view) 
{
	// メンバ変数に代入
	viewProjection_ = view;

	// プレイヤー
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
}

void Enemy::Update() 
{
	// 変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_,
		worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);

	// 変換行列を定数バッファに転送
	worldTransformEnemy_.TransferMatrix();
}

void Enemy::Draw3D() 
{
	if (aliveFlag_ == 1)
	{
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}

// 敵移動
void Enemy::Move() 
{
	if (aliveFlag_ == 1) {
		worldTransformEnemy_.rotation_.x -= 0.15f;
		worldTransformEnemy_.translation_.z -= 0.5f;
	}

	// 画面端処理
	if (worldTransformEnemy_.translation_.z < -5)
		aliveFlag_ = 0;
}

// 敵生成
void Enemy::Born() 
{ 
	if (aliveFlag_ == 0) 
	{
		 aliveFlag_ = 1;
		int x = rand() % 80;
		float x2 = (float)x/10-4;

		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = 40.0f;
    }
}
