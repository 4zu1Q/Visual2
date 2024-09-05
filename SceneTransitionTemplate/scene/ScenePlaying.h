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

	//���f���ϐ�
	int m_modelHeartH;

	//�t���[���֌W
	int m_frameScene;
	int m_attackFrameHit;
	int m_skillFrameHit;
	int m_frameDamage;

	//�t�F�[�h�t���O
	bool m_isInterval;
	bool m_isTitle;

	//�����蔻��
	bool m_isPlayerHit;	//�v���C���[�ƓG���������ꍇ
	bool m_isAttackHit;	//�v���C���[�̍U�������������ꍇ
	bool m_isDamageHit;	//�G�̍U�������������ꍇ
	bool m_isSkillHit;

	float m_selectAnimation;

	//enum�ϐ�
	Select m_select;
	int m_selectPosY;

	//���j���[
	bool m_isMenu;

	bool m_isDamageCount;
	bool m_isAttackHitCount;
	bool m_isSkillHitCount;
	bool m_isCommand;

};

