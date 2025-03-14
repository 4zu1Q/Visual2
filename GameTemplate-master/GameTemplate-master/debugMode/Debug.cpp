#ifdef _DEBUG
#include "Debug.h"
#include "game.h"

#include "DebugPad.h"	// Scene��Update�Ńp�b�h�������s���Ă���̂�"Pad.h"�͎g�p�s��


#include <string>

Debug* Debug::m_instance = nullptr;

namespace
{
	const char* const kTitle = "�f�o�b�O���j���[";

	constexpr int kScreenShotInterval = 60;
}

void Debug::update()
{
	// �p�b�h��Ԃ̍X�V
	DebugPad::update();

	if (m_setting.isOpenTrg)
	{
		if (DebugPad::isTrigger(PAD_INPUT_5))
		{
			m_isActive = !m_isActive;
		}
	}
	else
	{
		m_isActive = DebugPad::isPress(PAD_INPUT_5);
	}
	

	// ��A�N�e�B�u���ɂ����s���鏈��


	// ��A�N�e�B�u���ɂ����s���鏈�� �����܂�

	if (!m_isActive)	return;

	// �A�N�e�B�u���̂ݗL���ȏ���
	m_isSaveScreenShot = false;
	if (DebugPad::isTrigger(PAD_INPUT_6))
	{
		// �����ł̓X�N�V������炸�A�Q�[����ʕ`���ɎB�e����
		m_isSaveScreenShot = true;
	}
}

void Debug::draw()
{
	drawBg();
	drawFps();
	drawProcessBar();

	if (!m_isActive)	return;

	DrawString(32, 16 + 16, "R:�X�N���[���V���b�g�B�e", 0xffffff);
	DrawString(32, 32 + 16, "��:�R�}����", 0xffffff);

	DrawString(32, 48 + 16, "FPS�\��:�@  ���  [�f�o�b�O���j���[���J���Ă����]  �\�����Ȃ�", 0xffffff);
	DrawString(32, 64 + 16, "�����o�[:�@[���]  �f�o�b�O���j���[���J���Ă����   �\�����Ȃ�", 0xffffff);
	DrawString(32, 80 + 16, "�f�o�b�O���j���[�J��:�@[L�{�^���������Ă���ԊJ��]  L�{�^���ŊJ��", 0xffffff);
//	DrawString(32, 96 + 16, "�w�i�̔Z��:�@[L�{�^���������Ă���ԊJ��]  L�{�^���ŊJ��", 0xffffff);
	DrawFormatString(32, 96 + 16, 0xffffff, "�w�i�̔Z��:�@%3d%%", m_setting.bgAlpha*10);

	DrawString(32, 112 + 16, "�f�o�b�O�ݒ�̕ۑ�", 0xffffff);
}

void Debug::postDraw()
{
	if (m_isSaveScreenShot)
	{
		screenShot();
	}
}

bool Debug::isGameUpdate() const
{
	if (!m_isActive)	return true;
	if( DebugPad::isTrigger(PAD_INPUT_RIGHT) )	return true;

	return false;
}

void Debug::screenShot(const char* fileName)
{
	std::string name = "screenShot/";
	if (fileName)
	{
		name += fileName;
	}
	else
	{
		// �t�@�C������������
		DATEDATA buf;
		GetDateTime(&buf);

		name += std::to_string(buf.Year);
		if (buf.Mon < 10)	name += "0";
		name += std::to_string(buf.Mon);
		if (buf.Day < 10)	name += "0";
		name += std::to_string(buf.Day);
		if (buf.Hour < 10)	name += "0";
		name += std::to_string(buf.Hour);
		if (buf.Min < 10)	name += "0";
		name += std::to_string(buf.Min);
		if (buf.Sec < 10)	name += "0";
		name += std::to_string(buf.Sec);
		name += ".bmp";
	}

	SaveDrawScreen(0, 0, Game::kScreenWidth, Game::kScreenHeight, name.c_str());
	printfDx("�X�N���[���V���b�g��ۑ����܂���! %s\n", name.c_str());
}

void Debug::onStartUpdate()
{
	m_updateTime = GetNowHiPerformanceCount();
}

void Debug::onEndUpdate()
{
	m_updateTime = GetNowHiPerformanceCount() - m_updateTime;
}

void Debug::onStartDraw()
{
	m_drawTime = GetNowHiPerformanceCount();
}

void Debug::onEndDraw()
{
	m_drawTime = GetNowHiPerformanceCount() - m_drawTime;
}

void Debug::drawBg() const
{
	if (!m_isActive)	return;

	float rate = static_cast<float>(m_setting.bgAlpha) / 10;
	int alpha = static_cast<int>(rate * 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int width = GetDrawStringWidth(kTitle,static_cast<int>(strlen(kTitle)));
	DrawString(Game::kScreenWidth / 2 - width /2, 8, kTitle, 0xffffff);
}

void Debug::drawFps() const
{
	if (m_setting.dispFps == DispType::None)	return;
	if (m_setting.dispFps == DispType::DebugActive && !m_isActive)	return;

	int width = GetDrawFormatStringWidth("FPS:%.4f", GetFPS());
	DrawFormatString(Game::kScreenWidth - width - 32, 32, 0xffffff, "FPS:%.4f", GetFPS());
}

void Debug::drawProcessBar() const
{
	if (m_setting.dispProcessBar == DispType::None)	return;
	if (m_setting.dispProcessBar == DispType::DebugActive && !m_isActive)	return;

	float rate = static_cast<float>(m_updateTime + m_drawTime) / Game::kFrameMicroSec;
	int width = static_cast<int>(Game::kScreenWidth * rate);
	// Update+Draw
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0xff0000, true);

	rate = static_cast<float>(m_updateTime) / Game::kFrameMicroSec;
	// Update
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0x0000ff, true);
}


#endif // _DEBUG
