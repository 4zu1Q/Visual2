#include "FaceUi.h"

#include "object/player/PlayerBase.h"

namespace
{
	constexpr int kFacePosX = 32;
	constexpr int kFacePosY = 150;

	constexpr int kRadius = 16;
}

FaceUi::FaceUi()
{
	m_pPlayer = std::make_shared<PlayerBase>();
}

FaceUi::~FaceUi()
{

}

void FaceUi::Update()
{



}

void FaceUi::Draw()
{

	if (m_pPlayer->GetFaceKind() == PlayerBase::e_PlayerKind::kNormalPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xffffff, true, true);

	}
	else if (m_pPlayer->GetFaceKind() == PlayerBase::e_PlayerKind::kPowerPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xffff0f, true, true);

	}
	else if (m_pPlayer->GetFaceKind() == PlayerBase::e_PlayerKind::kSpeedPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xfff0ff, true, true);

	}
	else if (m_pPlayer->GetFaceKind() == PlayerBase::e_PlayerKind::kShotPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xff0fff, true, true);

	}
	else if (m_pPlayer->GetFaceKind() == PlayerBase::e_PlayerKind::kHuckShotPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xf0ffff, true, true);

	}
	else if (m_pPlayer->GetFaceKind() == PlayerBase::e_PlayerKind::kStrongestPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0x0fffff, true, true);

	}

}
