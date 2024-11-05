#include "ColliderDataSphere.h"
#include "myLib/MyLib"

using namespace MyLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataSphere::ColliderDataSphere(bool isTirgger)
	: ColiderData(ColliderData::e_Kind::kCircle3D,isTirgger)
	, m_radius(0.0f)
{
	//処理なし
}
