#pragma once

#include "myLib/Collidable.h"
#include "object/stage/Field.h"

#include <memory>
#include <list>
#include <vector>


class Field;
class Player;

namespace MyLib
{

	namespace ColInfo
	{
		//最大当たり判定ポリゴン数
		constexpr int kMaxColHitPolyNum = 2000;
		//壁押し出し処理の最大試行回数
		constexpr int kMaxColHitTryNum = 16;
		//壁押し出し時にスライドさせる距離
		constexpr float kColHitSlideLength = 1.0f;
	}

	class Collidable;

	/// <summary>
	/// 物理・衝突判定するオブジェクトを登録し、物理移動・衝突を通知する
	/// </summary>
	class Physics final
	{
	public:

		Physics(Game::e_StageKind stageKind);

		// 衝突物の登録・登録解除
		void Entry(std::shared_ptr<Collidable> collidable );
		void Exit(std::shared_ptr<Collidable> collidable );

		/// <summary>
		/// 更新（登録オブジェクトの物理移動、衝突通知）
		/// </summary>
		void Update();

	private:

		/// <summary>
		/// 二つのオブジェクトが当たっているかどうかを判定する
		/// </summary>
		/// <param name="objA">当たっているオブジェクトの一つ</param>
		/// <param name="objB">当たっているオブジェクトの一つ</param>
		/// <returns></returns>
		bool IsCollide(const Collidable* objA, const Collidable* objB) const;

		/// <summary>
		/// 位置補正
		/// </summary>
		/// <param name="primary">当たっているオブジェクトの一つ</param>
		/// <param name="secondary">当たっているオブジェクトの一つ</param>
		void FixNextPosition(Collidable* primary, Collidable* secondary) const;
		
		/// <summary>
		/// 位置の最終補正を決定する関数
		/// </summary>
		void FixPosition();

	private:

		/// <summary>
		/// チェックしたポリゴンが壁ポリゴンか床ポリゴンかを判断し保存する
		/// </summary>
		/// <param name="col">登録したコライダー情報</param>
		void CheckWallAndFloor(std::shared_ptr<Collidable>& col);

		/// <summary>
		/// 壁ポリゴンとの当たり判定をチェックし、移動させる
		/// </summary>
		/// <param name="col">登録したコライダー情報</param>
		void FixPositionWithWall(std::shared_ptr<Collidable>& col);

		/// <summary>
		/// 壁の中から押し出す
		/// </summary>
		/// <param name="col">登録したコライダー情報</param>
		void FixPositionWithWallInternal(std::shared_ptr<Collidable>& col);

		/// <summary>
		/// 床ポリゴンとの当たり判定をチェックし、移動させる
		/// </summary>
		/// <param name="col">登録したコライダー情報</param>
		void FixNowPositionWithFloor(std::shared_ptr<Collidable>& col);

		//当たり判定の種類
		enum class eOnCollideInfoKind
		{
			CollideEnter,
			CollideStay,
			CollideExit,
			TriggerEnter,
			TriggerStay,
			TriggerExit
		};

		// OnCollideの遅延通知のためのデータ
		struct OnCollideInfo_
		{
			Collidable* owner_;
			Collidable* colider_;
			eOnCollideInfoKind colKind_;
			//void OnCollide() { owner_->OnCollide(*colider_); }
		};

	private:

		//フィールドのポインタ
		std::shared_ptr<Field> m_pField;

		// 当たり判定チェック
		std::vector<OnCollideInfo_> CheckColide() const;

		// 重力と最大重力加速度
		static constexpr float m_gravity = -0.35f;
		static constexpr float m_maxGravityAccel = -1.8f;

		std::list<std::shared_ptr<Collidable>> m_collidables;	// 登録されたCollidableのリスト
		//std::list<std::shared_ptr<Collidable*>> m_collidables;	// 登録されたCollidableのリスト

		//壁と床のポリゴン数を入れるための変数
		int m_wallNum;
		int m_floorNum;

		bool m_isHitFlag;

		//当たり判定結果構造体
		MV1_COLL_RESULT_POLY_DIM m_hitDim{};
		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
		MV1_COLL_RESULT_POLY* m_pWallPoly[ColInfo::kMaxColHitPolyNum]{};
		// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
		MV1_COLL_RESULT_POLY* m_pFloorPoly[ColInfo::kMaxColHitPolyNum]{};
		// ポリゴンの構造体にアクセスするために使用するポインタ
		MV1_COLL_RESULT_POLY* m_pPoly = nullptr;
		// 線分とポリゴンとの当たり判定の結果を代入する構造体
		HITRESULT_LINE m_lineRes{};

		std::shared_ptr<Player> m_pPlayer;

	};

}
