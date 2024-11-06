#pragma once

#include "myLib/Collidable.h"

#include <list>
#include <vector>

namespace MyLib
{

	class Collidable;

	/// <summary>
	/// 物理・衝突判定するオブジェクトを登録し、物理移動・衝突を通知する
	/// </summary>
	class Physics final
	{
	public:
		// 衝突物の登録・登録解除
		void Entry(Collidable* collidable);
		void Exit(Collidable* collidable);

		void Update();	// 更新（登録オブジェクトの物理移動、衝突通知）

		// 指定ラインがオブジェクトとぶつかっているかどうか判定し、
		// ぶつかっているオブジェクトを返す
		std::list<Collidable*> IsCollideLine(const VECTOR& start, const VECTOR& end) const;

		// 重力と最大重力加速度
		static constexpr float m_kGravity = -0.1f;
		static constexpr float m_kMaxGravityAccel = -0.65f;

	private:

		std::list<Collidable*> collidables;	// 登録されたCollidableのリスト

		// OnCollideの遅延通知のためのデータ
		struct OnCollideInfo
		{
			Collidable* owner_;
			Collidable* colider_;
			void OnCollide() { owner_->OnCollide(*colider_); }
		};

		// 当たり判定チェック
		std::vector<OnCollideInfo> CheckColide() const;
		bool IsCollide(const Collidable* objA, const Collidable* objB) const;

		// 位置補正、決定
		void FixNextPosition(Collidable* primary, Collidable* secondary) const;
		void FixPosition();
	};

}
