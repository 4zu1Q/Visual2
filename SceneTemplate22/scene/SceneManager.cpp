#include "scene/SceneManager.h"
#include "scene/SceneTitle.h"
#include "util/Game.h"

namespace
{

	//説明用処理バーの文字の座標
	constexpr int kUpdateBarStringPosX = 0;
	constexpr int kUpdateBarStringPosY = Game::kScreenHeight - 48;
	constexpr int kDrawBarPosStringPosX = 0;
	constexpr int kDrawBarPosStringPosY = Game::kScreenHeight - 32;

	//説明用処理バーの座標
	constexpr int kExplainUpdateBarPosX1 = 34;
	constexpr int kExplainUpdateBarPosY1 = Game::kScreenHeight - 46;
	constexpr int kExplainUpdateBarPosX2 = 62;
	constexpr int kExplainUpdateBarPosY2 = Game::kScreenHeight - 34;
	constexpr int kExplainDrawBarPosX1 = 34;
	constexpr int kExplainDrawBarPosY1 = Game::kScreenHeight - 30;
	constexpr int kExplainDrawBarPosX2 = 62;
	constexpr int kExplainDrawBarPosY2 = Game::kScreenHeight - 18;


	//処理バーの色
	constexpr int kUpdateBarColor = 0xff00ff;
	constexpr int kDrawBarColor = 0x00ffff;

	//処理バーの座標
	constexpr int kBarPosX = 0;
	constexpr int kBarPosY = Game::kScreenHeight - 16;
}

SceneManager::SceneManager()
#ifdef _DEBUG
  :	m_updateTime(0.0f),
	m_drawTime(0.0f)
#endif
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
#ifdef _DEBUG
	//更新前のローディング時間取得
	LONGLONG start = GetNowHiPerformanceCount();
#endif

	//末尾のみ実行
	m_scenes.back()->Update();

#ifdef _DEBUG
	//更新後のローディング時間から更新前のローディング時間を引いた値を取得
	m_updateTime = static_cast<float>(GetNowHiPerformanceCount() - start);
#endif
}

void SceneManager::Draw()
{

#ifdef _DEBUG
	//更新前のローディング時間取得
	LONGLONG start = GetNowHiPerformanceCount();
#endif

	//先頭から順に描画
	//最後に積んだものが最後に描画される
	for (auto& scenes : m_scenes)
	{
		scenes->Draw();
	}

#ifdef _DEBUG
	m_drawTime = static_cast<float>(GetNowHiPerformanceCount() - start);
	DrawDebug();
#endif
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> nextScene)
{
	if (m_scenes.empty())	//リストが空だったら入れ替えずに
	{
		m_scenes.push_back(nextScene);	//末尾に追加
	}
	else
	{
		m_scenes.back() = nextScene;	//既に一つ以上であれば末尾を
	}
}

void SceneManager::ChangeAndClearScene(std::shared_ptr<SceneBase> nextScene)
{
	m_scenes.clear();

	m_scenes.push_back(nextScene);
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_scenes.push_back(scene);
}

void SceneManager::PopScene(bool isToTitle)
{
	m_scenes.pop_back();
}

void SceneManager::DrawDebug()
{
	//処理バーの表示　
	//説明
#ifdef _DEBUG

	DrawString(kUpdateBarStringPosX, kUpdateBarStringPosY, "処理", 0xffffff, 0x000000);
	DrawBox(kExplainUpdateBarPosX1, kExplainUpdateBarPosY1, kExplainUpdateBarPosX2, kExplainUpdateBarPosY2, kUpdateBarColor, true);
	DrawString(kDrawBarPosStringPosX, kDrawBarPosStringPosY, "描画", 0xffffff, 0x000000);
	DrawBox(kExplainDrawBarPosX1, kExplainDrawBarPosY1, kExplainDrawBarPosX2, kExplainDrawBarPosY2, kDrawBarColor, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	float width = static_cast<float>(Game::kScreenWidth * rate);
	DrawBox(kBarPosX, kBarPosY, static_cast<int>(width), Game::kScreenHeight, kDrawBarColor, true);

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<float>(Game::kScreenWidth * rate);
	DrawBox(kBarPosX, kBarPosY, static_cast<int>(width), Game::kScreenHeight, kUpdateBarColor, true);

#endif

}
