#pragma once

namespace MyLib {

	class ColliderData;

	/// <summary>
	/// 当たり判定データ:3Dの球
	/// </summary>
	class ColliderDataSphere
	{
	public:
		ColliderDataSphere(bool isTirgger);

		float radius;	//円の半径

	};

}


