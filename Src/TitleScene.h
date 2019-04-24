/**
* @file TitleScene.h
*/
#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED

#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Scene.h"

/**
* ƒ^ƒCƒgƒ‹‰æ–Ê.
*/
class TitleScene : public Scene {

public:
	TitleScene() = default;
	virtual ~TitleScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Finalize() override;

	bool IsFinish() const;

private:
	MeshList meshList;

	Texture::Image2D texLogo;
	Texture::Image2D texBackGround;

	Shader::Program progSimple;

	float timer;
	bool isFinish;
	Audio::SoundPtr se;
};

#endif //TITLESCENE_H_INCLUDED
