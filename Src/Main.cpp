/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "MainGameScene.h"
#include "TitleScene.h"
#include "Audio.h"

/**
* エントリーポイント.
*/
int main() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (!window.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}
	// 音声再生システムの初期化.
	Audio::EngineRef audio = Audio::Engine::Instance();
	if (!audio.Initialize()) {
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
		audio.Update();
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

		/*const std::string& nextScene = pScene->NextScene();
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
		pScene->Render();*/

		pScene->ProcessInput();
		pScene->Update();
		pScene->Render();
		// NextSceneが設定されていたら、シーンを切り替える.
		if (!pScene->NextScene().empty()) {
			const std::string sceneName = pScene->NextScene();
			pScene->Finalize();
			delete pScene;
			pScene = nullptr;
			if (sceneName == "MainGameScene") {
				pScene = new MainGameScene;
			}
			else if (sceneName == "TitleScene") {
				pScene = new TitleScene;
			}

			if (!pScene || !pScene->Initialize()) {
				break;
			}
		}
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
	audio.Destroy();
	return 0;
}