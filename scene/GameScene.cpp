﻿#include "GameScene.h"
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

}

void GameScene::Update() 
{ 
	stage_->Update(); // 更新 
	player_->Update();
	beam_->Update();
	enemy_->Update();
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

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
