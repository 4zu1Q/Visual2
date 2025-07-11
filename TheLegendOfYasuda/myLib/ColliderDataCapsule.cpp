#include "DxLib.h"
#include "MyLib.h"

using namespace MyLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataCapsule::ColliderDataCapsule(bool isTrigger):
	ColliderData(ColliderData::e_Kind::kCapsule, isTrigger),
	m_radius(0.0f),
	m_posDown(VGet(0,0,0)),
	m_posUp (VGet(0,0,0))
{

	// 処理なし

}