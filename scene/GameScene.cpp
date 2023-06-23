#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	// クラスの削除
	delete stage_;   // ステージ
	delete player_;  // プレイヤー
	delete beam_;    // ビーム
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 各クラスの生成
	stage_ = new Stage();    // ステージ
	player_ = new Player();  // プレイヤー
	beam_ = new Beam();      // ビーム
	enemy_ = new Enemy();

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// 各クラスの初期化
	stage_->Initialize(viewProjection_);  // ステージ
	player_->Initialize(viewProjection_); // プレイヤー
	beam_->Initialize(viewProjection_,player_);   // ビーム
	enemy_->Initialize(viewProjection_);

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

void GameScene::Update() 
{ 
	stage_->Update(); // 更新 
	player_->Update();
	beam_->Update();
	enemy_->Update();
	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる
	// 背景の描画
	stage_->Draw2DFar();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	stage_->Draw3D();  // ステージ
	player_->Draw3D(); // プレイヤー
	beam_->Draw3D();
	enemy_->Draw3D();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// ここに前景スプライトの描画処理を追加できる
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);

	debugText_->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

// 衝突判定(プレイヤーと敵)　後で更新から呼び出す。呼び出したらこの文は消す
void GameScene::CollisionPlayerEnemy()
{
	// 敵が存在すれば
	if (enemy_->GetFlag() == 1)
	{
		// 差を求める
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		// 衝突したら
		if (dx < 1 && dz < 1)
		{
			// 衝突処理
			enemy_->Hit();
			playerLife_ -= 1;
		}
	}
}

void GameScene::CollisionBeamEnemy() 
{
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
