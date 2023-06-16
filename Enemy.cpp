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
	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
}

void Enemy::Move() {}

void Enemy::Born() {}
