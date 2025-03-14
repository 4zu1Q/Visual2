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
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneTitle::isLoaded() const
{
	// 全てのリソースのロードが完了したかを確認する
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

	DrawString(100, 100, "1ボタンでデバッグメニューに戻る", 0xffffff);
}
