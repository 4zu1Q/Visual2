#include "Pad.h"
#include "DxLib.h"

namespace
{
	// �O�̃t���[���̃p�b�h�����������
	int lastPad = 0;
	// ���̃t���[���̃p�b�h�����������
	int nowPad = 0;
}

namespace Pad
{
	void Update()
	{
		// �O�̃t���[���Ɏ擾�����p�b�h������Â����ɂ���
		lastPad = nowPad;
		// ���݂̃p�b�h�̏����擾����
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool IsPress(int key)
	{
		return (nowPad & key);
	}

	bool IsTrigger(int key)
	{
		if (nowPad)
		{
			int i = 0;
		}

		bool isLast = (lastPad & key);	// �O�̃t���[��
		bool isNow = (nowPad & key);	// ���̃t���[��
		//return !isLast && isNow;		// ����ł������Ǘ������ɂ����̂ŕ�����₷��



		if (isNow &&		// ���̃t���[���ɉ�����Ă���
			!isLast)	// �O��̃t���[���ɉ�����Ă��Ȃ�
		{
			return true;
		}
		return false;
	}
	bool IsRelase(int key)
	{
		bool isNow = (nowPad & key);	// ���̃t���[��
		bool isLast = (lastPad & key);	// �O�̃t���[��
		//return isLast && !isNow;

		if (!isNow &&	// ���̃t���[���ɂ͉�����Ă��Ȃ���
			isLast)		// �O��̃t���[���͉�����Ă���
		{
			return true;
		}

		return false;
	}
}