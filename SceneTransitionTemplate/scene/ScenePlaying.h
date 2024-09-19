#pragma once
#include "SceneBase.h"
#include "Camera.h"
#include <vector>
#include <memory>

class Camera;
class Player;
class Enemy;
class Stage;
class SkyDome;
class ItemBase;
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

	void CreateItemHp(VECTOR pos);	//HP���񕜂���A�C�e���𐶐�

	void Knockback(VECTOR pos ,VECTOR move ,float length);

private:

	//�J�[�\���I��
	enum Select
	{
		kRestart,		//�X�^�[�g
		kOption,	//�I�v�V����
		kTitle,		//�^�C�g���ړ�
	};

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Stage> m_pStage;
	std::shared_ptr<SkyDome> m_pSkyDome;
	std::vector<ItemBase*> m_pItem;

	//�n���h���ϐ�
	int m_restartH;
	int m_optionH;
	int m_titleH;
	int m_selectH;
	int m_operatorH;

	int m_hitSH;
	int m_itemSH;
	int	m_hitPH;
	int	m_itemPH;

	//���f���ϐ�
	int m_modelHeartH;

	//�t���[���֌W
	int m_frameScene;
	int m_fadeFrameScene;
	int m_playerAttackHitFrame;
	int m_playerSkillHitFrame;
	int m_playerFrame;
	int m_enemyAttackHitFrame;
	int m_enemySkillHitFrame;
	int m_enemyFrame;

	

	//�t�F�[�h�t���O
	bool m_isWin;
	bool m_isLose;
	bool m_isInterval;
	bool m_isTitle;

	//�����蔻��
	bool m_isPlayerHit;	//�v���C���[�ƓG���������ꍇ
	bool m_isPlayerAttackHit;	//�v���C���[�̍U�������������ꍇ
	bool m_isPlayerSkillHit;	//�v���C���[�̃X�L�������������ꍇ
	bool m_isEnemyAttackHit;	//�G�̍U�������������ꍇ
	bool m_isEnemySkillHit;	//�G�̍U�������������ꍇ
	bool m_isItemHit;	//�A�C�e���ƃv���C���[�����������ꍇ

	//���񂾔���
	bool m_isEnemyDeath;
	bool m_isPlayerDeath;

	bool m_isEnemySearch;
	bool m_isEnemyStop;

	float m_selectAnimation;

	//enum�ϐ�
	Select m_select;
	int m_selectPosY;

	//���j���[
	bool m_isMenu;

	bool m_isEnemyAttackHitCount;
	bool m_isEnemySkillHitCount;
	bool m_isPlayerAttackHitCount;
	bool m_isPlayerSkillHitCount;
	bool m_isCommand;
	bool m_isOption;

	int m_soundBgmH;
	int m_soundSelectH;
	int m_soundCancelH;
	int m_soundDecsionH;

	int m_soundPlayerDamageH;
	int m_soundADamageH;
	int m_soundSDamageH;
	int m_soundItemH;

};

