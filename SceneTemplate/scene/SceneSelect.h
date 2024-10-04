#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneSelect :
	public SceneBase
{
public:
	SceneSelect(SceneManager& manager);
	virtual ~SceneSelect();

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
		kPowerTypeBoss,		//パワータイプのボス
		kSpeedTypeBoss,		//スピードタイプのボス
		kShooterTypeBoss,	//遠距離タイプのボス
		kHookShotTypeBoss,	//フックショットタイプのボス
		kRastBoss,			//ラスボス
	};

	e_SceneTrans m_sceneTrans;

	int m_frameCount;


};

