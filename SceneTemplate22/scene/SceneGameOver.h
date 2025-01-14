#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver(SceneManager& manager);
	virtual ~SceneGameOver();

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
		kGamePlay,	//ゲームプレイ
		kSelect,		//セレクト
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	e_SceneTrans m_sceneTrans;

};


