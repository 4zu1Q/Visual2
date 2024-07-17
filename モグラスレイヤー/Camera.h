#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy;
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Init();

	void TitleCameraUpdate();
	void PlayCameraUpdate(const Player& player);		//ゲームプレイ用のカメラ設定
	void TargetCameraUpadate();		//ロックオン機能のカメラ設定

	const float GetAngle() const { return m_cameraAngle; }

	const VECTOR& GetPos() const { return m_pos; }
	const VECTOR& GetTargetPos() const { return m_targetPos; }

private:

	std::shared_ptr<Player> m_pPlayer;

	void DrawGrid();

	VECTOR m_pos;			//カメラ座標
	VECTOR m_targetPos;		//注視点座標

	float m_cameraAngle;	//カメラ角度




};

