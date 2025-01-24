#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

#include "util/Vec2.h"

class ScenePause :
	public SceneBase
{
public:
	ScenePause(SceneManager& manager);
	virtual ~ScenePause();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	void DrawCursor();
	void UpdateCursorUp();
	void UpdateCursorDown();

private:
	//遷移先
	enum class e_SceneTrans : int
	{
		kRestart,				//リスタート
		kOption,				//オプション
		kSelect,				//セレクト
		kTitle,					//タイトル
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//画像ハンドル
	int m_pauseHandle;
	float m_selectAnimation;

	Vec2 m_cursorPos;
	Vec2 m_targetCursorUpPos;
	Vec2 m_targetCursorDownPos;

	e_SceneTrans m_sceneTrans;
};

