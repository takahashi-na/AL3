﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"

#include "DebugText.h"

// ゲームシーン
class GameScene {

public: // メンバ関数
	// コンストクラタ
	GameScene();
	
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// 衝突判定(プレイヤーと敵)
	void CollisionPlayerEnemy();

	// 衝突判定(ビームと敵)
	void CollisionBeamEnemy();

	// ゲームスコア
	int gameScore_ = 0;     

	// プレイヤーライフ
	int playerLife_ = 3;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beam_ = nullptr;     // びーむ
	Enemy* enemy_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
};
