#include "SceneBase.h"
#include "DxLib.h"
#include "game.h"
#include "Mouse.h"

#include <cassert>

namespace
{
	// �t�F�[�h�ɂ�����t���[���f�t�H���g
	constexpr int kFadeFrameDefault = 30;

	// �t�F�[�h���x
	constexpr int kFadeSpeed = 8;
	// �}�E�X�J�[�\���T�C�Y
	constexpr int kCursorSize = 32;
}

SceneBase::SceneBase():
	m_isInit(false),
	m_fadeFrame(kFadeFrameDefault),
	m_fadeTotalFrame(kFadeFrameDefault),
	m_fadeDir(0),
	m_isDispLoading(false),
	m_loadFrame(0),
	m_isDispCursor(true)
{
}

float SceneBase::getFadeProgress() const
{
	return static_cast<float>(m_fadeFrame) / static_cast<float>(m_fadeTotalFrame);
}

void SceneBase::setNextScene(SceneBase* pNext)
{
	assert(!isFading());
	assert(!m_isEnd);

	m_isEnd = true;
	m_pNext = pNext;
	startFadeOut();
}

void SceneBase::initAll()
{
	// �t�F�[�h�A�E�g��Ԃ���J�n
	m_fadeFrame = m_fadeTotalFrame;
	m_fadeDir = -1;
	// �p����V�[���̏���������
	init();
}

void SceneBase::endAll()
{
	// �p����V�[���̏I������
	end();
}

void SceneBase::updateAll()
{
	if (!isLoaded())
	{
		// ���\�[�X�̃��[�h�ɂ������Ă���t���[�������J�E���g
		m_loadFrame++;
	//	printfDx("LoadFrame : %d\n", m_loadFrame);
		return;	// ���[�h���͍X�V���������s��Ȃ�
	}
	if (!m_isInit)
	{
		initAll();
		m_isInit = true;
	}

	updateFade();
	// �p����V�[���̍X�V����
	update();
}

void SceneBase::drawAll()
{
	// �p����V�[���̕`�揈��
	draw();
	drawCursor();
	drawFade();
	drawLoading();
	drawFront();
#ifdef _DEBUG
	drawDebug();
#endif
}

bool SceneBase::isSceneEnd()
{
	// ���������I���ƌ����Ă��Ȃ�
	if (!m_isEnd)	return false;
	// �܂��t�F�[�h�A�E�g�I����ĂȂ�
	if (m_fadeFrame < m_fadeTotalFrame)	return false;
	
	return true;
}

void SceneBase::updateFade()
{
	// ���\�[�X�̃��[�h���������Ă��Ȃ�
	if (m_fadeDir < 0 && (!isLoaded()))
	{
		return;	// ���[�h�����܂ł̓t�F�[�h�C���������s��Ȃ�
	}

	m_fadeFrame += m_fadeDir;
	if (m_fadeFrame >= m_fadeTotalFrame)
	{
		m_fadeFrame = m_fadeTotalFrame;
		m_fadeDir = 0;
	}
	if (m_fadeFrame <= 0)
	{
		m_fadeFrame = 0;
		m_fadeDir = 0;
	}
}

void SceneBase::drawFade() const
{
#if true
	float rate = getFadeProgress();
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#else
	// ���S�ɕ\������Ă����ԂȂ̂Ńt�F�[�h�͕\�����Ȃ�
	if (m_fadeFrame <= 0)	return;

	float rate = getFadeProgress();
	int startX = 0;
	int endX = Game::kScreenWidth;

	if (m_fadeDir < 0)
	{
		// �t�F�[�h�C����
		endX = static_cast<int>(Game::kScreenWidth * rate);
		for (int y = 0; y < Game::kScreenHeight; y++)
		{
			int elem = GetRand(16) + 4;
			DrawLine(startX, y, endX - GetRand(128), y, GetColor(elem,elem,elem));
		}
	}
	else
	{
		// �t�F�[�h�A�E�g��
		startX = static_cast<int>(Game::kScreenWidth * (1.0f - rate));
		for (int y = 0; y < Game::kScreenHeight; y++)
		{
			int elem = GetRand(16) + 4;
			DrawLine(startX - GetRand(128), y, endX, y, GetColor(elem, elem, elem));
		}
	}
#endif
}

void SceneBase::drawLoading() const
{
	if (m_isDispLoading && !isLoaded())
	{
#if false
		for (int i = 0; i < 8; i++)
		{
			int x = GetRand(Game::kScreenWidth);
			int y = GetRand(Game::kScreenHeight);

			DrawString(x,y,"���[�h��",0xffffff);
		}
#endif
		float progress = getLoadProgress();
		if (progress >= 0.0f)
		{
			// ���[�h�i���̃f�o�b�O�\��
			constexpr int kBoxLeft = 120;
			constexpr int kBoxRight = Game::kScreenWidth - 120;
			constexpr int kBoxTop = Game::kScreenHeight - 160;
			constexpr int kBoxBottom = Game::kScreenHeight - 80;
			constexpr int kBoxLen = kBoxRight - kBoxLeft;
			DrawBox(kBoxLeft, kBoxTop, static_cast<int>(kBoxLeft + kBoxLen * progress), kBoxBottom, 0xffffff, true);
			DrawBox(kBoxLeft, kBoxTop, kBoxRight, kBoxBottom, 0xffffff, false);

			DrawFormatString(kBoxLeft + 8, kBoxBottom - 48, 0x000000, "���[�h���c");
			DrawFormatString(kBoxLeft + 8, kBoxBottom - 32, 0x000000, "%3.1f %%", progress * 100);
		}
	}
}

void SceneBase::drawCursor() const
{
	if (!m_isDispCursor)	return;

	int count = 0;
	int size = kCursorSize / 2;
	for (int i = 0; i < Mouse::getLogNum(); i++)
	{
		float rate = static_cast<float>(i) / static_cast<float>(Mouse::getLogNum() - 1);
		if (rate < 0.0f)	rate = 0.0f;
		if (rate > 1.0f)	rate = 1.0f;
		rate = 1.0f - rate;
		int alpha = static_cast<int>(255 * rate);

		Vec2 pos = Mouse::getPos(i);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawLine(static_cast<int>(pos.x), static_cast<int>(pos.y) - size,
			static_cast<int>(pos.x), static_cast<int>(pos.y) + size,
			0xffffff, 3);
		DrawLine(static_cast<int>(pos.x) - size, static_cast<int>(pos.y),
			static_cast<int>(pos.x) + size, static_cast<int>(pos.y),
			0xffffff, 3);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		size -= 1;
		count++;
	}
}

bool SceneBase::isFadingIn() const
{
	if (m_fadeDir < 0)	return true;
	return false;
}

bool SceneBase::isFadingOut() const
{
	if (m_fadeDir > 0)	return true;
	return false;
}

void SceneBase::startFadeOut()
{
	m_fadeDir = 1;
}