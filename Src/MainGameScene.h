/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Scene.h"
#include "Actor.h"
#include <vector>
#include <random>

/**
* �v���C���[�����삷��A�N�^�[.
*/
class PlayerActor :public Actor {

public:
	virtual ~PlayerActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
* �v���C���[���甭�˂����e�̃A�N�^�[.
*/
class BulletActor :public Actor {

public:
	virtual ~BulletActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
* �G�i�]���r�j�̃A�N�^�[.
*/
class ZombieActor :public Actor {

public:
	virtual ~ZombieActor() = default;
	virtual void Update(float deltaTime) override;

	Actor* target = nullptr;
	float attackingTimer = 1.0f;
	bool isAttacking = false;
	float baseSpeed = 1.0f;
};

/**
* ���C���Q�[�����.
*/
class MainGameScene : public Scene {

public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;
	
	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Finalize() override;

	bool IsFinish() const;

private:
	MeshList meshList;

	Texture::Image2D texId;
	Texture::Image2D texId2;
	Texture::Image2D texId3;
	Texture::Image2D texTree;
	Texture::Image2D texHouse;
	Texture::Image2D texRock;
	Texture::Image2D texGround;
	Texture::Image2D texHuman;
	Texture::Image2D texBarrel;
	Texture::Image2D texHome;
	Texture::Image2D texLeaf;
	Texture::Image2D texMushroom;
	Texture::Image2D texTwinte;
	Texture::Image2D texCity;
	Texture::Image2D texMountain;
	Texture::Image2D texZombie;

	// ���\���p�e�N�X�`��.
	Texture::Image2D texScore;
	Texture::Image2D texNumber[10];
	Texture::Image2D texHP;
	Texture::Image2D texStageClear;
	Texture::Image2D texGameOver;
	Texture::Image2D texDay;
	Texture::Image2D texPause;

	Shader::Program progSimple;
	Shader::Program progLighting;
	Shader::LightList lights;

	float angleY = 0;
	glm::vec3 viewPos;

	PlayerActor player;
	std::vector<Actor*> playerBullets;
	std::vector<Actor*> enemies;
	std::vector<Actor*> objects;
	std::vector<Actor*> items;

	std::mt19937 random;

	// �Q�[���̏��.
	enum class State {
		play,		// �ʏ�v���C��.
		stageClear,	// �X�e�[�W�N���A.
		gameOver,	// �Q�[���I�[�o�[.
		pause,		// �ꎞ��~.
	};
	State state = State::play;

	float playerBulletTimer = 0;
	float timer;
	bool isFinish;
	int stageNo = 1;
	int score = 0;
	float enemySpeed = 1.0f;	// �]���r�̈ړ����x.
	float enemyPoppingInterval = 15.0f;	// �]���r�̏o���Ԋu.
	float enemyPoppingTimer = 0.0f;	// �]���r�̏o���^�C�}�[.
	int enemyTotal = 20;		// �G�̑���.
	int enemyLeft = 20;		// ���o��̓G�̐�.�G���o�������邽�тɌ������Ă���.
	int enemyKilled = 0;		// �|�����G�̐�.���̐��l��enemyTotal�Ɠ������Ȃ�����Q�[���N���A.
};

#endif //MAINGAMESCENE_H_INCLUDED

