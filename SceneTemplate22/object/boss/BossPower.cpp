#include "BossPower.h"

namespace
{

}

BossPower::BossPower():
	m_pos(VGet(0,0,0))
{

}

BossPower::~BossPower()
{
}

void BossPower::Initialize()
{
	m_pos = VGet(0, 0, 100);

}

void BossPower::Finalize()
{

}

void BossPower::Update()
{

	

}

void BossPower::Draw()
{
	DrawSphere3D(m_pos, 32, 16, 0xffffff, 0xffffff, false);
}
