#pragma once
#include "DxLib.h"

class Stage
{
public:
	Stage();
	virtual ~Stage();

	void Load();
	void Delete();

	void Init();
	void Update();
	void Draw();

private:

	int m_modelHandle;
	VECTOR m_pos;

	int m_light;
};

