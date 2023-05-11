#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"
#include "time.h"

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete spriteBG_;    // BG
    delete modelStage_;  // ステージ
	delete modelPlayer_; // プレイヤー
	delete modelBeam_;   // ビーム
	delete modelEnemy_;  // 敵
}

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

    // ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	// ステージの位置を変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	// 変換行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, 
		worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();

	// 敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();

	// 乱数の初期化
	srand((unsigned int)time(NULL));

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

// 更新
void GameScene::Update()
{
	switch (sceneMode_)
	{
	case 0:
	    GamePlayUpdate();  // ゲームプレイ更新
		break;
	}
}

void GameScene::PlayerUpdate(){
	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	worldTransformPlayer_.scale_,
	worldTransformPlayer_.rotation_,
	worldTransformPlayer_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();

	// 右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	// 左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4);
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4);
}

// ビーム更新
void GameScene::BeamUpdate() {
	BeamMove();
	BeamBorn();

	// 変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	worldTransformBeam_.scale_, 
	worldTransformBeam_.rotation_,
	worldTransformBeam_.translation_);

	// 変換行列を定数バッファに転送
	worldTransformBeam_.TransferMatrix();
}

// ビーム移動
void GameScene::BeamMove(){
	// 奥へ移動
	if (isBeamFlag_ == false)return;
	worldTransformBeam_.rotation_.x += 0.1f;
	worldTransformBeam_.translation_.z += 0.3f;

	// 画面外に行ったら
	if (worldTransformBeam_.translation_.z >= 40) 
	{
		isBeamFlag_ = false;
	}
}

// ビーム発生(発射)
void GameScene::BeamBorn() 
{
	if (isBeamFlag_ == true)return;
	if (input_->TriggerKey(DIK_SPACE))
	{
		isBeamFlag_ = true;
		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
	}
}

// 敵更新
void GameScene::EnemyUpdate()
{
	EnemyMove();
	EnemyBorn();

	// 変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	worldTransformEnemy_.translation_);

	// 変換行列を定数バッファに転送
	worldTransformEnemy_.TransferMatrix();
}

// 敵移動
void GameScene::EnemyMove() 
{
	if (isEnemyFlag_ == true)
	{
		worldTransformEnemy_.rotation_.x -= 0.15f;
		worldTransformEnemy_.translation_.z -= 0.5f;
	}

	// 画面端処理
	if (worldTransformEnemy_.translation_.z < -5)
	isEnemyFlag_ = false;
}

// 敵生成
void GameScene::EnemyBorn()
{
	if (isEnemyFlag_ == false)
	{ 
		isEnemyFlag_ = true; 

		// 乱数処理
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;

		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = 40.f;
	}

}

// 衝突判定
void GameScene::Collision()
{ 
	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}

// 衝突判定(プレイヤーと敵)
void GameScene::CollisionPlayerEnemy() {
	// 敵が存在すれば
	if (isEnemyFlag_ == true) {
		// 差を求める
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);
	    
		// 衝突したら
		if (dx < 1 && dz < 1)
		{
			// 存在しない
			isEnemyFlag_ = false;
			playerLife_ -= 1;
			if (playerLife_ < 0)
			{
				playerLife_ = 3;
			}
		}
	}
}

void GameScene::CollisionBeamEnemy() { 
	// ビームと敵が両方とも存在するなら
	if (isBeamFlag_ == true && isEnemyFlag_)
	{
		// 差を求める
		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			isBeamFlag_ = false;
			isEnemyFlag_ = false;

			gameScore_ += 1;
		}
	}
}

// 描画
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	
	// 各シーンの背景2D表示を呼び出す
	switch (sceneMode_) {
	case 0:
	    GamePlayDraw2DBack();   // ゲームプレイ2D表示
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

    // 各シーンの背景3D表示を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayDraw3D(); // ゲームプレイ3D表示
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList)
;
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
		break;
	}
	debugText_->DrawAll();
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

// ゲームプレイ更新
void GameScene::GamePlayUpdate() 
{
	// 自機更新
	PlayerUpdate();

	// 弾更新
	BeamUpdate();

	// 敵更新
	EnemyUpdate();

	// 当たり判定
	Collision();
}

// ゲームプレイ表示3D
void GameScene::GamePlayDraw3D() {
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	if (isBeamFlag_ == true)
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);

	// 敵
	if (isEnemyFlag_ == true)
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
}

// ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() {
	// 背景
	spriteBG_->Draw();
}

// ゲームプレイ近景2D表示
void GameScene::GamePlayDraw2DNear() {
	// score
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);
}