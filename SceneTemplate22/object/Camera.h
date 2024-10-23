#pragma once
#include "DxLib.h"
#include <memory>


class Player;
class Camera
{
public:

	Camera();
	virtual ~Camera();

	void Initialize();
	void Finalize();

	void Update(Player& player);
	void Draw();

	//デバッグ用のアップデート
	void DebugUpdate();

	//カメラのアングルを取得
	const float GetAngle() const { return m_angle; }

private:

	/// <summary>
	/// 奥行などが分かりやすくするための描画
	/// </summary>
	void DrawGrid();

private:

	//スマートポインタ
	std::shared_ptr<Player> m_pPlayer;

	VECTOR m_pos;			//カメラ座標
	VECTOR m_targetPos;		//注視点座標
	VECTOR m_cameraangle;			//カメラのアングル

	float m_angle;	//カメラ角度

};

