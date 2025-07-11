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

	//使う画像の種類
	enum e_Ui
	{
		kButtonAH,
		kButtonBH,
		kButtonXH,
		kButtonYH,
		kButtonRBH,
		kButtonLBH,
		kPushButtonAH,
		kPushButtonBH,
		kPushButtonXH,
		kPushButtonYH,
		kPushButtonRBH,
		kPushButtonLBH,
		kButtonPauseH,
	};

}

ButtonUi::ButtonUi():
	//m_kind(e_ButtonKind::kNone),
	m_isPush(false)
{
	//画像のロード
	m_handles.push_back(LoadGraph(kButtonAFileName));
	m_handles.push_back(LoadGraph(kButtonBFileName));
	m_handles.push_back(LoadGraph(kButtonXFileName));
	m_handles.push_back(LoadGraph(kButtonYFileName));				
	m_handles.push_back(LoadGraph(kRBFileName));				
	m_handles.push_back(LoadGraph(kLBFileName));				
	m_handles.push_back(LoadGraph(kPushButtonAFileName));
	m_handles.push_back(LoadGraph(kPushButtonBFileName));
	m_handles.push_back(LoadGraph(kPushButtonXFileName));
	m_handles.push_back(LoadGraph(kPushButtonYFileName));
	m_handles.push_back(LoadGraph(kPushRBFileName));
	m_handles.push_back(LoadGraph(kPushLBFileName));
	m_handles.push_back(LoadGraph(kButtonPauseFileName));
}

ButtonUi::~ButtonUi()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	m_handles.clear();
}

void ButtonUi::Initialize()
{

}

void ButtonUi::Update()
{

}

void ButtonUi::Draw(Player& player)
{
	DrawGraph(kButtonAPos.x, kButtonAPos.y, m_handles[kButtonAH], true);
	DrawGraph(kButtonBPos.x, kButtonBPos.y, m_handles[kButtonBH], true);
	DrawGraph(kButtonXPos.x, kButtonXPos.y, m_handles[kButtonXH], true);
	DrawGraph(kButtonYPos.x, kButtonYPos.y, m_handles[kButtonYH], true);
	DrawGraph(kRBPos.x, kRBPos.y, m_handles[kButtonRBH], true);
	DrawGraph(kLBPos.x, kLBPos.y, m_handles[kButtonLBH], true);

	if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kAbutton)
	{
		DrawGraph(kButtonAPos.x, kButtonAPos.y, m_handles[kPushButtonAH], true);

	}
	else if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kBbutton)
	{
		DrawGraph(kButtonBPos.x, kButtonBPos.y, m_handles[kPushButtonBH], true);

	}
	else if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kXbutton)
	{
		DrawGraph(kButtonXPos.x, kButtonXPos.y, m_handles[kPushButtonXH], true);

	}
	else if (player.GetIsButtonPush() && player.GetButtonKind() == Player::e_ButtonKind::kYbutton)
	{
		DrawGraph(kButtonYPos.x, kButtonYPos.y, m_handles[kPushButtonYH], true);

	}

	if (player.GetIsFaceUse())
	{
		DrawGraph(kRBPos.x, kRBPos.y, m_handles[kPushButtonRBH], true);
		DrawGraph(kLBPos.x, kLBPos.y, m_handles[kPushButtonLBH], true);
	}


	DrawGraph(kButtonPausePos.x, kButtonPausePos.y, m_handles[kButtonPauseH], true);
}
