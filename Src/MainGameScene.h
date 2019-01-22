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

/**
* ÉÅÉCÉìÉQÅ[ÉÄâÊñ .
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

	Shader::Program progSimple;
	Shader::Program progLighting;
	Shader::LightList lights;

	float angleY = 0;

	float timer;
	bool isFinish;
};

#endif //MAINGAMESCENE_H_INCLUDED

