// 2024 Takeru Yui All Rights Reserved.
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

		// 初期化
		void Initialize(bool useGravity = false);

		// velocityに力を加える
		void AddForce(const VECTOR& force);

		// Getter
		const VECTOR& GetPos() const { return m_pos; }
		const VECTOR& GetDir() const { return m_dir; }
		const VECTOR& GetVelocity() const { return m_velocity; }
		bool UseGravity() const { return m_isUseGravity; }

		// Setter
		void SetPos(const VECTOR& set) { m_pos = set; }
		void SetVelocity(const VECTOR& set);
		void SetUseGravity(bool set) { m_isUseGravity = set; }

	private:

		VECTOR	m_pos;
		VECTOR	m_dir;
		VECTOR	m_velocity;
		bool	m_isUseGravity;
	};

}
