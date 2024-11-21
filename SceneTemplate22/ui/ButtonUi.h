#pragma once
#include "DxLib.h"
#include "util/Vec2.h"
#include <memory>

class Player;
class ButtonUi
{
public:

	ButtonUi();
	virtual ~ButtonUi();

	void Initialize();
	void Update();
	void Draw(Player& player);



	//Setter
	void SetIsButtonPush(bool isPush) { m_isPush = isPush; }


private:

	//画像ハンドル ※できれば良い感じにまとめたいもの(あきらかに非効率)
	int m_buttonAHandle;
	int m_buttonBHandle;
	int m_buttonXHandle;
	int m_buttonYHandle;

	int m_buttonRBHandle;
	int m_buttonLBHandle;


	int m_buttonPauseHandle;

	//押された用の画像ハンドル
	int m_pushButtonAHandle;
	int m_pushButtonBHandle;
	int m_pushButtonXHandle;
	int m_pushButtonYHandle;

	int m_pushButtonRBHandle;
	int m_pushButtonLBHandle;

	//押されているかどうかのフラグ
	bool m_isPush;

	//ボタンの種類
	//e_ButtonKind m_kind;

	//bool m_isButtonAPush;
	//bool m_isButtonBPush;
	//bool m_isButtonXPush;
	//bool m_isButtonYPush;

	//bool m_isButtonRBPush;
	//bool m_isButtonLBush;

};

