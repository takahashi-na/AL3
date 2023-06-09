#include "Player.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Input.h"

// �R���X�g���N�^
Player::Player()
{

};

// �f�X�g���N�^
Player::~Player()
{ 
	delete modelPlayer_; 
}

// ������
void Player::Initialize(ViewProjection view) 
{
	// �����o�[�ϐ��ɑ��
	viewProjection_ = view;

	// �v���C���[
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// �E�ֈړ�
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	// ���ֈړ�
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4);
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4);

	// �C���v�b�g�N���X
	input_ = Input::GetInstance();
}

// �X�V
void Player::Update()
{
	// �ϊ��s����X�V
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, 
		worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	// �ϊ��s���萔�o�b�t�@�ɓ]��
	worldTransformPlayer_.TransferMatrix();
};

// 3D�\��
void Player::Draw3D()
{
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
};