#pragma once

#include <memory>

class SceneBase;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void init();
	void end();

	bool update();	// ゲーム終了時falseを返す
	void draw();
private:
//	SceneBase* m_pScene;
	std::unique_ptr<SceneBase>	m_pScene;
};