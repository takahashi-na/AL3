#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"
#include "time.h"

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() 
{
	delete spriteBG_;         // BG
    delete modelStage_;       // ステージ
	delete modelPlayer_;      // プレイヤー
	delete modelBeam_;        // ビーム
	delete modelEnemy_;       // 敵
	delete spriteTitle_;      // タイトル
	delete spriteEnter_;      // エンター
	delete spriteGameOver_;   // ゲームオーバー
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
	for (int i = 0; i < 10; i++) 
	{
		worldTransformBeam_[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldTransformBeam_[i].Initialize();
	}

	// 敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	for (int i = 0; i < 10; i++)
	{
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}

	// タイトル(2Dスプライト)
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	// タイトル(エンター)
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 450});

	// ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 100});

	// 乱数の初期化
	srand((unsigned int)time(NULL));

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

	// サウンドデータの読み込み
	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

	// タイトルBGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
}

// 更新
void GameScene::Update()
{
	switch (sceneMode_)
	{
	case 0:
	    GamePlayUpdate();   // ゲームプレイ更新
		break;			   
						   
	case 1:				   
		TitleUpdate();      // タイトル更新
		break;			   

	case 2:				   
		GameOverUpdate();   // ゲームオーバー画面更新
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
	for (int i = 0; i < 10; i++) 
	{
		worldTransformBeam_[i].matWorld_ = MakeAffineMatrix(
		worldTransformBeam_[i].scale_, worldTransformBeam_[i].rotation_,
		worldTransformBeam_[i].translation_);

		// 変換行列を定数バッファに転送
		worldTransformBeam_[i].TransferMatrix();
	}
}

// ビーム移動
void GameScene::BeamMove(){
	// 奥へ移動,回転
	for (int i = 0; i < 10; i++) {
		if (isBeamFlag_[i] == false)continue;
		worldTransformBeam_[i].rotation_.x += 0.1f;
		worldTransformBeam_[i].translation_.z += 0.3f;

		// 画面外に行ったら
		if (worldTransformBeam_[i].translation_.z >= 40) {
			isBeamFlag_[i] = false;
			worldTransformBeam_[i].translation_.z = 0;
		}
	}
}

// ビーム発生(発射)
void GameScene::BeamBorn() 
{
	for (int i = 0; i < 10; i++) 
	{
		if (isBeamFlag_[i] == true)
			continue;

		//

		// 発射タイマーが0ならば
		if (beamTimer_ == 0) {

			if (input_->TriggerKey(DIK_SPACE))
			{
				isBeamFlag_[i] = true;
				worldTransformBeam_[i].translation_.x = worldTransformPlayer_.translation_.x;
				worldTransformBeam_[i].translation_.z = worldTransformPlayer_.translation_.z;
				break;
			}
		}
		else
		{
			// 発射タイマーが1以上
			// 10を越えると再び発射可能
			beamTimer_++;
		}
		if (beamTimer_ > 10) {
			beamTimer_ = 0;
		}
	}
}

// 敵更新
void GameScene::EnemyUpdate()
{
	EnemyMove();
	EnemyBorn();

	// 変換行列を更新
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
	    worldTransformEnemy_[i].translation_);

		// 変換行列を定数バッファに転送
		worldTransformEnemy_[i].TransferMatrix();
	}
}

// 敵移動
void GameScene::EnemyMove() 
{
	for (int i = 0; i < 10; i++) {
		if (isEnemyFlag_[i] == true) {
			worldTransformEnemy_[i].rotation_.x -= 0.15f;
			worldTransformEnemy_[i].translation_.z -= 0.5f;
			worldTransformEnemy_[i].translation_.x += enemySpeed_[i];

			if (worldTransformEnemy_[i].translation_.x > 4)
			{
				enemySpeed_[i] = -0.1f;
			} 
			else if (worldTransformEnemy_[i].translation_.x < -4)
			{
				enemySpeed_[i] = 0.1f;
			}
		}

		// 画面端処理
		if (worldTransformEnemy_[i].translation_.z < -5)
			isEnemyFlag_[i] = false;
	}
}

// 敵生成
void GameScene::EnemyBorn()
{
	// 乱数で発生
	if (rand() % 10 == 0) 
	{
		// 敵の数ループする
		for (int e = 0; e < 10; e++) 
		{
			// 存在しなければ
			if (enemyFlag[e] == 0) 
			{
				
				// 発生処理
				if (isEnemyFlag_[e] == false) 
				{
					isEnemyFlag_[e] = true;

					// 乱数処理
					int x = rand() % 80;
					float x2 = (float)x / 10 - 4;

					worldTransformEnemy_[e].translation_.x = x2;
					worldTransformEnemy_[e].translation_.z = 40.f;

					// 敵のスピード
					if (rand() % 2 == 0) {
						enemySpeed_[e] = 0.1f;
					} else {
						enemySpeed_[e] = -0.1f;
					}

					// ループ終了
					break;
				}
			}
		}
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
	for (int i = 0; i < 10; i++) {
		if (isEnemyFlag_[i] == true) {
			// 差を求める
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);

			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				isEnemyFlag_[i] = false;

				// ライフ減算	
				playerLife_ -= 1;

				// プレイヤーヒットSE
				audio_->PlayWave(soundDataHandlePlayerHitSE_);
			}
		}
	}
}

void GameScene::CollisionBeamEnemy() { 
	
	// 敵の数ループする
	for (int e = 0;e < 10;e++)
	{
		if (isEnemyFlag_[e] == 1)
		{
			// ビームの数ループする
			for (int b = 0; b < 10; b++) 
			{
				// ビームが存在すれば
				if (isBeamFlag_[b] == 1)
				{
					float dx =
					    abs(worldTransformBeam_[b].translation_.x -
					        worldTransformEnemy_[e].translation_.x);
					float dz =
					    abs(worldTransformBeam_[b].translation_.z -
					        worldTransformEnemy_[e].translation_.z);

					// 衝突したら
					if (dx < 1 && dz < 1) {
						// 存在しない
						isBeamFlag_[b] = false;
						isEnemyFlag_[e] = false;

						gameScore_ += 1;

						// エネミーヒットSE
						audio_->PlayWave(soundDataHandleEnemyHitSE_);
					}
				}
			}
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
	case 2:
		GamePlayDraw2DBack(); // ゲームプレイ2D表示
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
	case 2:
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
	case 1:
		TitleDraw2DNear();    // タイトル2D近景表示
		break;
	case 2:
		GameOverDraw2DNear(); // ゲームオーバー2D近景表示
		GamePlayDraw2DNear(); // ゲームプレイ2D表示
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
	if (playerLife_ <= 0)
	{
		sceneMode_ = 2;
	}

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
void GameScene::GamePlayDraw3D() 
{
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	for (int i = 0; i < 10; i++) {
		if (isBeamFlag_[i] == true)
			modelBeam_->Draw(worldTransformBeam_[i], viewProjection_, textureHandleBeam_);
	}
	// 敵
	for (int i = 0; i < 10; i++) {
		if (isEnemyFlag_[i] == true)
			modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
	}
}

// ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() {
	// 背景
	spriteBG_->Draw();
}

// ゲームプレイ近景2D近景
void GameScene::GamePlayDraw2DNear() {
	// score
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);
}

// タイトル更新
void GameScene::TitleUpdate()
{ 
	if (input_->TriggerKey(DIK_RETURN))
	{
		// モードをゲームプレイへ変更
		GamePlayStart();

		for (int e = 0;e < 10;e++)
		{
			isEnemyFlag_[e] = false;
		}
		for (int b = 0; b < 10; b++) {
			isBeamFlag_[b] = false;
		}

		worldTransformPlayer_.translation_.x = 0;

		// BGＭ切り替え
		audio_->StopWave(voiceHandleBGM_);// 現在のＢＧＭを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true); // ゲームプレイBGMを再生
	}
	gameTimer_ += 1;
}

// タイトル表示
void GameScene::TitleDraw2DNear() 
{ 
	spriteTitle_->Draw();

	// エンター表示
	if (gameTimer_ % 40 >= 20)
	{
		spriteEnter_->Draw();
	}
}

// 開始時
void GameScene::GamePlayStart()
{
	playerLife_ = 3;
	sceneMode_ = 0;
	gameScore_ = 0;
}

// ゲームオーバー
void GameScene::GameOverUpdate()
{
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 1;

		// BGＭ切り替え
		audio_->StopWave(voiceHandleBGM_); // 現在のＢＧＭを停止
		voiceHandleBGM_ =audio_->PlayWave(soundDataHandleTitleBGM_, true); // 遷移時にタイトルBGMを再生
	}
	gameTimer_ += 1;
}

void GameScene::GameOverDraw2DNear() 
{ 
	spriteGameOver_->Draw(); 

	// エンター表示
	if (gameTimer_ % 40 >= 20) 
	{
		spriteEnter_->Draw();
	}
}