#pragma once

namespace Game
{
	// �E�C���h�E���[�h�ݒ�
#ifdef _DEBUG
	constexpr bool kDefaultWindowMode = true;
#else
	constexpr bool kDefaultWindowMode = false;
#endif
	// �E�C���h�E��
	const char* const kTitleText = "������Scene�����̃T���v��";
	// �E�C���h�E�T�C�Y
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	// �J���[���[�h
	constexpr int kColorDepth = 32;		// 32 or 16

	// �t���[�����[�g
	constexpr int kFps = 60;
	// 1�t���[���̎���(�}�C�N���b)
	constexpr int kFrameMicroSec = 1000000 / kFps;
};