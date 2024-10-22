#pragma once
#include "DxLib.h"
#include <memory>


class PlayerBase;
class Camera
{
public:

	Camera();
	virtual ~Camera();

	void Initialize();
	void Finalize();

	void Update(PlayerBase& player);
	void Draw();

private:

	void DrawGrid();


private:

	std::shared_ptr<PlayerBase> m_pPlayer;


	VECTOR m_pos;			//カメラ座標
	VECTOR m_targetPos;		//注視点座標
	VECTOR m_cameraangle;			//カメラのアングル


	float m_angle;	//カメラ角度

};

