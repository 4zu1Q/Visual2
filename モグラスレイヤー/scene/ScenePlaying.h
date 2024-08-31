#pragma once
#include "SceneBase.h"
#include "Camera.h"

class Camera;
class Player;
class Enemy;
class GimmickObj;
class Stage;
class SkyDome;
class ScenePlaying :
    public SceneBase
{
public:
    ScenePlaying();
    virtual ~ScenePlaying();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<GimmickObj> m_pGimmick;
	std::shared_ptr<Stage> m_pStage;
	std::shared_ptr<SkyDome> m_pSkyDome;

	//�t���[���֌W
	int m_frameScene;

	//�t���O
	bool m_isInterval;

	//�����蔻��
	bool m_isPlayerHit;
	bool m_isAttackHit;
	bool m_isGimmickHit;
};

