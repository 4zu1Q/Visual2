#pragma once
#include "SceneBase.h"
#include "Vec2.h"

#include "DxLib.h"

#include <vector>
#include <memory>

class SceneTest : public SceneBase
{
public:
	SceneTest();
	virtual ~SceneTest() {}

	virtual void startLoad() override;
	virtual bool isLoaded() const override;

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

private:

	// �O���b�h�̕`��
	void drawGrid();

private:
	int m_frameCount;

	//���̂̈ʒu
	VECTOR m_objPos;

	//�|���S���̒��_�f�[�^
	std::vector<VERTEX3D> m_fieldVtx;

};