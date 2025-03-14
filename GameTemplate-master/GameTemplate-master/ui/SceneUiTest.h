#pragma once
#include "SceneBase.h"
#include <memory>

//class UiBase;
class UiScroll;
class SceneUiTest : public SceneBase
{
public:
	SceneUiTest();
	virtual ~SceneUiTest() {}

	virtual void startLoad() override;
	virtual bool isLoaded() const override;

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

private:
	int m_frameCount;
	std::shared_ptr<UiScroll>	m_pTestMenu;
};

