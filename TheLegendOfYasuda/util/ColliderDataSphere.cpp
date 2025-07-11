#include "DxLib.h"
#include "myLib/MyLib.h"

using namespace MyLib;

ColliderDataSphere::ColliderDataSphere(bool isTrigger):
	ColliderData(ColliderData::e_Kind::Sphere, isTrigger),
	radius(0.0f)
{

	//　処理なし

}
