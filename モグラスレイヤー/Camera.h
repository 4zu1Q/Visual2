#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy;
class Stage;
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Init();

	void TitleCameraUpdate();
	void PlayCameraUpdate(Player& player);		//ゲームプレイ用のカメラ設定
	void TargetCameraUpadate();		//ロックオン機能のカメラ設定

	//カメラのアングルを取得
	const float GetAngle() const { return m_angle; }

	//カメラの座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	//注視点の座標を取得
	const VECTOR& GetTargetPos() const { return m_targetPos; }

private:


	std::shared_ptr<Player> m_pPlayer;

	void DrawGrid();

	VECTOR m_pos;			//カメラ座標
	VECTOR m_targetPos;		//注視点座標

	float m_angle;	//カメラ角度




};

