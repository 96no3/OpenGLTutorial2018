/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* 初期化.
*/
bool TitleScene::Initialize() {

	if (!meshList.Allocate()) {
		return false;
	}

	progSimple.Reset(Shader::BuildFromFile("Res/Shader/Simple.vert", "Res/Shader/Simple.frag"));

	texLogo.Reset(Texture::LoadImage2D("Res/Image/TitleLogo.tga"));
	texBackGround.Reset(Texture::LoadImage2D("Res/Image/TitleBack.tga"));

	timer = 1.0f;
	isFinish = false;

	return true;
}

/**
* 入力の反映.
*/
void TitleScene::ProcessInput() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (!isFinish && timer <= 0.0f) {
		if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
			isFinish = true;
			NextScene("MainGameScene");
		}
	}
}

/**
* 状態の更新.
*/
void TitleScene::Update() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (timer > 0.0f) {
		timer -= window.DeltaTime();
	}
}

/**
* 描画.
*/
void TitleScene::Render() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// 半透明合成を有効にする.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	meshList.BindVertexArray();

	progSimple.Use();

	// 正射影で描画するように行列を設定.
	const glm::mat4x4 matProj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 1.0f, 500.0f);
	const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	// 背景とロゴをウィンドウの中心に描画.
	progSimple.BindTexture(0, texBackGround.Get());
	progSimple.Draw(meshList.Get(12), glm::vec3(400, 300, -1), glm::vec3(0), glm::vec3(400, 300, 1));
	progSimple.BindTexture(0, texLogo.Get());
	progSimple.Draw(meshList.Get(12), glm::vec3(400, 300, 0), glm::vec3(0), glm::vec3(400, 300, 1));
}

/**
* 終了.
*/
void TitleScene::Finalize() {

}

/**
* タイトル画面が終わったかどうか調べる.
*
* @retval true	終わった.
* @retval false	終わっていない.
*/
bool TitleScene::IsFinish() const {
	return isFinish;
}