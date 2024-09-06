#include "ItemBase.h"
#include "Player.h"

namespace
{



	//�����蔻��
	constexpr float kAddPosY = 8.0f;
}

ItemBase::ItemBase():
	m_handle(-1),
	m_pos(VGet(0,0,0)),
	m_radius(6.0f)
{
	//m_pPlayer = std::make_shared<Player>();
}

void ItemBase::ItemLost()
{
	
}

bool ItemBase::ItemSphereFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}
