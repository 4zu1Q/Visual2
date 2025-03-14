#pragma once
#include "SceneBase.h"
#include <memory>

class UiBase;
class ScenePreviewFont : public SceneBase
{
public:
	ScenePreviewFont();
	virtual ~ScenePreviewFont() {}

	virtual void startLoad() override;
	virtual bool isLoaded() const override;

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

private:
	int m_frameCount;

	std::shared_ptr<UiBase>	m_pFontNameMenu;
};

