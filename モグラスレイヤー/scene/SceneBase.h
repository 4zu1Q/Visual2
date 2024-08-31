#pragma once
#include <memory>

class SceneManager;
class SceneBase :public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase() {}

	
	virtual void Init() = 0;	//シーンに入るときの初期化処理

														//毎フレーム行う更新処理
	virtual std::shared_ptr<SceneBase> Update() = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
														//シーン変更しない場合は自身のポインタ
										
	virtual void Draw() = 0;	//毎フレーム行う描画処理
	virtual void End() = 0;	//シーンを抜けるときの処理

	//フェード関係
	void UpdateFade();
	void DrawFade();

	void SkipFadeIn();
	void SkipFadeOut();

	bool IsFadeingIn() const;
	bool IsFadeingOut() const;

	bool IsFadeing() const { return IsFadeingIn() || IsFadeingOut(); }	//フェードインorアウト中

	void StartFadeOut();	//フェードアウト開始



private:
	//フェード関連
	int m_fadeColor;
	int m_fadeBright;
	int m_fadeSpeed;

protected:
	SceneManager& m_pManager;

};

