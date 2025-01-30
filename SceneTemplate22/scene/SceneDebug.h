#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneDebug :
	public SceneBase
{
public:
	SceneDebug(SceneManager& manager);
	virtual ~SceneDebug();

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
		kDebug,
		kTitle,
		kSelect,
		kGamePlayPower,
		kGamePlaySpeed,
		kGamePlayShot,
		kGamePlayRast,
		kGameClear,
		kGameOver,
		kOption,
		kPause
	};

	//押し込んだ時間
	int m_pushCount;
	//押したかどうか
	bool m_isPush;

	e_SceneTrans m_sceneTrans;

};

