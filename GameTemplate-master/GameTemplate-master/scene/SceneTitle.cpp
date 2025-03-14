#include "SceneTitle.h"
#include "DxLib.h"

#include "SceneDebug.h"
#include "Pad.h"

SceneTitle::SceneTitle():
	m_frameCount(0)
{
}

void SceneTitle::startLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

bool SceneTitle::isLoaded() const
{
	// �S�Ẵ��\�[�X�̃��[�h���������������m�F����
//	if (CheckHandleASyncLoad(handle))	return false;

	return true;
}

void SceneTitle::init()
{
}

void SceneTitle::end()
{
}

void SceneTitle::update()
{
	if (getFadeProgress() > 0.0f)
	{
		return;
	}

	m_frameCount++;
	if (Pad::isTrigger(PAD_INPUT_1))
	{
		setNextScene(new SceneDebug);
	}
}

void SceneTitle::draw()
{
	if (!isLoaded())	return;

	DrawString(0,0,"SceneTitle", 0xffffff);
	DrawFormatString(0, 16, 0xffffff, "Frame:%d", m_frameCount);

	DrawString(100, 100, "1�{�^���Ńf�o�b�O���j���[�ɖ߂�", 0xffffff);
}
