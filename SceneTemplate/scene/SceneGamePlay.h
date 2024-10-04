#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneGamePlay :
	public SceneBase
{
public:
	SceneGamePlay(SceneManager& manager);
	virtual ~SceneGamePlay();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理


	void DrawGrid();

private:

	float m_cameraAngle;

};

