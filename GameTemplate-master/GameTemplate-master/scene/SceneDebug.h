#pragma once
#include "SceneBase.h"

#include <vector>
#include <memory>


class UiBase;
class UiItemBase;
class UiDebugMenu;
class SceneDebug : public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug() {}

	virtual void startLoad() override;
	virtual bool isLoaded() const override;

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	virtual float getLoadProgress() const override;

private:

private:
	int m_handle[4];
	// ロードするファイルの数
	int m_loadFileNum;

	int m_frameCount;

	std::shared_ptr<UiDebugMenu>	m_pDebugMenu;
};