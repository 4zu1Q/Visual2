#include "DxLib.h"
#include "game.h"
#include "SceneDebug.h"

#include "Pad.h"
#include "Mouse.h"

#include "UiDebugMenu.h"

#include <string>
#include <cassert>

namespace
{
}

SceneDebug::SceneDebug():
	m_loadFileNum(0),
	m_frameCount(0)
{
	// �V�[���ݒ�
	setDispLoading(false);	// ���[�h���\�����s����
//	setFadeFrame(8);		// �t�F�[�h�t���[����ݒ�	�ݒ肹���Ƀf�t�H���g�t���[���ɂ��Ă��ǂ�
}

void SceneDebug::startLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);

	std::string filename;
	m_loadFileNum = 0;
	
	for (auto& handle : m_handle)
	{
		handle = LoadGraph("data/dummy000.bmp");
		assert(handle != -1);
		m_loadFileNum++;
	}

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

bool SceneDebug::isLoaded() const
{
	// �S�Ẵ��\�[�X�̃��[�h���������������m�F����
//	for (auto& handle : m_handle)
//	{
//		if (CheckHandleASyncLoad(handle))	return false;
//	}
	// �P�P�`�F�b�N���Ȃ��Ă��������ł悩����
	if (GetASyncLoadNum() > 0)	return false;
	return true;
}

void SceneDebug::init()
{
	// init()�̓��[�h������ɌĂ΂��
	m_pDebugMenu = std::make_shared<UiDebugMenu>();
	m_pDebugMenu->createItems(this);
	m_pDebugMenu->init();
}

void SceneDebug::end()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void SceneDebug::update()
{
	// �t�F�[�h�C���A�E�g���̓Q�[�������s��Ȃ�
//	if (isFading())
	if(getFadeProgress() > 0.0f)
	{
		// �t�F�[�h�����w�i����������ҋ@�A�j������ꍇ�͂�����

		return;
	}

	m_pDebugMenu->update();
	m_frameCount++;
}

void SceneDebug::draw()
{
	if (!isInitialized())	return;

	// �t�F�[�h�C���A�E�g�����`��͍s��
//	if (isFading()) {}

	// ���\�[�X�̃��[�h���I���܂ł͕`�悵�Ȃ��̂��悳����
	// (�ǂ���ɂ���t�F�[�h�d�؂��Ă���̂ŉ��������Ȃ��͂�)
	if (!isLoaded())	return;

	DrawString(64, 0, "�f�o�b�O���j���[", 0xffffff);
	m_pDebugMenu->draw();

	DrawString(640,8,"SceneDebug", 0xffffff);
	DrawFormatString(640, 8+16,0xffffff,"FRAME:%d", m_frameCount);

}

float SceneDebug::getLoadProgress() const
{
	// ���[�h�������Ă���t�@�C���̐��𒲂ׂ�
	int loadEndNum = 0;
	for (auto& handle : m_handle)
	{
		if (!CheckHandleASyncLoad(handle))	loadEndNum++;
	}
	return static_cast<float>(loadEndNum) / static_cast<float>(m_loadFileNum);
}
