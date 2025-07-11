#pragma once
#include "ColliderData.h"

namespace MyLib 
{

	class ColliderData;

	/// <summary>
	/// 当たり判定データ:3Dの球
	/// </summary>
	class ColliderDataSphere : public ColliderData
	{
	public:

		//コンストラクタ
		ColliderDataSphere(bool isTrigger);

		float m_radius;	//円の半径
		VECTOR m_center;//円の中心

	};

}


