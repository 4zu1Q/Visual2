#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneSaveDataSelect : public SceneBase
{
public:
	SceneSaveDataSelect(SceneManager& manager);
	virtual ~SceneSaveDataSelect();

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
		kSaveData1,	//セーブデータ1
		kSaveData2,	//セーブデータ2
		kSaveData3,	//セーブデータ3
	};

	//
	e_SceneTrans m_sceneTrans;
};

