#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Tomb
{
public:
	Tomb();
	virtual ~Tomb();

	void Initialize();
	void Finalize();

	virtual void Update();
	virtual void Draw();


	bool TombHit(std::shared_ptr<Player> pPlayer);


private:

	VECTOR m_pos

};

