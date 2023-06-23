#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


#include "GamePlay.h"

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
	GamePlay* gamePlay_ = nullptr;

	int sceneMode_ = 0;    // シーンモード(0:ゲームプレイ  1:タイトル)

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;
};
