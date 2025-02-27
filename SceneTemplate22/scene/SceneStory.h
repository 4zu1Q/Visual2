#pragma once
#include "scene/SceneBase.h"
#include <vector>
#include <memory>

class SceneStory : public SceneBase
{
public:
	SceneStory(SceneManager& manager);
	virtual ~SceneStory();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	void Update01();
	void Update02();

	void OnUpdate02();

	void Next();

private:

	//メンバ関数ポインタの場所をだすため
	enum e_Scene
	{
		kScene1,
		kScene2,

	};

	//メンバ関数ポインタ
	using UpdateFunc_t = void(SceneStory::*)(/*引数書く*/);
	UpdateFunc_t m_updateFunc;

	bool m_isNext;	//押したら次に進む
	bool m_isSkip;	//押したらスキップするフラグ

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//
	e_Scene m_sceneTrans;

	//タイム変数
	int m_storyTime;
	int m_pressTime;

	//カーソルアニメーション
	float m_selectAnimation;

};

