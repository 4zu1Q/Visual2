#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneGameClear :
	public SceneBase
{
public:
	SceneGameClear(SceneManager& manager);
	virtual ~SceneGameClear();

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
		kTitle,		//オプション
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	e_SceneTrans m_sceneTrans;



};

