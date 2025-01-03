﻿#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneTitle : public SceneBase
{
public:
    SceneTitle(SceneManager& manager);
    virtual ~SceneTitle();

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
		kSelect,	//セレクト
		kOption,	//オプション
		kQuit,		//ゲーム終了
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	bool m_isStart;
	int m_startTime;

	e_SceneTrans m_sceneTrans;

};

