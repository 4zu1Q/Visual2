#ifdef _DEBUG
#pragma once

// �f�o�b�O�@�\�\�����Ɏg�p����p�b�h����	1�R��&�L�[�{�[�h�̂ݑΉ�
// (�f�o�b�O�@�\�\�����͒ʏ�̃��[�v�����Ȃ�����)
namespace DebugPad
{
	// �p�b�h�̓��͏�Ԏ擾
	void update();

	// ������������
	bool isPress(int button);
	// �g���K�[����
	bool isTrigger(int button);
};
#endif