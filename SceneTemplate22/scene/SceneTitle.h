﻿#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class PlayerProduction;
class CameraProduction;
class SkyDome;
class TitleField;

class SceneTitle : public SceneBase
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
    
	const bool& GetIsPlayer() const { return m_isPlayer; }

private:

	std::shared_ptr<PlayerProduction> m_pPlayerProduction;
	std::shared_ptr<CameraProduction> m_pCameraProduction;
	std::shared_ptr<SkyDome> m_pSkyDome;
	std::shared_ptr<TitleField> m_pTitleField;


	//遷移先
	enum class e_SceneTrans : int
	{
		kNewGame,	//ニューゲーム
		kLoadGame,	//ロードゲーム
		kOption,	//オプション
		kQuit,		//ゲーム終了
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	bool m_isPlayer;	//プレイヤーのタイトルバージョンのアニメーションを行う

	bool m_isStart;
	int m_startTime;
	float m_selectAnimation;

	e_SceneTrans m_sceneTrans;

};

