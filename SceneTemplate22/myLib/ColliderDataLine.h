#pragma once
#include "ColliderData.h"

namespace MyLib
{

	class ColliderData;

	/// <summary>
	/// 当たり判定データ:3Dの球
	/// </summary>
	class ColliderDataLine : public ColliderData
	{
	public:

		//コンストラクタ
		ColliderDataLine(bool isTrigger);

		VECTOR m_startPos;	//ラインの始めの座標
		VECTOR m_endPos;		//ラインの終わりの座標

	};

}