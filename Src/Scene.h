/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <string>
#include "Audio.h"

/**
* �V�[���̊��N���X.
*/
class Scene {

public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual bool Initialize() = 0;
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

	// �V�[���؂�ւ��p�̊֐�.
	const std::string& NextScene() const { return nextScene; }
	void NextScene(const char* name) { nextScene = name; }

protected:
	Audio::SoundPtr bgm;			// ��������p�ϐ�.

private:
	std::string nextScene;
};

#endif // SCENE_H_INCLUDED