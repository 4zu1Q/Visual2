#include "SceneManager.h"
#include "SceneTitle.h"
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
	constexpr int kUpdateBarColor = 0x0000ff;
	constexpr int kDrawBarColor = 0xff0000;

	//処理バーの座標
	constexpr int kBarPosX = 0;
	constexpr int kBarPosY = Game::kScreenHeight - 16;
}

SceneManager::SceneManager():
	m_pScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	//最初のシーンのメモリを確保する
	m_pScene = std::make_shared<SceneTitle>();
	m_pScene->Init();
}

void SceneManager::Update()
{
	//更新のローディング時間を取得
	LONGLONG start = GetNowHiPerformanceCount();

	//SceneBase* pNext = m_pScene->Update();
	std::shared_ptr<SceneBase>pNext = m_pScene->Update();
	if (pNext != m_pScene)
	{
		//現在処理中のシーンの終了処理
		m_pScene->End();

		//Updateが返した新しいシーンに遷移する
		m_pScene = pNext;
		m_pScene->Init();

	}
}

void SceneManager::Draw()
{
	LONGLONG start = GetNowHiPerformanceCount();


	m_pScene->Draw();

#ifdef _DEBUG
	m_drawTime = static_cast<float>(GetNowHiPerformanceCount() - start);

	//処理バーの表示　
	//説明
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

void SceneManager::End()
{
	m_pScene->End();
}
