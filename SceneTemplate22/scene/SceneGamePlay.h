﻿#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class PlayerBase;
class SkyDome;

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

#ifdef _DEBUG

	std::shared_ptr<PlayerBase> m_pPlayer;
	std::shared_ptr<SkyDome> m_pSkyDome;

	VECTOR m_playerPos;
	VECTOR m_cameraPos;

	int m_analogX;
	int m_analogZ;

	int m_radius;
	float m_angle;

#endif 



	float m_cameraAngle;

};
