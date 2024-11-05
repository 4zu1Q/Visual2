#pragma once
#include "DxLib.h"

#define ScreenWidth 1280
#define ScreenHeight 720

// ゲーム全体で使用する定数の定義
namespace Game
{

	//ウィンドウモード設定
#ifdef _DEBUG 
	constexpr bool kWindowMode = false;
#else
	constexpr bool kWindowMode = false;
#endif

	//ウィンドウ名
	const char* const kTitleText = "マイフェイス";


	//ウィンドウサイズ
	constexpr int kScreenWidth = ScreenWidth;
	constexpr int kScreenHeight = ScreenHeight;

	//constexpr int kScreenWidth = 1920;
	//constexpr int kScreenHeight = 1080;
	

	//ウィンドウサイズ * 0.5
	constexpr int kScreenWidthHalf = ScreenWidth * 0.5f;
	constexpr int kScreenHeightHalf = ScreenHeight * 0.5f;

	//ウィンドウカラーモード
	constexpr int kColorDepth = 32; //16 or 32

	//ステージ名
	enum class e_StageKind
	{
		kPowerStage,		//パワーステージ
		kSpeedStage,		//スピードステージ
		kRastStage,			//ラスボスステージ
	};

	// 当たったものの判別を行うためのタグ
	enum class e_GameObjectTag
	{
		kPlayer,			// プレイヤー
		kBoss,			// ボス
		kItem,			// アイテム
		kShot,			// 弾		//できればやりたい…
		kSystemWall,		// システム壁
		kStepGround,		// 足場の地面
	};

}
