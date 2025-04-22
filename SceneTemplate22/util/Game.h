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
	const char* const kTitleText = "TheLegendOfYasuda";


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


	//プレイヤーの特殊攻撃の種類(顔の種類)
	enum class e_PlayerKind : int
	{
		kPowerPlayer,		//パワー型
		kSpeedPlayer,		//スピード型
		kShotPlayer,		//ショット型
		kRassPlayer,		//ラスボス型
		kNormalPlayer,		//ノーマル型
		kMax,				// 最大数
	};

	//ボスの種類
	enum class e_BossKind
	{
		kPower,		//パワー
		kSpeed,		//スピード
		kShot,			//ショット
		kRast,			//ラスボス
		kBossNum		//ボスの種類の総数
	};

	//ボスの攻撃種類
	enum class e_BossAttackKind
	{
		kBossAttackNone,
		kBossAttack,	//手の攻撃
		kBossShock,		//衝撃波の攻撃
		kBossWeapon,	//武器の攻撃
	};

	//敵の攻撃種類
	enum class e_EnemyAttackKind
	{
		kEnemyAttackNone,
		kEnemyAttack,			//手の攻撃
		kEnemyJumpAttack,		//衝撃波の攻撃
	};

	//プレイヤーの攻撃種類
	enum class e_PlayerAttackKind
	{
		kPlayerAttackNone,	
		kPlayerAttackX,		//Xボタンの攻撃
		kPlayerAttackY,		//Yボタンの攻撃
		kPlayerShock,		//衝撃波の攻撃
		kPlayerShot,		//玉の攻撃
	};

	//ステージの種類
	enum class e_StageKind
	{
		kTitle,
		kTutorial,
		kSelect,
		kGamePlay,
		kStageNum		//ステージの種類の総数
	};

	//演出用のプレイヤーの位置
	enum class e_PlayerProduction
	{
		kTitle,
		kGameOver,
		kGameClear,
	};

	// 当たったものの判別を行うためのタグ
	enum class e_GameObjectTag
	{
		kPlayer,			// プレイヤー
		kCamera,			// カメラ
		kAttack,			// 攻撃
		
		kItemHp,			// アイテム(体力回復用)
		kItemMp,			// アイテム(魔力回復用)
		kItemWarp,			// アイテム(ゲームセレクトへワープするアイテム)
		
		kEnemy,				// 敵
		kBoss,				// ボス
		kBossAttack1,		// 攻撃1(ボス用)
		kBossAttack2,		// 攻撃2(ボス用)
		kBossAttack3,		// 攻撃3(ボス用)
		kBossShot,			// 弾(ボス用) //できればやりたい…
		kGround,			// 足場の地面
		kWall,
	};

	//チュートリアル進行用のenum
	enum e_TutorialProgress
	{
		kTutorialStart,			//始まって10秒後
		kTutorialJump,			//ジャンプ
		kTutorialJumpClear,		//ジャンプクリア
		kTutorialDashJump,		//ダッシュジャンプ
		kTutorialDashJumpClear,	//ダッシュジャンプクリア
		kTutorialAction,		//一通りのアクション
		kTutorialActionClear,	//一通りのアクションクリア
		kTutorialBoss,			//ボス戦
		kTutorialBossClear,		//ボス戦クリア
		kTutorialMask,			//マスクを拾ったとき
	};

}
