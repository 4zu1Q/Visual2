#pragma once
#include "ColliderData.h"

namespace MyLib 
{

	class ColliderData;

	/// <summary>
	/// 当たり判定データ:3Dの球
	/// </summary>
	class ColliderDataCapsule : public ColliderData
	{
	public:

		//コンストラクタ
		ColliderDataCapsule(bool isTrigger);

		float m_radius;	//円の半径

		VECTOR m_posDown;	//カプセルの下座標
		VECTOR m_posUp;	//カプセルの上座標

	};

}