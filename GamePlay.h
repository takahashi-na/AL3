#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"

#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"

#include "DebugText.h"

class GamePlay 
{
public:
	// コンストラクタ
	GamePlay();

    // デストラクタ
	~GamePlay();

	// 初期化
	void Initialize(ViewProjection view);

	// 更新
	void Update();

	// 2D背景描画
	void Draw2DFar();

	// 3D
	void Draw3D();

	// 2D近景描画
	void Draw2DNear();

	// 衝突判定(プレイヤーと敵)
	void CollisionPlayerEnemy();

	// 衝突判定(ビームと敵)
	void CollisionBeamEnemy();

private:
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beam_ = nullptr;     // びーむ
	Enemy* enemy_ = nullptr;   // 敵

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	int gameScore_ = 0;  // ゲームスコア
	int playerLife_ = 3; // プレイヤーライフ

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
};
