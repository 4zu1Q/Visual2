#pragma once
#include "DxLib.h"

class CharaBase
{
public:

	CharaBase();
	virtual ~CharaBase();

	void Initialize();
	void Finalize();

	virtual void Update();
	virtual void Draw();



	/// <summary>
	/// 継承しているものに重量を与える関数
	/// </summary>
	void Gravity();

private:



};

