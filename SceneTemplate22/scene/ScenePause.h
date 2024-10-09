#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

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
	//遷移先
	enum class e_SceneTrans : int
	{
		kRestart,	//リスタート
		kOption,	//オプション
		kSelect,	//セレクト
	};

	e_SceneTrans m_sceneTrans;
};

