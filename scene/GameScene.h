#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Stage.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Stage* stage_ = nullptr;  // ステージ

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
};
