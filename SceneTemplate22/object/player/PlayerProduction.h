﻿#pragma once
#include "DxLib.h"
#include "myLib/Collidable.h"
#include <memory>

class AnimController;

class PlayerProduction
{
public:

	PlayerProduction();
	virtual ~PlayerProduction();

	void Initialize(Game::e_PlayerProduction playerScene);

	void Update();
	void Draw();

	void SetTitleFlag(float isTitle) { m_isTitle = isTitle; }
	void SetGameOverFlag(float isGameOver) { m_isGameOver = isGameOver; }
	void SetGameClearFlag(float isGameClear) { m_isGameClear = isGameClear; }

	const VECTOR& GetPos() const { return m_pos; }


private:

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void TitleIdleUpdate();
	void DashUpdate();
	void SitUpdate();
	void SitUpUpdate();
	void SitDownUpdate();
	void GameOverUpdate();
	void RetryUpdate();
	void GameClearUpdate();


private:

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnTitleIdle();
	void OnDash();
	void OnSit();
	void OnSitDown();
	void OnSitUp();
	void OnGameOver();
	void OnRetry();
	void OnGameClear();

private:

	std::shared_ptr<AnimController> m_pAnim;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(PlayerProduction::*)(/*引数書く*/);
	UpdateFunc_t m_updateFunc;

	Game::e_PlayerProduction m_playerProduction;

	VECTOR m_pos;
	VECTOR m_cameraPos;

	bool m_isTitle;
	bool m_isGameOver;
	bool m_isGameClear;

	int m_modelH;
};

