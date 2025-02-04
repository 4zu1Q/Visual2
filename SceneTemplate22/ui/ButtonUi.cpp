#include "ButtonUi.h"
#include "object/player/Player.h"

namespace
{
	/* UIファイル名 */
	//押されていないときの画像ファイル名
	const char* const kButtonAFileName = "Data/Image/ButtonA.png";
	const char* const kButtonBFileName = "Data/Image/ButtonB.png";
	const char* const kButtonXFileName = "Data/Image/ButtonX.png";
	const char* const kButtonYFileName = "Data/Image/ButtonY.png";
	const char* const kButtonPauseFileName = "Data/Image/ButtonPause.png";
	const char* const kRBFileName = "Data/Image/RB1.png";
	const char* const kLBFileName = "Data/Image/LB1.png";

	//押されているときの画像ファイル名
	const char* const kPushButtonAFileName = "Data/Image/ButtonA_Push.png";
	const char* const kPushButtonBFileName = "Data/Image/ButtonB_Push.png";
	const char* const kPushButtonXFileName = "Data/Image/ButtonX_Push.png";
	const char* const kPushButtonYFileName = "Data/Image/ButtonY_Push.png";
	const char* const kPushRBFileName = "Data/Image/RB2.png";
	const char* const kPushLBFileName = "Data/Image/LB2.png";

	//ボタンUIの配置座標
	const Vec2 kButtonAPos = { 1145.0f , 222.0f };
	const Vec2 kButtonBPos = { 1191.0f , 175.0f };
	const Vec2 kButtonXPos = { 1099.0f , 175.0f };
	const Vec2 kButtonYPos = { 1145.0f , 130.0f };
	const Vec2 kButtonPausePos = { 1219.0f , 658.0f };

	const Vec2 kRBPos = { 1220.0f , 36.0f };
	const Vec2 kLBPos = { 1040.0f , 36.0f };



}

ButtonUi::ButtonUi():
	//m_kind(e_ButtonKind::kNone),
	m_isPush(false),
	m_buttonAHandle(LoadGraph(kButtonAFileName)),
	m_buttonBHandle(LoadGraph(kButtonBFileName)),
	m_buttonXHandle(LoadGraph(kButtonXFileName)),
	m_buttonYHandle(LoadGraph(kButtonYFileName)),
	m_buttonRBHandle(LoadGraph(kRBFileName)),
	m_buttonLBHandle(LoadGraph(kLBFileName)),
	m_pushButtonAHandle(LoadGraph(kPushButtonAFileName)),
	m_pushButtonBHandle(LoadGraph(kPushButtonBFileName)),
	m_pushButtonXHandle(LoadGraph(kPushButtonXFileName)),
	m_pushButtonYHandle(LoadGraph(kPushButtonYFileName)),
	m_pushButtonRBHandle(LoadGraph(kPushRBFileName)),
	m_pushButtonLBHandle(LoadGraph(kPushLBFileName)),
	m_buttonPauseHandle(LoadGraph(kButtonPauseFileName))
{

}

ButtonUi::~ButtonUi()
{
	DeleteGraph(m_buttonAHandle);
	DeleteGraph(m_buttonBHandle);
	DeleteGraph(m_buttonXHandle);
	DeleteGraph(m_buttonYHandle);
	DeleteGraph(m_buttonRBHandle);
	DeleteGraph(m_buttonLBHandle);

	DeleteGraph(m_pushButtonAHandle);
	DeleteGraph(m_pushButtonBHandle);
	DeleteGraph(m_pushButtonXHandle);
	DeleteGraph(m_pushButtonYHandle);
	DeleteGraph(m_pushButtonRBHandle);
	DeleteGraph(m_pushButtonLBHandle);

	DeleteGraph(m_buttonPauseHandle);
}

void ButtonUi::Initialize()
{

}

void ButtonUi::Update()
{

}

void ButtonUi::Draw(Player& player)
{
	DrawGraph(kButtonAPos.x, kButtonAPos.y, m_buttonAHandle, true);
	DrawGraph(kButtonBPos.x, kButtonBPos.y, m_buttonBHandle, true);
	DrawGraph(kButtonXPos.x, kButtonXPos.y, m_buttonXHandle, true);
	DrawGraph(kButtonYPos.x, kButtonYPos.y, m_buttonYHandle, true);
	DrawGraph(kRBPos.x, kRBPos.y, m_buttonRBHandle, true);
	DrawGraph(kLBPos.x, kLBPos.y, m_buttonLBHandle, true);

	if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kAbutton)
	{
		DrawGraph(kButtonAPos.x, kButtonAPos.y, m_pushButtonAHandle, true);

	}
	else if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kBbutton)
	{
		DrawGraph(kButtonBPos.x, kButtonBPos.y, m_pushButtonBHandle, true);

	}
	else if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kXbutton)
	{
		DrawGraph(kButtonXPos.x, kButtonXPos.y, m_pushButtonXHandle, true);

	}
	else if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kYbutton)
	{
		DrawGraph(kButtonYPos.x, kButtonYPos.y, m_pushButtonYHandle, true);

	}

	if (player.GetIsFaceUse())
	{
		DrawGraph(kRBPos.x, kRBPos.y, m_pushButtonRBHandle, true);
		DrawGraph(kLBPos.x, kLBPos.y, m_pushButtonLBHandle, true);
	}


	DrawGraph(kButtonPausePos.x, kButtonPausePos.y, m_buttonPauseHandle, true);
}
