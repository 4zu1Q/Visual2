#pragma once
#include "DxLib.h"

// �Q�[���S�̂Ŏg�p����萔�̒�`
namespace Game
{

	//�E�B���h�E���[�h�ݒ�
#ifdef _DEBUG 
	constexpr bool kWindowMode = false;
#else
	constexpr bool kWindowMode = false;
#endif

	//�E�B���h�E��
	const char* const kTitleText = "�Q�[����";


	//�E�B���h�E�T�C�Y
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;

	//constexpr int kScreenWidth = 1920;
	//constexpr int kScreenHeight = 1080;


	//�E�B���h�E�J���[���[�h
	constexpr int kColorDepth = 32; //16 or 32

}
