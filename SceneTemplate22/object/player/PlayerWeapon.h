#pragma once
#include "DxLib.h"

class PlayerWeapon
{
public:
	PlayerWeapon();
	virtual ~PlayerWeapon();

	void Initialize();
	void Finalize();

	/// <summary>
	/// 右手に武器を持たせたいとき用の関数
	/// </summary>
	/// <param name="modelH"></param>
	void OneHandUpdate(int modelH);

	/// <summary>
	/// 両手に武器を持たせたいとき用の関数
	/// </summary>
	/// <param name="modelH"></param>
	void BothHandsUpdate(int modelH);


	void OneHandDraw();

	void BothHandsDraw();


private:

	//モデルハンドル
	int m_rightModelH;	//右手に持つ用のハンドル
	int m_leftModelH;	//左手に持つ用のハンドル

	//武器に関する変数
	VECTOR m_attachFramePos;
};

