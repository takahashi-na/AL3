#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤー更新
	/// </summary>
	void PlayerUpdate();

	/// <summary>
	/// ビームの更新
	/// </summary>
	void BeamUpdate();

	/// <summary>
	/// ビームの移動
	/// </summary>
	void BeamMove();

	/// <summary>
	/// ビーム発生
	/// </summary>
	void BeamBorn();

	/// <summary>
	/// 敵の更新
	/// </summary>
	void EnemyUpdate();

	/// <summary>
	/// 敵の移動
	/// </summary>
	void EnemyMove();

	/// <summary>
	/// 敵の生成
	/// </summary>
	void EnemyBorn();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void Collision();

	/// <summary>
	/// 衝突判定(プレイヤーと敵)
	/// </summary>
	void CollisionPlayerEnemy();

	/// <summary>
	/// 衝突判定(ビームと敵)
	/// </summary>
	void CollisionBeamEnemy();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	 
	// BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	bool isBeamFlag_ = false;

	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	bool isEnemyFlag_ = false;

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	int gameScore_ = 0;  // ゲームスコア
	int playerLife_ = 3; // プレイヤーライフ

private:
	void GamePlayUpdate();  // ゲームプレイ更新
	void GamePlayDraw3D();	// ゲームプレイ3D表示
	void GamePlayDraw2DBack();	// ゲームプレイ背景2D表示
	void GamePlayDraw2DNear();	// ゲームプレイ近景2D表示

	// シーン遷移用
	int sceneMode_ = 0;  // シーンモード(0:ゲームプレイ　1:タイトル)
};
