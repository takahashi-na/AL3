#include "Player.h"
#include "ViewProjection.h"
#include "TextureManager.h"

// �R���X�g���N�^
Player::Player()
{

};

// �f�X�g���N�^
Player::~Player()
{ 
	delete modelPlayer_; 
};

// ������
void Initialize(ViewProjection view) 
{
	ViewProjection viewProjection_; 

	// �v���C���[
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// �C���v�b�g�N���X
	Input_ = Input::GetInstance();
}

// �X�V
void Player::Update()
{
	// �ϊ��s����X�V
	worldTransformPlayer_.matworld_ = MakeAffineMatrix(
	    worldTransformPlayer.scale_, 
		worldTransformPlayer.rotation_,
	    worldTransformPlayer_.transform_);
	// �ϊ��s���萔�o�b�t�@�ɓ]��
	worldTransformPlayer_.Transfermatrix();
};

// 3D�\��
void Player::Draw3D()
{
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
};