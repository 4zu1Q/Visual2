#pragma once
#include "scene/SceneBase.h"

class SceneTutorial : public SceneBase
{
public:

	SceneTutorial(SceneManager& manager);
	virtual ~SceneTutorial();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

};

