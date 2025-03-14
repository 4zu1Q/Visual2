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
	// シーン設定
	setDispLoading(false);	// ロード中表示を行うか
//	setFadeFrame(8);		// フェードフレームを設定	設定せずにデフォルトフレームにしても良い
}

void SceneDebug::startLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	std::string filename;
	m_loadFileNum = 0;
	
	for (auto& handle : m_handle)
	{
		handle = LoadGraph("data/dummy000.bmp");
		assert(handle != -1);
		m_loadFileNum++;
	}

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneDebug::isLoaded() const
{
	// 全てのリソースのロードが完了したかを確認する
//	for (auto& handle : m_handle)
//	{
//		if (CheckHandleASyncLoad(handle))	return false;
//	}
	// １個１個チェックしなくてもこっちでよかった
	if (GetASyncLoadNum() > 0)	return false;
	return true;
}

void SceneDebug::init()
{
	// init()はロード完了後に呼ばれる
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
	// フェードインアウト中はゲーム処理行わない
//	if (isFading())
	if(getFadeProgress() > 0.0f)
	{
		// フェード中も背景動かしたり待機アニメする場合はここで

		return;
	}

	m_pDebugMenu->update();
	m_frameCount++;
}

void SceneDebug::draw()
{
	if (!isInitialized())	return;

	// フェードインアウト中も描画は行う
//	if (isFading()) {}

	// リソースのロードが終わるまでは描画しないのがよさそう
	// (どちらにしろフェード仕切っているので何も見えないはず)
	if (!isLoaded())	return;

	DrawString(64, 0, "デバッグメニュー", 0xffffff);
	m_pDebugMenu->draw();

	DrawString(640,8,"SceneDebug", 0xffffff);
	DrawFormatString(640, 8+16,0xffffff,"FRAME:%d", m_frameCount);

}

float SceneDebug::getLoadProgress() const
{
	// ロード完了しているファイルの数を調べる
	int loadEndNum = 0;
	for (auto& handle : m_handle)
	{
		if (!CheckHandleASyncLoad(handle))	loadEndNum++;
	}
	return static_cast<float>(loadEndNum) / static_cast<float>(m_loadFileNum);
}
