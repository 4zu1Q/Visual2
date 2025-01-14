﻿#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneOption :
	public SceneBase
{
public:
	SceneOption(SceneManager& manager);
	virtual ~SceneOption();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	enum e_Item : int
	{
		kBgm,
		kSe,
		kSensitivity,
		kFullScreen
		//バイブレーションも欲しいかも？
	};

	e_Item m_nowItem;

	int m_bgmScale;
	int m_seScale;
	int m_sensitivityScale;
	bool m_isFullScreen;

	int m_pushCount;
	float m_selectAnimation;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//更新メンバ関数ポインタ
	using UpdateFunc_t = void(SceneOption::*)(/*引数書く*/);
	UpdateFunc_t m_updateFunc;

	//BGM
	void BgmUpdate();
	//SE
	void SeUpdate();
	//Sensitivity
	void SensitivityUpdate();
	//FullScreen
	void FullScreenUpdate();


};

