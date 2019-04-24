/**
* @file MainGameScene.cpp
*/

#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* �v���C���[�̏�Ԃ��X�V����.
*
* @param deltaTime	�o�ߎ���.
*/
void PlayerActor::Update(float deltaTime) {
	// �v���C���[������ł�����A�n�ʂɓ|���(-90�x�ɂȂ�)�܂Łu�|���v��傫�����Ă���.
	if (health <= 0) {
		rotation.x -= glm::radians(45.0f) * deltaTime;
		if (rotation.x < glm::radians(-90.0f)) {
			rotation.x = glm::radians(-90.0f);
		}
	}
	Actor::Update(deltaTime);
}

/**
* �v���C���[�̒e�̏�Ԃ��X�V����.
*
* @param deltaTime	�o�ߎ���.
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
* �]���r�̏�Ԃ��X�V����.
*
* @param deltaTime	�o�ߎ���.
*/
void ZombieActor::Update(float deltaTime) {

	if (!target) {
		return;
	}

	const float moveSpeed = baseSpeed * 2.0f;
	const float rotationSpeed = baseSpeed * glm::radians(60.0f);
	const float frontRange = glm::radians(15.0f);

	// �^�[�Q�b�g�i�v���C���[�j�̕����𒲂ׂ�.
	const glm::vec3 v = target->position - position;
	const glm::vec3 vTarget = glm::normalize(v);
	float radian = std::atan2(-vTarget.z, vTarget.x) - glm::radians(90.0f);
	if (radian <= 0) {
		radian += glm::radians(360.0f);
	}
	const glm::vec3 vZombieFront = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

	// �^�[�Q�b�g�����ʂɂ��Ȃ�������A���ʂ𑨂���悤�ɍ��E�ɐ���.
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

	// �\���ɐڋ߂��Ă��Ȃ���Έړ�����.�ڋ߂��Ă���΍U������.
	if (glm::length(v) > 1.0f) {
		velocity = vZombieFront * moveSpeed;
	}
	else {
		velocity = glm::vec3(0);	// �ڋ߂��Ă���̂ł����ړ����Ȃ�.
		// ����I�ɍU����ԂɂȂ�.
		if (isAttacking) {
			isAttacking = false;
			attackingTimer = 1.0f;	// ���̍U����1�b��.
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
* ������.
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

	// �e�N�X�`�����쐬����.
	const int imageWidth = 8;			// �摜�̕�.
	const int imageHeight = 8;			// �摜�̍���.
	const uint32_t B = 0xff'00'00'00;	// ��.
	const uint32_t W = 0xff'ff'ff'ff;	// ��.
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

	// ���\���p�e�N�X�`����ǂݍ���.
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

	// ���C�g�̐ݒ�.
	lights.ambient.color = glm::vec3(0.05f, 0.1f, 0.1f);			// �����̐F.
	lights.directional.direction = glm::normalize(glm::vec3(5, -2, -2));	// �w�������C�g�̕���.
	lights.directional.color = glm::vec3(1.0f, 1.0f, 1.0f);			// �w�������C�g�̐F.
	lights.point.position[0] = glm::vec3(5, 4, 0);
	lights.point.color[0] = glm::vec3(1.0f, 0.8f, 0.4f) * 100.0f;
	lights.spot.dirAndCutOff[0] = glm::vec4(glm::normalize(glm::vec3(-1, -2, -2)), std::cos(glm::radians(20.0f)));
	lights.spot.posAndInnerCutOff[0] = glm::vec4(-6, 6, 8, std::cos(glm::radians(15.0f)));
	lights.spot.color[0] = glm::vec3(0.4f, 0.8f, 1.0f) * 200.0f;

	// �v���C���[�A�N�^�[�̒ǉ�.
	player.Initialize(4, texHuman.Get(), 10, glm::vec3(8, 0, 8), glm::vec3(0), glm::vec3(1));
	player.colLocal = { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f, 1.7f, 1.0f) };

	// �e�A�N�^�[�̏�����.
	playerBullets.resize(128);
	for (auto& bullet : playerBullets) {
		bullet = new BulletActor;
	}

	// �G�A�N�^�[�̏�����.
	enemies.resize(128);
	for (auto& zombie : enemies) {
		zombie = new ZombieActor;
	}

	// �Î~�I�u�W�F�N�g�̏�����.
	objects.resize(128);
	for (auto& e : objects) {
		e = new Actor;
	}

	const float treeCount = 10;	// �؂̖{��.
	const float radius = 8;		// �؂�A����~�̔��a.
	for (float i = 0; i < treeCount; ++i) {
		const float theta = 3.14f * 2 / treeCount * i;
		const float x = std::cos(theta) * radius;
		const float z = std::sin(theta) * radius;
		objects[(size_t)i]->Initialize(0, texTree.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
		objects[(size_t)i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 4, 1 } };
		objects[(size_t)i]->Update(0);
	}

	const float houseCount = 3;		// �Ƃ̐�.
	const float houseRadius = 14;	// �Ƃ�u���~�̔��a.
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

	const float rockCount = 4;		// ��̐�.
	const float rockRadius = 5;		// ���u���~�̔��a.
	for (float i = 0; i < rockCount; i++) {
		const float x = std::cos(3.14f * 2 / rockCount * i) * rockRadius;
		const float z = std::sin(3.14f * 2 / rockCount * i) * rockRadius;
		objects[treeCount + houseCount + i]->Initialize(2, texRock.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		objects[treeCount + houseCount + i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		objects[treeCount + houseCount + i]->Update(0);
	}

	// �n��.	
	objects[treeCount + houseCount + rockCount + 1]->Initialize(3, texGround.Get(), 1, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1));

	// �񐢑яZ��.
	objects[treeCount + houseCount + rockCount + 2]->Initialize(6, texHome.Get(), 1, glm::vec3(20, 0, -17), glm::vec3(0), glm::vec3(1));
	
	// �R.
	objects[treeCount + houseCount + rockCount + 3]->Initialize(11, texMountain.Get(), 1, glm::vec3(0, -5, 0), glm::vec3(0), glm::vec3(2));
	
	//// �X.	
	//progLighting.BindTexture(0, texCity.Get());
	//progLighting.Draw(meshList.Get(10), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));	

	//// �l��.	
	//progLighting.BindTexture(0, texHuman.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	//// �]���r.	
	//progLighting.BindTexture(0, texZombie.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(-7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));
		
	//// �c�C���e�����.	
	//progLighting.BindTexture(0, texTwinte.Get());
	//progLighting.Draw(meshList.Get(9), glm::vec3(9, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));
		

	// �A�C�e���I�u�W�F�N�g�̏�����.
	items.resize(128);
	for (auto& item : items) {
		item = new Actor;
	}

	const float barrelCount = 6;		// �M�̐�.
	const float barrelRadius = 16;		// �M��u���~�̔��a.
	for (float i = 0; i < barrelCount; i++) {
		const float x = std::cos(3.14f * 2 / barrelCount * i) * barrelRadius;
		const float z = std::sin(3.14f * 2 / barrelCount * i) * barrelRadius;
		items[(size_t)i]->Initialize(5, texBarrel.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		items[(size_t)i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		items[(size_t)i]->Update(0);
	}

	const float leafCount = 8;		// �t�̐�.
	const float leafRadius = 18;		// �t��u���~�̔��a.
	for (float i = 0; i < leafCount; i++) {
		const float x = std::cos(3.14f * 2 / leafCount * i) * leafRadius;
		const float z = std::sin(3.14f * 2 / leafCount * i) * leafRadius;
		items[barrelCount + i]->Initialize(7, texLeaf.Get(), 1, glm::vec3(x, 0, z), glm::vec3(0), glm::vec3(1));
		items[barrelCount + i]->colLocal = { {-0.5f, 0, -0.5f }, { 1, 1, 1 } };
		items[barrelCount + i]->Update(0);
	}

	const float mushroomCount = 10;		// �L�m�R�̐�.
	const float mushroomRadius = 20;		// �L�m�R��u���~�̔��a.
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
* ���͂̔��f.
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

	//// 0�Ԃ̃|�C���g�E���C�g���ړ�����.
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

		// �v���C���[���ړ�����.
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

			// �V���b�g�{�^����������Ă��Ȃ���Ε����]��.
			if (!window.IsKeyPressed(GLFW_KEY_SPACE)) {
				player.rotation.y = std::atan2(-player.velocity.z, player.velocity.x);
				player.rotation.y -= glm::radians(90.0f);
			}
			const float speed = 10.0f;
			player.velocity *= speed;
		}

		// �V���b�g�{�^����������Ă�����e�𔭎�.
		if (window.IsKeyPressed(GLFW_KEY_SPACE)) {

			if (playerBulletTimer <= 0) {
				Actor* bullet = FindAvailableActor(playerBullets);
				if (bullet) {
					const int meshId = 5;	// �e�̃��b�V��ID.
					const float speed = 40.0f;	// �e�̈ړ����x(m/�b).
					const glm::mat4 matRotY = glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					bullet->Initialize(meshId, texBarrel.Get(), 1, player.position, player.rotation, glm::vec3(1));
					bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
					bullet->colLocal = { glm::vec3(-0.25f), glm::vec3(1) };
					playerBulletTimer = 1.0f / 8.0f;	// �b��8�A��.
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

			// ��葽���̓G���A��葁���o��������.
			enemyTotal = 10 + stageNo * 10;
			enemyLeft = enemyTotal;
			enemyKilled = 0;
			enemyPoppingInterval = 15.0f - (float)(stageNo - 1) * 2.0f;
			if (enemyPoppingInterval < 5.0f) {
				enemyPoppingInterval = 5.0f;
			}
			enemyPoppingTimer = 0;

			// �G�̈ړ����x���グ��.
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
* ��Ԃ̍X�V.
*/
void MainGameScene::Update() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)window.DeltaTime();

	if (timer > 0.0f) {
		timer -= deltaTime;
	}

	// ���f����y����]�p���X�V.
	static float angleY = 0;
	angleY += glm::radians(0.5f);
	if (angleY > glm::radians(360.0f)) {
		angleY -= glm::radians(360.0f);
	}

	// �v���C���[�̏�Ԃ��X�V.
	player.Update(deltaTime);
	
	if (state == State::play) {		

		// �v���C���[�̒e�̏�Ԃ��X�V.
		UpdateActorList(playerBullets, deltaTime);
		// �]���r�̏�Ԃ��X�V.
		UpdateActorList(enemies, deltaTime);
		/*UpdateActorList(items, deltaTime);
		UpdateActorList(objects, deltaTime);*/

		// �]���r�̔���.
		if (enemyLeft > 0) {
			if (enemyPoppingTimer >= 0) {
				enemyPoppingTimer -= deltaTime;
			}
			else {
				enemyPoppingTimer += enemyPoppingInterval;

				int popCount = 10;	// �����ɔ�������]���r�̍ő吔.
				if (enemyLeft < popCount) {
					popCount = enemyLeft;
				}
				enemyLeft -= popCount;

				// �}�b�v�̑O�㍶�E15m�͈̔͂ŏo���|�C���g�����߁A�����𒆐S�Ƃ��đO�㍶�E5m�͈̔͂ɏo��.
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
						++enemyLeft;	// �o���ł��Ȃ������ꍇ�A�c��̐��𑝂₵�Ď���̏o����҂�.
					}
				}
			}
		}

		// �Î~�I�u�W�F�N�g�̍X�V
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
				// �v���C���[�̒e�ƃI�u�W�F�N�g�̏Փ˔���.
				DetectCollision(playerBullets, objects, [&](Actor& bullet, Actor& object) {
					bullet.health = 0;
				});
				// �]���r�ƃI�u�W�F�N�g�̏Փ˔���.
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

		// �v���C���[�̒e�ƓG�̏Փ˔���.
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

		// �]���r�ƃ]���r�̏Փ˔���.
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

		// �]���r�ƃv���C���[�̏Փ˔���.
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

		// �X�e�[�W�N���A����.
		if (/*state == State::play &&*/ enemyKilled >= enemyTotal) {
			state = State::stageClear;
		}

		// �]���r�̍U��.
		for (auto& actor : enemies) {
			if (actor->health <= 0) {
				continue;
			}

			// �^�[�Q�b�g�̗̑͂�0�ȉ��Ȃ�U�����Ȃ�.
			ZombieActor* zombie = (ZombieActor*)actor;
			if (zombie->target->health <= 0) {
				continue;
			}

			if (zombie->isAttacking) {
				// �]���r�̐��ʍ��E45�x�͈̔͂Ƀ^�[�Q�b�g��������A�^�[�Q�b�g�̗̑͂�1���炷.
				const glm::vec3 vFront = glm::rotate(glm::mat4(1), zombie->rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
				const glm::vec3 vTarget = zombie->target->position - zombie->position;
				const float angle = std::acos(glm::dot(vFront, glm::normalize(vTarget)));
				if (std::abs(angle) < glm::radians(45.0f) && glm::length(vTarget) < 1.5f) {
					--zombie->target->health;
				}
			}
		}

		// �Q�[���I�[�o�[����.
		if (/*state == State::play &&*/ player.health <= 0) {
			state = State::gameOver;
		}

		// �v���C���[�̒e�̔��˃^�C�}�[���X�V.
		if (playerBulletTimer > 0) {
			playerBulletTimer -= deltaTime;
		}

		// ���_�̈ʒu�̍X�V.
		const glm::vec3 viewOffset(0, 20, 10);
		viewPos = player.position + viewOffset;
	}
		
}

/**
* �`��.
*/
void MainGameScene::Render() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)window.DeltaTime();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//// ���_����]�ړ�������.
	//static float degree = 0.0f;
	//degree += 0.01f;
	//if (degree >= 360.0f) { degree -= 360.0f; }
	//const glm::vec3 viewPos = glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(0, 1, 0)) * glm::vec4(-20, 20, -30, 1);
	//// ���_��ݒ肷��.
	//const glm::vec3 viewPos(20, 30, 30);

	progLighting.Use();

	// ���W�ϊ��s����쐬���ăV�F�[�_�[�ɓ]������.
	const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
	//const glm::mat4x4 matView = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	const glm::mat4x4 matView = glm::lookAt(viewPos, player.position, glm::vec3(0, 1, 0));
	progLighting.SetViewProjectionMatrix(matProj * matView);

	meshList.BindVertexArray();

	// ������ݒ肷��.
	progLighting.SetLightList(lights);

	// �v���C���[
	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList.Get(player.mesh), player.position, player.rotation, player.scale);
	// �e
	RenderActorList(playerBullets, progLighting, meshList);
	// �]���r
	RenderActorList(enemies, progLighting, meshList);
	// �Î~�I�u�W�F�N�g
	RenderActorList(objects, progLighting, meshList);
	// �A�C�e��
	RenderActorList(items, progLighting, meshList);

	//progLighting.BindTexture(0, texTree.Get());
	//const float treeCount = 10;	// �؂̖{��.
	//const float radius = 8;		// �؂�A����~�̔��a.
	//for (float i = 0; i < treeCount; ++i) {
	//	const float theta = 3.14f * 2 / treeCount * i;
	//	const float x = std::cos(theta) * radius;
	//	const float z = std::sin(theta) * radius;

	//	progLighting.Draw(meshList.Get(0), glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
	//}

	//progLighting.BindTexture(0, texHouse.Get());
	//const float houseCount = 3;		// �Ƃ̐�.
	//const float houseRadius = 14;	// �Ƃ�u���~�̔��a.
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
	//const float rockCount = 4;		// ��̐�.
	//const float rockRadius = 5;		// ���u���~�̔��a.
	//for (float i = 0; i < rockCount; i++) {
	//	const float x = std::cos(3.14f * 2 / rockCount * i) * rockRadius;
	//	const float z = std::sin(3.14f * 2 / rockCount * i) * rockRadius;
	//	progLighting.Draw(meshList.Get(2), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//// �n��.	
	//progLighting.BindTexture(0, texGround.Get());
	//progLighting.Draw(meshList.Get(3), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));

	//// �l��.	
	//progLighting.BindTexture(0, texHuman.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	//// �]���r.	
	//progLighting.BindTexture(0, texZombie.Get());
	//progLighting.Draw(meshList.Get(4), glm::vec3(-7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	//progLighting.BindTexture(0, texBarrel.Get());
	//const float barrelCount = 6;		// �M�̐�.
	//const float barrelRadius = 16;		// �M��u���~�̔��a.
	//for (float i = 0; i < barrelCount; i++) {
	//	const float x = std::cos(3.14f * 2 / barrelCount * i) * barrelRadius;
	//	const float z = std::sin(3.14f * 2 / barrelCount * i) * barrelRadius;
	//	progLighting.Draw(meshList.Get(5), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//// �񐢑яZ��.	
	//progLighting.BindTexture(0, texHome.Get());
	//progLighting.Draw(meshList.Get(6), glm::vec3(20, 0, -17), glm::vec3(0, 0, 0), glm::vec3(1));

	//progLighting.BindTexture(0, texLeaf.Get());
	//const float leafCount = 8;		// �t�̐�.
	//const float leafRadius = 18;		// �t��u���~�̔��a.
	//for (float i = 0; i < leafCount; i++) {
	//	const float x = std::cos(3.14f * 2 / leafCount * i) * leafRadius;
	//	const float z = std::sin(3.14f * 2 / leafCount * i) * leafRadius;
	//	progLighting.Draw(meshList.Get(7), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//progLighting.BindTexture(0, texMushroom.Get());
	//const float mushroomCount = 10;		// �L�m�R�̐�.
	//const float mushroomRadius = 20;		// �L�m�R��u���~�̔��a.
	//for (float i = 0; i < leafCount; i++) {
	//	const float x = std::cos(3.14f * 2 / mushroomCount * i) * mushroomRadius;
	//	const float z = std::sin(3.14f * 2 / mushroomCount * i) * mushroomRadius;
	//	progLighting.Draw(meshList.Get(8), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	//}

	//// �c�C���e�����.	
	//progLighting.BindTexture(0, texTwinte.Get());
	//progLighting.Draw(meshList.Get(9), glm::vec3(9, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	////// �X.	
	////progLighting.BindTexture(0, texCity.Get());
	////progLighting.Draw(meshList.Get(10), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));

	//// �R.	
	//progLighting.BindTexture(0, texMountain.Get());
	//progLighting.Draw(meshList.Get(11), glm::vec3(-60, -5, -50), glm::vec3(0, 0, 0), glm::vec3(1));

	// �|�C���g�E���C�g�̈ʒu���킩��悤�ɓK���ȃ��f����\��.
	{
		// ���C�g�̈ʒu�ɖ؂̃��f����\��.		
		progLighting.BindTexture(0, texTree.Get());
		for (int i = 0; i < 8; ++i) {
			progLighting.Draw(meshList.Get(0), lights.point.position[i], glm::vec3(0, angleY, 0), glm::vec3(0.5f, -0.5f, 0.5f));
		}
	}

	// ����\��.
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		progSimple.Use();

		// ���ˉe�ŕ`�悷��悤�ɍs���ݒ�.
		const glm::mat4x4 matProj = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, 1.0f, 500.0f);
		const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		progSimple.SetViewProjectionMatrix(matProj * matView);

		const Mesh& planeMeshId = meshList.Get(12);

		// �X�R�A��\��.
		progSimple.BindTexture(0, texScore.Get());
		progSimple.Draw(planeMeshId, glm::vec3(-60, 270, 0), glm::vec3(0), glm::vec3(96, 32, 1));

		const int maxScoreDigits = 8;	// �\�����錅��.
		int tmpScore = score;
		for (int i = 0; i < maxScoreDigits; ++i) {
			const int posX = 28 + 32 * (maxScoreDigits - i);
			const int number = tmpScore % 10;
			tmpScore /= 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(planeMeshId, glm::vec3(posX, 270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
		}

		// �v���C���[�̗̑͂�\��.
		progSimple.BindTexture(0, texHP.Get());
		progSimple.Draw(planeMeshId, glm::vec3(-336, -270, 0), glm::vec3(0), glm::vec3(48, 32, 1));

		const int maxHealthDigits = 2;	// �\�����錅��.
		int tmpHealth = player.health;
		for (int i = 0; i < maxHealthDigits; ++i) {
			const int posX = -300 + 32 * (maxHealthDigits - i);
			const int number = tmpHealth % 10;
			tmpHealth /= 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(planeMeshId, glm::vec3(posX, -270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
		}

		// �X�e�[�W�ԍ���\��.
		progSimple.BindTexture(0, texDay.Get());
		progSimple.Draw(planeMeshId, glm::vec3(-328, 270, 0), glm::vec3(0), glm::vec3(64, 32, 1));

		const int maxDayDigits = 2;	// �\�����錅��.
		int tmpDay = stageNo;
		for (int i = 0; i < maxDayDigits; ++i) {
			const int posX = -280 + 32 * (maxDayDigits - i);
			const int number = tmpDay % 10;
			tmpDay /= 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(planeMeshId, glm::vec3(posX, 270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
		}

		// �X�e�[�W�N���A�E�Q�[���I�[�o�[�E�ꎞ��~�\��.
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
* �I��.
*/
void MainGameScene::Finalize() {
	ClearActorList(playerBullets);
	ClearActorList(enemies);
}

/**
* �Q�[����ʂ��I��������ǂ������ׂ�.
*
* @retval true	�I�����.
* @retval false	�I����Ă��Ȃ�.
*/
bool MainGameScene::IsFinish() const {
	return isFinish;
}