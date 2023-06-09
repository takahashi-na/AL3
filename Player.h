#pragma once
#include "ViewProjection.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class Player
{
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize(ViewProjection ViewProjection);

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
};
