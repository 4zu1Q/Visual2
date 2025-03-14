#include "SceneBase.h"
#include "DxLib.h"
#include "game.h"
#include "Mouse.h"

#include <cassert>

namespace
{
	// フェードにかかるフレームデフォルト
	constexpr int kFadeFrameDefault = 30;

	// フェード速度
	constexpr int kFadeSpeed = 8;
	// マウスカーソルサイズ
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
	// フェードアウト状態から開始
	m_fadeFrame = m_fadeTotalFrame;
	m_fadeDir = -1;
	// 継承先シーンの初期化処理
	init();
}

void SceneBase::endAll()
{
	// 継承先シーンの終了処理
	end();
}

void SceneBase::updateAll()
{
	if (!isLoaded())
	{
		// リソースのロードにかかっているフレーム数をカウント
		m_loadFrame++;
	//	printfDx("LoadFrame : %d\n", m_loadFrame);
		return;	// ロード中は更新処理何も行わない
	}
	if (!m_isInit)
	{
		initAll();
		m_isInit = true;
	}

	updateFade();
	// 継承先シーンの更新処理
	update();
}

void SceneBase::drawAll()
{
	// 継承先シーンの描画処理
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
	// そもそも終わると言っていない
	if (!m_isEnd)	return false;
	// まだフェードアウト終わってない
	if (m_fadeFrame < m_fadeTotalFrame)	return false;
	
	return true;
}

void SceneBase::updateFade()
{
	// リソースのロードが完了していない
	if (m_fadeDir < 0 && (!isLoaded()))
	{
		return;	// ロード完了まではフェードイン処理を行わない
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
	// 完全に表示されている状態なのでフェードは表示しない
	if (m_fadeFrame <= 0)	return;

	float rate = getFadeProgress();
	int startX = 0;
	int endX = Game::kScreenWidth;

	if (m_fadeDir < 0)
	{
		// フェードイン中
		endX = static_cast<int>(Game::kScreenWidth * rate);
		for (int y = 0; y < Game::kScreenHeight; y++)
		{
			int elem = GetRand(16) + 4;
			DrawLine(startX, y, endX - GetRand(128), y, GetColor(elem,elem,elem));
		}
	}
	else
	{
		// フェードアウト中
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

			DrawString(x,y,"ロード中",0xffffff);
		}
#endif
		float progress = getLoadProgress();
		if (progress >= 0.0f)
		{
			// ロード進捗のデバッグ表示
			constexpr int kBoxLeft = 120;
			constexpr int kBoxRight = Game::kScreenWidth - 120;
			constexpr int kBoxTop = Game::kScreenHeight - 160;
			constexpr int kBoxBottom = Game::kScreenHeight - 80;
			constexpr int kBoxLen = kBoxRight - kBoxLeft;
			DrawBox(kBoxLeft, kBoxTop, static_cast<int>(kBoxLeft + kBoxLen * progress), kBoxBottom, 0xffffff, true);
			DrawBox(kBoxLeft, kBoxTop, kBoxRight, kBoxBottom, 0xffffff, false);

			DrawFormatString(kBoxLeft + 8, kBoxBottom - 48, 0x000000, "ロード中…");
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