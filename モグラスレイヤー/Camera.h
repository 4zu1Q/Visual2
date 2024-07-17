#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy;
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Init();

	void TitleCameraUpdate();
	void PlayCameraUpdate(const Player& player);		//�Q�[���v���C�p�̃J�����ݒ�
	void TargetCameraUpadate();		//���b�N�I���@�\�̃J�����ݒ�

	const float GetAngle() const { return m_cameraAngle; }

	const VECTOR& GetPos() const { return m_pos; }
	const VECTOR& GetTargetPos() const { return m_targetPos; }

private:

	std::shared_ptr<Player> m_pPlayer;

	void DrawGrid();

	VECTOR m_pos;			//�J�������W
	VECTOR m_targetPos;		//�����_���W

	float m_cameraAngle;	//�J�����p�x




};

