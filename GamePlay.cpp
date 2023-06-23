#include "GamePlay.h"

// コンストラクタ
GamePlay::GamePlay(){}

// デストラクタ
GamePlay::~GamePlay() 
{
	// クラスの削除
	delete stage_;  // ステージ
	delete player_; // プレイヤー
	delete beam_;   // ビーム
	delete enemy_;
}

// 初期化
void GamePlay::Initialize(ViewProjection view) 
{
	// メンバ変数に代入
	viewProjection_ = view;


	// 各クラスの生成
	stage_ = new Stage();   // ステージ
	player_ = new Player(); // プレイヤー
	beam_ = new Beam();     // ビーム
	enemy_ = new Enemy();

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// 各クラスの初期化
	stage_->Initialize(viewProjection_);         // ステージ
	player_->Initialize(viewProjection_);        // プレイヤー
	beam_->Initialize(viewProjection_, player_); // ビーム
	enemy_->Initialize(viewProjection_);

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

// 更新
void GamePlay::Update()
{
	stage_->Update(); // 更新
	player_->Update();
	beam_->Update();
	enemy_->Update();

	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}

// 2D背景
void GamePlay::Draw2DFar()
{
	// 背景の描画
	stage_->Draw2DFar();
}


// 3D描画
void GamePlay::Draw3D()
{
	stage_->Draw3D();  // ステージ
	player_->Draw3D(); // プレイヤー
	beam_->Draw3D();   // ビーム
	enemy_->Draw3D();  // 敵
}

// 2D近景
void GamePlay::Draw2DNear()
{
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);

	debugText_->DrawAll();
}


// 衝突判定(プレイヤーと敵)　後で更新から呼び出す。呼び出したらこの文は消す
void GamePlay::CollisionPlayerEnemy() {
	// 敵が存在すれば
	if (enemy_->GetFlag() == 1) {
		// 差を求める
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 衝突処理
			enemy_->Hit();
			playerLife_ -= 1;
		}
	}
}

void GamePlay::CollisionBeamEnemy() {
	// 敵が存在すれば
	if (enemy_->GetFlag() == 1) {
		// 差を求める
		float dx = abs(beam_->GetX() - enemy_->GetX());
		float dz = abs(beam_->GetZ() - enemy_->GetZ());

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 衝突処理
			enemy_->Hit();
			beam_->Hit();

			gameScore_ += 100;
		}
	}
}
