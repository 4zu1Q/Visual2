#include "DxLib.h"
#include "MyLib.h"

using namespace MyLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataSphere::ColliderDataSphere(bool isTrigger):
	ColliderData(ColliderData::e_Kind::kSphere, isTrigger),
	m_center(VGet(0,0,0)),
	m_radius(0.0f)
{
	// 処理なし
}