#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneTitle :
    public SceneBase
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

	e_SceneTrans m_sceneTrans;


	

};

