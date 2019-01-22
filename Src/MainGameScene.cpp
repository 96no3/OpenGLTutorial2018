/**
* @file MainGameScene.cpp
*/

#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* ������.
*/
bool MainGameScene::Initialize() {

	if (!meshList.Allocate()) {
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
	texRock.Reset(Texture::LoadImage2D("Res/Model/Rock2.tga"));
	texGround.Reset(Texture::LoadImage2D("Res/Model/Ground.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/Model/human.tga"));
	texBarrel.Reset(Texture::LoadImage2D("Res/Model/Barrel.tga"));
	texHome.Reset(Texture::LoadImage2D("Res/Model/home.tga"));
	texLeaf.Reset(Texture::LoadImage2D("Res/Model/leaf.tga"));
	texMushroom.Reset(Texture::LoadImage2D("Res/Model/mushroom.tga"));

	// ���C�g�̐ݒ�.
	lights.ambient.color = glm::vec3(0.05f, 0.1f, 0.1f);			// �����̐F.
	lights.directional.direction = glm::normalize(glm::vec3(5, -2, -2));	// �w�������C�g�̕���.
	lights.directional.color = glm::vec3(1.0f, 1.0f, 1.0f);			// �w�������C�g�̐F.
	lights.point.position[0] = glm::vec3(5, 4, 0);
	lights.point.color[0] = glm::vec3(1.0f, 0.8f, 0.4f) * 100.0f;
	lights.spot.dirAndCutOff[0] = glm::vec4(glm::normalize(glm::vec3(-1, -2, -2)), std::cos(glm::radians(20.0f)));
	lights.spot.posAndInnerCutOff[0] = glm::vec4(-6, 6, 8, std::cos(glm::radians(15.0f)));
	lights.spot.color[0] = glm::vec3(0.4f, 0.8f, 1.0f) * 200.0f;

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
		if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
			isFinish = true;
			NextScene("TitleScene");
		}
	}

	// 0�Ԃ̃|�C���g�E���C�g���ړ�����.
	const float speed = 10.0f * deltaTime;
	if (window.IsKeyPressed(GLFW_KEY_A)) {
		lights.point.position[0].x -= speed;
	}
	else if (window.IsKeyPressed(GLFW_KEY_D)) {
		lights.point.position[0].x += speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		if (window.IsKeyPressed(GLFW_KEY_W)) {
			lights.point.position[0].y += speed;
		}
		else if (window.IsKeyPressed(GLFW_KEY_S)) {
			lights.point.position[0].y -= speed;
		}
	}
	else {
		if (window.IsKeyPressed(GLFW_KEY_W)) {
			lights.point.position[0].z -= speed;
		}
		else if (window.IsKeyPressed(GLFW_KEY_S)) {
			lights.point.position[0].z += speed;
		}
	}
}

/**
* ��Ԃ̍X�V.
*/
void MainGameScene::Update() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (timer > 0.0f) {
		timer -= window.DeltaTime();
	}

	// ���f����y����]�p���X�V.
	angleY += glm::radians(0.5f);
	if (angleY > glm::radians(360.0f)) {
		angleY -= glm::radians(360.0f);
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
	// ���_��ݒ肷��.
	const glm::vec3 viewPos(20, 30, 30);

	progLighting.Use();

	// ���W�ϊ��s����쐬���ăV�F�[�_�[�ɓ]������.
	const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
	const glm::mat4x4 matView = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progLighting.SetViewProjectionMatrix(matProj * matView);

	meshList.BindVertexArray();

	progLighting.SetLightList(lights);

	progLighting.BindTexture(0, texTree.Get());
	const float treeCount = 10;	// �؂̖{��.
	const float radius = 8;		// �؂�A����~�̔��a.
	for (float i = 0; i < treeCount; ++i) {
		const float theta = 3.14f * 2 / treeCount * i;
		const float x = std::cos(theta) * radius;
		const float z = std::sin(theta) * radius;

		progLighting.Draw(meshList.Get(0), glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
	}

	progLighting.BindTexture(0, texHouse.Get());
	const float houseCount = 3;		// �Ƃ̐�.
	const float houseRadius = 14;	// �Ƃ�u���~�̔��a.
	for (float i = 0; i < houseCount; i++) {
		if (i == 0) {
			progLighting.Draw(meshList.Get(1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));
		}
		else {
			const float x = std::cos(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
			const float z = std::sin(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
			progLighting.Draw(meshList.Get(1), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
		}
	}

	progLighting.BindTexture(0, texRock.Get());
	const float rockCount = 4;		// ��̐�.
	const float rockRadius = 5;		// ���u���~�̔��a.
	for (float i = 0; i < rockCount; i++) {
		const float x = std::cos(3.14f * 2 / rockCount * i) * rockRadius;
		const float z = std::sin(3.14f * 2 / rockCount * i) * rockRadius;
		progLighting.Draw(meshList.Get(2), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	}

	// �n��.	
	progLighting.BindTexture(0, texGround.Get());
	progLighting.Draw(meshList.Get(3), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));

	// �l��.	
	progLighting.BindTexture(0, texHuman.Get());
	progLighting.Draw(meshList.Get(4), glm::vec3(7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

	progLighting.BindTexture(0, texBarrel.Get());
	const float barrelCount = 6;		// �M�̐�.
	const float barrelRadius = 16;		// �M��u���~�̔��a.
	for (float i = 0; i < barrelCount; i++) {
		const float x = std::cos(3.14f * 2 / barrelCount * i) * barrelRadius;
		const float z = std::sin(3.14f * 2 / barrelCount * i) * barrelRadius;
		progLighting.Draw(meshList.Get(5), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	}

	// �񐢑яZ��.	
	progLighting.BindTexture(0, texHome.Get());
	progLighting.Draw(meshList.Get(6), glm::vec3(20, 0, -17), glm::vec3(0, 0, 0), glm::vec3(1));

	progLighting.BindTexture(0, texLeaf.Get());
	const float leafCount = 8;		// �t�̐�.
	const float leafRadius = 18;		// �t��u���~�̔��a.
	for (float i = 0; i < leafCount; i++) {
		const float x = std::cos(3.14f * 2 / leafCount * i) * leafRadius;
		const float z = std::sin(3.14f * 2 / leafCount * i) * leafRadius;
		progLighting.Draw(meshList.Get(7), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	}

	progLighting.BindTexture(0, texMushroom.Get());
	const float mushroomCount = 10;		// �L�m�R�̐�.
	const float mushroomRadius = 20;		// �L�m�R��u���~�̔��a.
	for (float i = 0; i < leafCount; i++) {
		const float x = std::cos(3.14f * 2 / mushroomCount * i) * mushroomRadius;
		const float z = std::sin(3.14f * 2 / mushroomCount * i) * mushroomRadius;
		progLighting.Draw(meshList.Get(8), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
	}

	// �|�C���g�E���C�g�̈ʒu���킩��悤�ɓK���ȃ��f����\��.
	{
		// ���C�g�̈ʒu�ɖ؂̃��f����\��.		
		progLighting.BindTexture(0, texTree.Get());
		for (int i = 0; i < 8; ++i) {
			progLighting.Draw(meshList.Get(0), lights.point.position[i], glm::vec3(0, angleY, 0), glm::vec3(0.5f, -0.5f, 0.5f));
		}
	}	
}

/**
* �I��.
*/
void MainGameScene::Finalize() {
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