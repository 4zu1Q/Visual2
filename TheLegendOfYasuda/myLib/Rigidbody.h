#pragma once

namespace MyLib 
{

	/// <summary>
	/// 物理、衝突判定に必要なデータの塊
	/// </summary>
	class Rigidbody
	{
	public:

		//コンストラクタ
		Rigidbody();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="useGravity">ture : 重力あり　false : 重力なし</param>
		void Initialize(bool useGravity = false);

		// velocityに力を加える
		void AddForce(const VECTOR& force);

		/*Getter*/
		const VECTOR& GetPos() const { return m_pos; }
		const VECTOR& GetNextPos() const { return m_nextPos; }
		const VECTOR& GetDir() const { return m_dir; }
		const VECTOR& GetVelocity() const { return m_velocity; }
		bool UseGravity() const { return m_isUseGravity; }

		/*Setter*/
		void SetPos(const VECTOR& set) { m_pos = set; }
		void SetNextPos(const VECTOR& set) { m_nextPos = set; }
		void SetVelocity(const VECTOR& set);
		void SetUseGravity(bool set) { m_isUseGravity = set; }

	private:
		//現在座標
		VECTOR	m_pos;
		//移動予定座標
		VECTOR m_nextPos;

		//向き
		VECTOR	m_dir;
		//移動速度
		VECTOR	m_velocity;
		//重力を与えるかどうかのフラグ
		bool	m_isUseGravity;
	};

}
