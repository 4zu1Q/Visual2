#pragma once
#include "BossBase.h"
class BossPower :
	public BossBase
{
public:
	BossPower();
	virtual ~BossPower();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

private:



};

