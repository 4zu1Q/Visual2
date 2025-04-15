#pragma once
#include "scene/SceneBase.h"
#include <vector>
#include <memory>

#include "util/Vec2.h"

class SceneWords :
	public SceneBase
{
public:
	SceneWords(SceneManager& manager);
	virtual ~SceneWords();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理




};

