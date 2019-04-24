/**
* @file MainGameScene.cpp
*/

#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* プレイヤーの状態を更新する.
*
* @param deltaTime	経過時間.
*/
void PlayerActor::Update(float deltaTime) {
	// プレイヤーが死んでいたら、地面に倒れる(-90度になる)まで「倒れ具合」を大きくしていく.
	if (health <= 0) {
		rotation.x -= glm::radians(45.0f) * deltaTime;
		if (rotation.x < glm::radians(-90.0f)) {
			rotation.x = glm::radians(-90.0f);
		}
	}
	Actor::Update(deltaTime);
}

/**
* プレイヤーの弾の状態を更新する.
*
* @param deltaTime	経過時間.
*/
void BulletActor::Update(float deltaTime) {

	const float mapSize = 30;
	for (size_t i = 0; i < 3; ++i) {
		if (position[i]< -mapSize || position[i]> mapSize) {
			health = 0;
			break;
		}
	}
	Actor::Update(deltaTime);
}

/**
* ゾンビの状態を更新する.
*
* @param deltaTime	経過時間.
*/
void ZombieActor::Update(float deltaTime) {

	if (!target) {
		return;
	}

	const float moveSpeed = baseSpeed * 2.0f;
	const float rotationSpeed = baseSpeed * glm::radians(60.0f);
	const float frontRange = glm::radians(15.0f);

	// ターゲット（プレイヤー）の方向を調べる.
	const glm::vec3 v = target->position - position;
	const glm::vec3 vTarget = glm::normalize(v);
	float radian = std::atan2(-vTarget.z, vTarget.x) - glm::radians(90.0f);
	if (radian <= 0) {
		radian += glm::radians(360.0f);
	}
	const glm::vec3 vZombieFront = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

	// ターゲットが正面にいなかったら、正面を捉えるように左右に旋回.
	if (std::abs(radian - rotation.y) > frontRange) {
		const glm::vec3 vRotDir = glm::cross(vZombieFront, vTarget);
		if (vRotDir.y >= 0) {
			rotation.y += rotationSpeed * deltaTime;
			if (rotation.y >= glm::radians(360.0f)) {
				rotation.y -= glm::radians(360.0f);
			}
		}
		else {
			rotation.y -= rotationSpeed * deltaTime;
			if (rotation.y < 0) {
				rotation.y += glm::radians(360.0f);
			}
		}
	}

	// 十分に接近していなければ移動する.接近していれば攻撃する.
	if (glm::length(v) > 1.0f) {
		velocity = vZombieFront * moveSpeed;
	}
	else {
		velocity = glm::vec3(0);	// 接近しているのでもう移動しない.
		// 定期的に攻撃状態になる.
		if (isAttacking) {
			isAttacking = false;
			attackingTimer = 1.0f;	// 次の攻撃は1秒後.
		}
		else {
			attackingTimer -= deltaTime;
			if (attackingTimer <= 0) {
				isAttacking = true;
			}
		}
	}

	Actor::Update(deltaTime);
}

/**
* 初期化.
*/
bool MainGameScene::Initialize() {
	
	random.seed(std::random_device()());

	/*if (!meshList.Allocate()) {
		return false;
	}*/
	std::vector<std::string> modelFiles;
	modelFiles.push_back("Res/Model/Tree.obj");
	modelFiles.push_back("Res/Model/house2.obj");
	modelFiles.push_back("Res/Model/Rock2.obj");
	modelFiles.push_back("Res/Model/Ground.obj");
	modelFiles.push_back("Res/Model/human.obj");
	modelFiles.push_back("Res/Model/Barrel.obj");
	modelFiles.push_back("Res/Model/home.obj");
	modelFiles.push_back("Res/Model/leaf.obj");
	modelFiles.push_back("Res/Model/mushroom.obj");
	modelFiles.push_back("Res/Model/twinte.obj");
	modelFiles.push_back("Res/Model/city.obj");
	modelFiles.push_back("Res/Model/mountain.obj");
	modelFiles.push_back("Res/Model/Plane.obj");
	if (!meshList.Allocate(modelFiles)) {
		return false;
	}

	progSimple.Reset(Shader::BuildFromFile("Res/Shader/Simple.vert", "Res/Shader/Simple.frag"));
	progLighting.Reset(Shader::BuildFromFile("Res/Shader/FragmentLighting.vert", "Res/Shader/FragmentLighting.frag"));

	// テクスチャを作成する.
	const int imageWidth = 8;			// 画像の幅.
	const int imageHeight = 8;			// 画像の高さ.
	const uint32_t B = 0xff'00'00'00;	// 黒.
	const uint32_t W = 0xff'ff'ff'ff;	// 白.
	const uint32_t imageData[imageWidth * imageHeight] = {
		W, W, B, W, W, W, W, W,
		W, W, B, W, W, W, W, W,
		W, W, B, W, W, W, W, W,
		B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, W,
		W, W, W, W, W, W, B, W,
		W, W, W, W, W, W, B, W,
		B, B, B, B, B, B, B, B,
	};
	
	const uint32_t imageData2[imageWidth * imageHeight] = {
		B, W, W, W, B, W, W, W,
		W, B, W, W, W, B, W, W,
		W, W, B, W, W, W, B, W,
		W, W, W, B, W, W, W, B,
		B, W, W, W, B, W, W, W,
		W, B, W, W, W, B, W, W,
		W, W, B, W, W, W, B, W,
		W, W, W, B, W, W, W, B,
	};

	const uint32_t imageData3[imageWidth * imageHeight] = {
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
	};
	
	texId.Reset(Texture::CreateImage2D(imageWidth, imageHeight, imageData, GL_RGBA, GL_UNSIGNED_BYTE));
	texId2.Reset(Texture::CreateImage2D(imageWidth, imageHeight, imageData2, GL_RGBA, GL_UNSIGNED_BYTE));
	texId3.Reset(Texture::CreateImage2D(imageWidth, imageHeight, imageData3, GL_RGBA, GL_UNSIGNED_BYTE));
	texTree.Reset(Texture::LoadImage2D("Res/Model/Tree2.tga"));
	texHouse.Reset(Texture::LoadImage2D("Res/Model/house2.tga"));
	texRock.Reset(Texture::LoadImage2D("Res/Model/rock3.tga"));
	texGround.Reset(Texture::LoadImage2D("Res/Model/ground4.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/Model/human.tga"));
	texBarrel.Reset(Texture::LoadImage2D("Res/Model/Barrel.tga"));
	texHome.Reset(Texture::LoadImage2D("Res/Model/home.tga"));
	texLeaf.Reset(Texture::LoadImage2D("Res/Model/leaf.tga"));
	texMushroom.Reset(Texture::LoadImage2D("Res/Model/mushroom.tga"));
	texTwinte.Reset(Texture::LoadImage2D("Res/Model/twinte.tga"));
	texCity.Reset(Texture::LoadImage2D("Res/Model/city.tga"));
	texMountain.Reset(Texture::LoadImage2D("Res/Model/mountain.tga"));
	texZombie.Reset(Texture::LoadImage2D("Res/Model/zombie.tga"));

	// 情報表示用テクスチャを読み込む.
	texScore.Reset(Texture::LoadImage2D("Res/Image/Score.tga"));
	for (int i = 0; i < 10; ++i) {
		const std::string filename = std::string("Res/Image/Number_") + (char)('0' + i) + ".tga";
		texNumber[i].Reset(Texture::LoadImage2D(filename.c_str()));
	}
	texHP.Reset(Texture::LoadImage2D("Res/Image/HP.tga"));
	texStageClear.Reset(Texture::LoadImage2D("Res/Image/StageClear.tga"));
	texGameOver.Reset(Texture::LoadImage2D("Res/Image/GameOver.tga"));
	texDay.Reset(Texture::LoadImage2D("Res/Image/Day.tga"));
	texPause.Reset(Texture::LoadImage2D("Res/Image/pause.tga"));

	if (texId.IsNull()) {
		return false;
	}

	// ライトの設定.
	lights.ambient.color = glm::vec3(0.05f, 0.1f, 0.1f);			// 環境光の色.
	lights.directional.direction = glm::normalize(glm::vec3(5, -2, -2));	// 指向性ライトの方向.
	lights.directional.color = glm::vec3(1.0f, 1.0f, 1.0f);			// 指向性ライトの色.
	lights.point.position[0] = glm::vec3(5, 4, 0);
	lights.point.color[0] = glm::vec3(1.0f, 0.8f, 0.4f) * 100.0f;
	lights.spot.dirAndCutOff[0] = glm::vec4(glm::normalize(glm::vec3(-1, -2, -2)), std::cos(glm::radians(20.0f)));
	lights.spot.posAndInnerCutOff[0] = glm::vec4(-6, 6, 8, std::cos(glm::radians(15.0f)));
	lights.spot.color[0] = glm::vec3(0.4f, 0.8f, 1.0f) * 200.0f;

	// プレイヤーアクターの追加.
	player.Initialize(4, texHuman.Get(), 10, glm::vec3(8, 0, 8), glm::vec3(0), glm::vec3(1));
	player.colLocal = { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f, 1.7f, 1.0f) };

	// 弾アクターの初期化.
	playerBullets.resize(128);
	for (auto& bullet : playerBullets) {
		bullet = new BulletActor;
	}

	// 敵アクターの初期化.
	enemies.resize(128);
	for (auto& zombie : enemies) {
		zombie = new ZombieActor;
	}

	// 静止オブジェクトの初期化.
	objects.resize(128);
	for (auto& e : objects) {
		e = new Actor;
	}

	const float treeCount = 10;	// 木の本数.
	const float radius = 8;		// 木を植える円の半径.
	for (float i = 0; i < treeCount; ++i) {
		const float theta = 3.14f * 2 / treeCount * i;
		const float x = std::cos(theta) * radius;
		const float z = std::sin(theta) * radius;
		objects[(size_t)i]->Initialize(0, texTree.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
		objects[(size_t)i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 4, 1 } };
		objects[(size_t)i]->Update(0);
	}

	const float houseCount = 3;		// 家の数.
	const float houseRadius = 14;	// 家を置く円の半径.
	for (float i = 0; i < houseCount; i++) {
		if (i == 0) {
			objects[treeCount + i]->Initialize(1, texHouse.Get(), 1, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1));
			objects[treeCount + i]->colLocal = { {-3, 0, -3 }, { 6, 4, 6 } };
			objects[treeCount + i]->Update(0);
		}
		else {
			const float x = std::cos(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
			const float z = std::sin(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
			objects[treeCount + i]->Initialize(1, texHouse.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
			objects[treeCount + i]->colLocal = { {-3, 0, -3 }, { 6, 4, 6 } };
			objects[treeCount + i]->Update(0);
		}
	}

	const float rockCount = 4;		// 岩の数.
	const float rockRadius = 5;		// 岩を置く円の半径.
	for (float i = 0; i < rockCount; i++) {
		const float x = std::cos(3.14f * 2 / rockCount * i) * rockRadius;
		const float z = std::sin(3.14f * 2 / rockCount * i) * rockRadius;
		objects[treeCount + houseCount + i]->Initialize(2, texRock.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		objects[treeCount + houseCount + i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		objects[treeCount + houseCount + i]->Update(0);
	}

	// 地面.	
	objects[treeCount + houseCount + rockCount + 1]->Initialize(3, texGround.Get(), 1, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1));

	// 二世帯住宅.
	objects[treeCount + houseCount + rockCount + 2]->Initialize(6, texHome.Get(), 1, glm::vec3(20, 0, -17), glm::vec3(0), glm::vec3(1));
	
	// 山.
	objects[treeCount + houseCount + rockCount + 3]->Initialize(11, texMountain.Get(), 1, glm::vec3(0, -5, 0), glm::vec3(0), glm::vec3(2));
	
	//// 街.	
	//progLighting.BindTexture(0, texCity.Get());
	//progLighting.Draw(meshList.Get(10), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));	

	//// 人間.	
	//progLighting.BindTexture(0, texHuman.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	//// ゾンビ.	
	//progLighting.BindTexture(0, texZombie.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(-7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));
		
	//// ツインテちゃん.	
	//progLighting.BindTexture(0, texTwinte.Get());
	//progLighting.Draw(meshList.Get(9), glm::vec3(9, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));
		

	// アイテムオブジェクトの初期化.
	items.resize(128);
	for (auto& item : items) {
		item = new Actor;
	}

	const float barrelCount = 6;		// 樽の数.
	const float barrelRadius = 16;		// 樽を置く円の半径.
	for (float i = 0; i < barrelCount; i++) {
		const float x = std::cos(3.14f * 2 / barrelCount * i) * barrelRadius;
		const float z = std::sin(3.14f * 2 / barrelCount * i) * barrelRadius;
		items[(size_t)i]->Initialize(5, texBarrel.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		items[(size_t)i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		items[(size_t)i]->Update(0);
	}

	const float leafCount = 8;		// 葉の数.
	const float leafRadius = 18;		// 葉を置く円の半径.
	for (float i = 0; i < leafCount; i++) {
		const float x = std::cos(3.14f * 2 / leafCount * i) * leafRadius;
		const float z = std::sin(3.14f * 2 / leafCount * i) * leafRadius;
		items[barrelCount + i]->Initialize(7, texLeaf.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		items[barrelCount + i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		items[barrelCount + i]->Update(0);
	}

	const float mushroomCount = 10;		// キノコの数.
	const float mushroomRadius = 20;		// キノコを置く円の半径.
	for (float i = 0; i < leafCount; i++) {
		const float x = std::cos(3.14f * 2 / mushroomCount * i) * mushroomRadius;
		const float z = std::sin(3.14f * 2 / mushroomCount * i) * mushroomRadius;
		items[barrelCount + mushroomCount + i]->Initialize(8, texMushroom.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		items[barrelCount + mushroomCount + i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		items[barrelCount + mushroomCount + i]->Update(0);
	}

	timer = 1.0f;
	isFinish = false;

	return true;
}

/**
* 入力の反映.
*/
void MainGameScene::ProcessInput() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)window.DeltaTime();

	if (!isFinish && timer <= 0.0f) {
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			isFinish = true;
			NextScene("TitleScene");
		}
	}

	//// 0番のポイント・ライトを移動する.
	//const float speed = 10.0f * deltaTime;
	//if (window.IsKeyPressed(GLFW_KEY_A)) {
	//	lights.point.position[0].x -= speed;
	//}
	//else if (window.IsKeyPressed(GLFW_KEY_D)) {
	//	lights.point.position[0].x += speed;
	//}
	//if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
	//	if (window.IsKeyPressed(GLFW_KEY_W)) {
	//		lights.point.position[0].y += speed;
	//	}
	//	else if (window.IsKeyPressed(GLFW_KEY_S)) {
	//		lights.point.position[0].y -= speed;
	//	}
	//}
	//else {
	//	if (window.IsKeyPressed(GLFW_KEY_W)) {
	//		lights.point.position[0].z -= speed;
	//	}
	//	else if (window.IsKeyPressed(GLFW_KEY_S)) {
	//		lights.point.position[0].z += speed;
	//	}
	//}

	if (state == State::play) {

		if (!isFinish && timer <= 0.0f) {
			if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
				timer = 1.0f;
				state = State::pause;
			}
		}

		// プレイヤーを移動する.
		player.velocity = glm::vec3(0);
		if (window.IsKeyPressed(GLFW_KEY_A)) {
			player.velocity.x = -1;
		}
		else if (window.IsKeyPressed(GLFW_KEY_D)) {
			player.velocity.x += 1;
		}
		if (window.IsKeyPressed(GLFW_KEY_W)) {
			player.velocity.z = -1;
		}
		else if (window.IsKeyPressed(GLFW_KEY_S)) {
			player.velocity.z = 1;
		}
		if (player.velocity.x || player.velocity.z) {
			player.velocity = glm::normalize(player.velocity);

			// ショットボタンが押されていなければ方向転換.
			if (!window.IsKeyPressed(GLFW_KEY_SPACE)) {
				player.rotation.y = std::atan2(-player.velocity.z, player.velocity.x);
				player.rotation.y -= glm::radians(90.0f);
			}
			const float speed = 10.0f;
			player.velocity *= speed;
		}

		// ショットボタンが押されていたら弾を発射.
		if (window.IsKeyPressed(GLFW_KEY_SPACE)) {

			if (playerBulletTimer <= 0) {
				Actor* bullet = FindAvailableActor(playerBullets);
				if (bullet) {
					const int meshId = 5;	// 弾のメッシュID.
					const float speed = 40.0f;	// 弾の移動速度(m/秒).
					const glm::mat4 matRotY = glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					bullet->Initialize(meshId, texBarrel.Get(), 1, player.position, player.rotation, glm::vec3(1));
					bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
					bullet->colLocal = { glm::vec3(-0.25f), glm::vec3(1) };
					playerBulletTimer = 1.0f / 8.0f;	// 秒間8連射.
				}
			}
		}
		else {
			playerBulletTimer = 0;
		}
	}
	else if (state == State::stageClear) {
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
			++stageNo;
			player.position = glm::vec3(8, 0, 8);
			state = State::play;

			// より多くの敵を、より早く出現させる.
			enemyTotal = 10 + stageNo * 10;
			enemyLeft = enemyTotal;
			enemyKilled = 0;
			enemyPoppingInterval = 15.0f - (float)(stageNo - 1) * 2.0f;
			if (enemyPoppingInterval < 5.0f) {
				enemyPoppingInterval = 5.0f;
			}
			enemyPoppingTimer = 0;

			// 敵の移動速度を上げる.
			enemySpeed = 1.0f + (float)(stageNo - 1) * 0.2f;
		}
	}
	else if (state == State::pause) {
		player.velocity.x = player.velocity.z = 0;
		if (!isFinish && timer <= 0.0f) {
			if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
				timer = 1.0f;
				state = State::play;
			}
		}
	}
	else {
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
			NextScene("TitleScene");
		}
	}
}


/**
* 状態の更新.
*/
void MainGameScene::Update() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)window.DeltaTime();

	if (timer > 0.0f) {
		timer -= deltaTime;
	}

	// モデルのy軸回転角を更新.
	static float angleY = 0;
	angleY += glm::radians(0.5f);
	if (angleY > glm::radians(360.0f)) {
		angleY -= glm::radians(360.0f);
	}

	// プレイヤーの状態を更新.
	player.Update(deltaTime);
	
	if (state == State::play) {		

		// プレイヤーの弾の状態を更新.
		UpdateActorList(playerBullets, deltaTime);
		// ゾンビの状態を更新.
		UpdateActorList(enemies, deltaTime);
		/*UpdateActorList(items, deltaTime);
		UpdateActorList(objects, deltaTime);*/

		// ゾンビの発生.
		if (enemyLeft > 0) {
			if (enemyPoppingTimer >= 0) {
				enemyPoppingTimer -= deltaTime;
			}
			else {
				enemyPoppingTimer += enemyPoppingInterval;

				int popCount = 10;	// 同時に発生するゾンビの最大数.
				if (enemyLeft < popCount) {
					popCount = enemyLeft;
				}
				enemyLeft -= popCount;

				// マップの前後左右15mの範囲で出現ポイントを決め、そこを中心として前後左右5mの範囲に出現.
				std::uniform_int_distribution<int> rangeBase(-15, 15);
				std::uniform_int_distribution<int> range(-5, 5);

				glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
				for (int i = 0; i < popCount; ++i) {
					ZombieActor* zombie = (ZombieActor*)FindAvailableActor(enemies);
					if (zombie) {
						glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
						zombie->Initialize(4, texZombie.Get(), 5, pos, glm::vec3(0), glm::vec3(1));
						zombie->colLocal = { glm::vec3(-0.5f,0,-0.5f), glm::vec3(1.0f,1.7f,1.0f) };
						zombie->target = &player;
						zombie->baseSpeed = enemySpeed;
					}
					else {
						++enemyLeft;	// 出現できなかった場合、残りの数を増やして次回の出現を待つ.
					}
				}
			}
		}

		// 静止オブジェクトの更新
		for (auto& object : objects) {
			if (object->health > 0) {
				if (DetectCollision(player, *object)) {
					const CollisionTime t = FindCollisionTime(*object, player, deltaTime);
					if (t.plane != CollisionPlane::none) {
						const float time = deltaTime * t.time - 0.00001f;
						player.position += player.velocity * time;
						if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX) {
							player.velocity.x = 0;
						}
						else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY) {
							player.velocity.y = 0;
						}
						else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ) {
							player.velocity.z = 0;
						}
						player.velocity *= -t.time;
						player.position += player.velocity * -time;
					}
				}
				// プレイヤーの弾とオブジェクトの衝突判定.
				DetectCollision(playerBullets, objects, [&](Actor& bullet, Actor& object) {
					bullet.health = 0;
				});
				// ゾンビとオブジェクトの衝突判定.
				DetectCollision(enemies, objects, [&](Actor& enemy, Actor& object) {
					const CollisionTime t = FindCollisionTime(object, enemy, deltaTime);
					if (t.plane != CollisionPlane::none) {
						const float time = deltaTime * t.time - 0.00001f;
						enemy.position += enemy.velocity * time;
						if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX) {
							enemy.velocity.x = 0;
						}
						else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY) {
							enemy.velocity.y = 0;
						}
						else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ) {
							enemy.velocity.z = 0;
						}
						enemy.velocity *= -t.time;
						enemy.position += enemy.velocity * -time;
					}
				});
			}
		}

		// プレイヤーの弾と敵の衝突判定.
		DetectCollision(playerBullets, enemies, [&](Actor& bullet, Actor& zombie) {
			zombie.health -= bullet.health;
			bullet.health = 0;
			if (zombie.health <= 0) {
				score += 200;
				++enemyKilled;
			}
			else {
				score += 10;
			}
		});

		// ゾンビとゾンビの衝突判定.
		DetectCollision(enemies, enemies, [&](Actor& enemy1, Actor& enemy2) {
			const CollisionTime t = FindCollisionTime(enemy1, enemy2, deltaTime);
			if (t.plane != CollisionPlane::none) {
				const float time = deltaTime * t.time - 0.00001f;
				enemy1.position += enemy1.velocity * time;
				if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX) {
					enemy1.velocity.x = 0;
					enemy2.velocity.x = 0;
				}
				else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY) {
					enemy1.velocity.y = 0;
					enemy2.velocity.y = 0;
				}
				else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ) {
					enemy1.velocity.z = 0;
					enemy2.velocity.z = 0;
				}
				enemy1.velocity *= -t.time;
				enemy1.position += enemy1.velocity * -time;
				enemy2.velocity *= -t.time;
				enemy2.position += enemy2.velocity * -time;
			}
		});

		// ゾンビとプレイヤーの衝突判定.
		for (auto& enemy : enemies) {
			if (enemy->health > 0) {
				if (DetectCollision(player, *enemy)) {
					const CollisionTime t = FindCollisionTime(*enemy, player, deltaTime);
					if (t.plane != CollisionPlane::none) {
						const float time = deltaTime * t.time - 0.00001f;
						player.position += player.velocity * time;
						if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX) {
							player.velocity.x = 0;
						}
						else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY) {
							player.velocity.y = 0;
						}
						else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ) {
							player.velocity.z = 0;
						}
						player.velocity *= -t.time;
						player.position += player.velocity * -time;
					}
				}
			}
		}

		// ステージクリア判定.
		if (/*state == State::play &&*/ enemyKilled >= enemyTotal) {
			state = State::stageClear;
		}

		// ゾンビの攻撃.
		for (auto& actor : enemies) {
			if (actor->health <= 0) {
				continue;
			}

			// ターゲットの体力が0以下なら攻撃しない.
			ZombieActor* zombie = (ZombieActor*)actor;
			if (zombie->target->health <= 0) {
				continue;
			}

			if (zombie->isAttacking) {
				// ゾンビの正面左右45度の範囲にターゲットがいたら、ターゲットの体力を1減らす.
				const glm::vec3 vFront = glm::rotate(glm::mat4(1), zombie->rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
				const glm::vec3 vTarget = zombie->target->position - zombie->position;
				const float angle = std::acos(glm::dot(vFront, glm::normalize(vTarget)));
				if (std::abs(angle) < glm::radians(45.0f) && glm::length(vTarget) < 1.5f) {
					--zombie->target->health;
				}
			}
		}

		// ゲームオーバー判定.
		if (/*state == State::play &&*/ player.health <= 0) {
			state = State::gameOver;
		}

		// プレイヤーの弾の発射タイマーを更新.
		if (playerBulletTimer > 0) {
			playerBulletTimer -= deltaTime;
		}

		// 視点の位置の更新.
		const glm::vec3 viewOffset(0, 20, 10);
		viewPos = player.position + viewOffset;
	}
		
}

/**
* 描画.
*/
void MainGameScene::Render() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)window.DeltaTime();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//// 視点を回転移動させる.
	//static float degree = 0.0f;
	//degree += 0.01f;
	//if (degree >= 360.0f) { degree -= 360.0f; }
	//const glm::vec3 viewPos = glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(0, 1, 0)) * glm::vec4(-20, 20, -30, 1);
	//// 視点を設定する.
	//const glm::vec3 viewPos(20, 30, 30);

	progLighting.Use();

	// 座標変換行列を作成してシェーダーに転送する.
	const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
	//const glm::mat4x4 matView = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	const glm::mat4x4 matView = glm::lookAt(viewPos, player.position, glm::vec3(0, 1, 0));
	progLighting.SetViewProjectionMatrix(matProj * matView);

	meshList.BindVertexArray();

	// 光源を設定する.
	progLighting.SetLightList(lights);

	// プレイヤー
	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList.Get(player.mesh), player.position, player.rotation, player.scale);
	// 弾
	RenderActorList(playerBullets, progLighting, meshList);
	// ゾンビ
	RenderActorList(enemies, progLighting, meshList);
	// 静止オブジェクト
	RenderActorList(objects, progLighting, meshList);
	// アイテム
	RenderActorList(items, progLighting, meshList);

	//progLighting.BindTexture(0, texTree.Get());
	//const float treeCount = 10;	// 木の本数.
	//const float radius = 8;		// 木を植える円の半径.
	//for (float i = 0; i < treeCount; ++i) {
	//	const float theta = 3.14f * 2 / treeCount * i;
	//	const float x = std::cos(theta) * radius;
	//	const float z = std::sin(theta) * radius;

	//	progLighting.Draw(meshList.Get(0), glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
	//}

	//progLighting.BindTexture(0, texHouse.Get());
	//const float houseCount = 3;		// 家の数.
	//const float houseRadius = 14;	// 家を置く円の半径.
	//for (float i = 0; i < houseCount; i++) {
	//	if (i == 0) {
	//		progLighting.Draw(meshList.Get(1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));
	//	}
	//	else {
	//		const float x = std::cos(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
	//		const float z = std::sin(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
	//		progLighting.Draw(meshList.Get(1), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//	}
	//}

	//progLighting.BindTexture(0, texRock.Get());
	//const float rockCount = 4;		// 岩の数.
	//const float rockRadius = 5;		// 岩を置く円の半径.
	//for (float i = 0; i < rockCount; i++) {
	//	const float x = std::cos(3.14f * 2 / rockCount * i) * rockRadius;
	//	const float z = std::sin(3.14f * 2 / rockCount * i) * rockRadius;
	//	progLighting.Draw(meshList.Get(2), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//// 地面.	
	//progLighting.BindTexture(0, texGround.Get());
	//progLighting.Draw(meshList.Get(3), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));

	//// 人間.	
	//progLighting.BindTexture(0, texHuman.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	//// ゾンビ.	
	//progLighting.BindTexture(0, texZombie.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(-7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	//progLighting.BindTexture(0, texBarrel.Get());
	//const float barrelCount = 6;		// 樽の数.
	//const float barrelRadius = 16;		// 樽を置く円の半径.
	//for (float i = 0; i < barrelCount; i++) {
	//	const float x = std::cos(3.14f * 2 / barrelCount * i) * barrelRadius;
	//	const float z = std::sin(3.14f * 2 / barrelCount * i) * barrelRadius;
	//	progLighting.Draw(meshList.Get(5), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//// 二世帯住宅.	
	//progLighting.BindTexture(0, texHome.Get());
	//progLighting.Draw(meshList.Get(6), glm::vec3(20, 0, -17), glm::vec3(0, 0, 0), glm::vec3(1));

	//progLighting.BindTexture(0, texLeaf.Get());
	//const float leafCount = 8;		// 葉の数.
	//const float leafRadius = 18;		// 葉を置く円の半径.
	//for (float i = 0; i < leafCount; i++) {
	//	const float x = std::cos(3.14f * 2 / leafCount * i) * leafRadius;
	//	const float z = std::sin(3.14f * 2 / leafCount * i) * leafRadius;
	//	progLighting.Draw(meshList.Get(7), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//progLighting.BindTexture(0, texMushroom.Get());
	//const float mushroomCount = 10;		// キノコの数.
	//const float mushroomRadius = 20;		// キノコを置く円の半径.
	//for (float i = 0; i < leafCount; i++) {
	//	const float x = std::cos(3.14f * 2 / mushroomCount * i) * mushroomRadius;
	//	const float z = std::sin(3.14f * 2 / mushroomCount * i) * mushroomRadius;
	//	progLighting.Draw(meshList.Get(8), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//// ツインテちゃん.	
	//progLighting.BindTexture(0, texTwinte.Get());
	//progLighting.Draw(meshList.Get(9), glm::vec3(9, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	////// 街.	
	////progLighting.BindTexture(0, texCity.Get());
	////progLighting.Draw(meshList.Get(10), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));

	//// 山.	
	//progLighting.BindTexture(0, texMountain.Get());
	//progLighting.Draw(meshList.Get(11), glm::vec3(-60, -5, -50), glm::vec3(0, 0, 0), glm::vec3(1));

	// ポイント・ライトの位置がわかるように適当なモデルを表示.
	{
		// ライトの位置に木のモデルを表示.		
		progLighting.BindTexture(0, texTree.Get());
		for (int i = 0; i < 8; ++i) {
			progLighting.Draw(meshList.Get(0), lights.point.position[i], glm::vec3(0, angleY, 0), glm::vec3(0.5f, -0.5f, 0.5f));
		}
	}

	// 情報を表示.
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		progSimple.Use();

		// 正射影で描画するように行列を設定.
		const glm::mat4x4 matProj = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, 1.0f, 500.0f);
		const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		progSimple.SetViewProjectionMatrix(matProj * matView);

		const Mesh& planeMeshId = meshList.Get(12);

		// スコアを表示.
		progSimple.BindTexture(0, texScore.Get());
		progSimple.Draw(planeMeshId, glm::vec3(-60, 270, 0), glm::vec3(0), glm::vec3(96, 32, 1));

		const int maxScoreDigits = 8;	// 表示する桁数.
		int tmpScore = score;
		for (int i = 0; i < maxScoreDigits; ++i) {
			const int posX = 28 + 32 * (maxScoreDigits - i);
			const int number = tmpScore % 10;
			tmpScore /= 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(planeMeshId, glm::vec3(posX, 270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
		}

		// プレイヤーの体力を表示.
		progSimple.BindTexture(0, texHP.Get());
		progSimple.Draw(planeMeshId, glm::vec3(-336, -270, 0), glm::vec3(0), glm::vec3(48, 32, 1));

		const int maxHealthDigits = 2;	// 表示する桁数.
		int tmpHealth = player.health;
		for (int i = 0; i < maxHealthDigits; ++i) {
			const int posX = -300 + 32 * (maxHealthDigits - i);
			const int number = tmpHealth % 10;
			tmpHealth /= 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(planeMeshId, glm::vec3(posX, -270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
		}

		// ステージ番号を表示.
		progSimple.BindTexture(0, texDay.Get());
		progSimple.Draw(planeMeshId, glm::vec3(-328, 270, 0), glm::vec3(0), glm::vec3(64, 32, 1));

		const int maxDayDigits = 2;	// 表示する桁数.
		int tmpDay = stageNo;
		for (int i = 0; i < maxDayDigits; ++i) {
			const int posX = -280 + 32 * (maxDayDigits - i);
			const int number = tmpDay % 10;
			tmpDay /= 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(planeMeshId, glm::vec3(posX, 270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
		}

		// ステージクリア・ゲームオーバー・一時停止表示.
		if (state == State::stageClear) {
			progSimple.BindTexture(0, texStageClear.Get());
			progSimple.Draw(planeMeshId, glm::vec3(0), glm::vec3(0), glm::vec3(350, 60, 1));
		}else if (state == State::gameOver) {
			progSimple.BindTexture(0, texGameOver.Get());
			progSimple.Draw(planeMeshId, glm::vec3(0), glm::vec3(0), glm::vec3(300, 60, 1));
		}
		else if (state == State::pause) {
			progSimple.BindTexture(0, texPause.Get());
			progSimple.Draw(planeMeshId, glm::vec3(0), glm::vec3(0), glm::vec3(260, 170, 1));
		}
	}
	
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}

/**
* 終了.
*/
void MainGameScene::Finalize() {
	ClearActorList(playerBullets);
	ClearActorList(enemies);
}

/**
* ゲーム画面が終わったかどうか調べる.
*
* @retval true	終わった.
* @retval false	終わっていない.
*/
bool MainGameScene::IsFinish() const {
	return isFinish;
}