#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"

class Player
{
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize(ViewProjection View);

	// �X�V
	void Update();

	// 2D�w�i�`��
	void Draw3D();

private:
	Input* input_ = nullptr;

	// �r���[�v���W�F�N�V����(����)
	ViewProjection viewProjection_;

	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
};
