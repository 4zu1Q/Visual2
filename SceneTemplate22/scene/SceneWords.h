#pragma once
#include "scene/SceneBase.h"
#include <vector>
#include <memory>

#include "util/Vec2.h"
#include "util/Game.h"

class SceneWords :
	public SceneBase
{
public:
	SceneWords(SceneManager& manager,Game::e_TutorialProgress progress);
	virtual ~SceneWords();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	void Next();
	void CloseWords(int wordsNum);

private:
	//遷移先
	//enum class e_SceneTrans : int
	//{
	//	kRestart,				//リスタート
	//	kOption,				//オプション
	//	kSelect,				//セレクト
	//	kTitle,					//タイトル
	//};

	//enum変数
	//e_SceneTrans m_sceneTrans;

	Game::e_TutorialProgress m_tutorialProgress;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	int m_fontH;

	//画像ハンドル
	int m_pauseHandle;
	float m_selectAnimation;

	int m_wordsNum;
	int m_skipTime;

	//カーソル座標
	Vec2 m_cursorPos;
	Vec2 m_targetCursorUpPos;
	Vec2 m_targetCursorDownPos;


};

