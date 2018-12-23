/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "MainGameScene.h"
#include "TitleScene.h"

/**
* エントリーポイント.
*/
int main() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (!window.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}

	/*MainGameScene mainGameScene;
	if (!mainGameScene.Initialize()) {*/
	/*TitleScene* pTitleScene = new TitleScene;
	MainGameScene* pMainGameScene = nullptr;
	if (!pTitleScene || !pTitleScene->Initialize()) {*/
	Scene* pScene = new TitleScene;
	if (!pScene || !pScene->Initialize()) {
		delete pScene;
		return 1;
	}

	// メインループ.
	window.InitTimer();
	while (!window.ShouldClose()) {
		window.UpdateTimer();

		/*mainGameScene.ProcessInput();
		mainGameScene.Update();
		mainGameScene.Render();*/
		/*if (pTitleScene) {
			if (pTitleScene->IsFinish()) {
				glFinish();
				pTitleScene->Finalize();
				delete pTitleScene;
				pTitleScene = nullptr;
				pMainGameScene = new MainGameScene;
				if (!pMainGameScene || !pMainGameScene->Initialize()) {
					delete pMainGameScene;
					return 1;
				}
			}
			else {
				pTitleScene->ProcessInput();
				pTitleScene->Update();
				pTitleScene->Render();
			}
		}
		else if (pMainGameScene) {
			pMainGameScene->ProcessInput();
			pMainGameScene->Update();
			pMainGameScene->Render();
		}*/
		const std::string& nextScene = pScene->NextScene();
		if (!nextScene.empty()) {
			glFinish();
			if (nextScene == "MainGameScene") {
				pScene->Finalize();
				delete pScene;
				pScene = new MainGameScene;
			}
			else if (nextScene == "TitleScene") {
				pScene->Finalize();
				delete pScene;
				pScene = new TitleScene;
			}

			if (!pScene || !pScene->Initialize()) {
				return 1;
			}
		}
		pScene->ProcessInput();
		pScene->Update();
		pScene->Render();

		window.SwapBuffers();
	}
	//mainGameScene.Finalize();
	/*if (pTitleScene) {
		pTitleScene->Finalize();
		delete pTitleScene;
	}
	if (pMainGameScene) {
		pMainGameScene->Finalize();
		delete pMainGameScene;
	}*/
	if (pScene) {
		pScene->Finalize();
		delete pScene;
	}

	return 0;
}