#pragma once
#include "DxLib.h"
#include "util/Vec2.h"
#include <vector>
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

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//押されているかどうかのフラグ
	bool m_isPush;

};

