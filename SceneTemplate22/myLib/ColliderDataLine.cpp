#include "DxLib.h"
#include "MyLib.h"

using namespace MyLib;


ColliderDataLine::ColliderDataLine(bool isTrigger):
	ColliderData(ColliderData::e_Kind::kCapsule, isTrigger),
	m_startPos(VGet(0, 0, 0)),
	m_endPos(VGet(0, 0, 0))
{

}
