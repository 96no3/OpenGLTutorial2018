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
* プレイヤーが操作するアクター.
*/
class PlayerActor :public Actor {

public:
	virtual ~PlayerActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
* プレイヤーから発射される弾のアクター.
*/
class BulletActor :public Actor {

public:
	virtual ~BulletActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
* 敵（ゾンビ）のアクター.
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
* メインゲーム画面.
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

	// 情報表示用テクスチャ.
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

	// ゲームの状態.
	enum class State {
		play,		// 通常プレイ中.
		stageClear,	// ステージクリア.
		gameOver,	// ゲームオーバー.
		pause,		// 一時停止.
	};
	State state = State::play;

	float playerBulletTimer = 0;
	float timer;
	bool isFinish;
	int stageNo = 1;
	int score = 0;
	float enemySpeed = 1.0f;	// ゾンビの移動速度.
	float enemyPoppingInterval = 15.0f;	// ゾンビの出現間隔.
	float enemyPoppingTimer = 0.0f;	// ゾンビの出現タイマー.
	int enemyTotal = 20;		// 敵の総数.
	int enemyLeft = 20;		// 未登場の敵の数.敵を出現させるたびに減少していく.
	int enemyKilled = 0;		// 倒した敵の数.この数値がenemyTotalと等しくなったらゲームクリア.
};

#endif //MAINGAMESCENE_H_INCLUDED

