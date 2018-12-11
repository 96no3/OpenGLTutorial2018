/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"
#include "MeshList.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>


/**
* エントリーポイント.
*/
int main() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (!window.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}
	MeshList meshList;
	if (!meshList.Allocate()) {
		return 1;
	}

	const GLuint shaderProgram = Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag");
	const GLuint pidLighting = Shader::BuildFromFile("Res/FragmentLighting.vert", "Res/FragmentLighting.frag");
	if (!shaderProgram || !pidLighting) {
		return 1;
	}	
	
	// uniform変数の位置を取得する.
	const GLint matMVPLoc = glGetUniformLocation(shaderProgram, "matMVP");
	if (matMVPLoc < 0) {
		std::cerr << "ERROR: uniform 変数'matMVP'が見つかりません.\n";
		return 1;
	}
	glUseProgram(shaderProgram);
	const GLint texColorLoc = glGetUniformLocation(shaderProgram, "texColor");
	if (texColorLoc >= 0) {
		glUniform1i(texColorLoc, 0);
	}
		
	Shader::Program progLighting(pidLighting);	

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
	GLuint texId = Texture::CreateImage2D(imageWidth, imageHeight, imageData, GL_RGBA, GL_UNSIGNED_BYTE);
	GLuint texHouse = Texture::LoadImage2D("Res/house2.tga");
	if (!texId) {
		return 1;
	}

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
	GLuint texId2 = Texture::CreateImage2D(imageWidth, imageHeight, imageData2, GL_RGBA, GL_UNSIGNED_BYTE);
	if (!texId2) {
		return 1;
	}

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
	GLuint texId3 = Texture::CreateImage2D(imageWidth, imageHeight, imageData3, GL_RGBA, GL_UNSIGNED_BYTE);
	//GLuint texRock = Texture::LoadImage2D("Res/rock.tga");
	GLuint texRock = Texture::LoadImage2D("Res/Model/Rock2.tga");
	if (!texId3) {
		return 1;
	}
	GLuint texTree = Texture::LoadImage2D("Res/Model/Tree2.tga");
	GLuint texGround = Texture::LoadImage2D("Res/Model/Ground.tga");
	GLuint texHuman = Texture::LoadImage2D("Res/human.tga");

	// ライトの設定.	
	Shader::LightList lights;
	lights.ambient.color = glm::vec3(0.05f, 0.1f, 0.1f);			// 環境光の色.
	lights.directional.direction = glm::normalize(glm::vec3(5, -2, -2));	// 指向性ライトの方向.
	lights.directional.color = glm::vec3(1.0f, 1.0f, 1.0f);			// 指向性ライトの色.
	lights.point.position[0] = glm::vec3(5, 4, 0);
	lights.point.color[0] = glm::vec3(1.0f, 0.8f, 0.4f) * 100.0f;
	lights.spot.dirAndCutOff[0] = glm::vec4(glm::normalize(glm::vec3(-1, -2, -2)), std::cos(glm::radians(20.0f)));
	lights.spot.posAndInnerCutOff[0] = glm::vec4(-6, 6, 8, std::cos(glm::radians(15.0f)));
	lights.spot.color[0] = glm::vec3(0.4f, 0.8f, 1.0f) * 200.0f;

	// メインループ.
	window.InitTimer();
	while (!window.ShouldClose()) {
		window.UpdateTimer();
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
		// 視点を設定する.
		const glm::vec3 viewPos(20, 30, 30);
		
		progLighting.Use();

		// 座標変換行列を作成してシェーダーに転送する.
		const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
		const glm::mat4x4 matView = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		progLighting.SetViewProjectionMatrix(matProj * matView);
				
		meshList.BindVertexArray();
		
		progLighting.SetLightList(lights);
		//progLighting.BindTexture(0, texId2);
		progLighting.BindTexture(0, texTree);

		const float treeCount = 10;	// 木の本数.
		const float radius = 8;		// 木を植える円の半径.
		for (float i = 0; i < treeCount; ++i) {			
			const float theta = 3.14f * 2 / treeCount * i;
			const float x = std::cos(theta) * radius;
			const float z = std::sin(theta) * radius;

			progLighting.Draw(meshList.Get(0), glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
		}

		//glUseProgram(shaderProgram);

		//glActiveTexture(GL_TEXTURE0);
		////glBindTexture(GL_TEXTURE_2D, texId);
		//glBindTexture(GL_TEXTURE_2D, texHouse);
		progLighting.BindTexture(0, texHouse);

		const float houseCount = 3;		// 家の数.
		const float houseRadius = 14;	// 家を置く円の半径.
		for (float i = 0; i < houseCount; i++) {
			if (i == 0) {
				/*const glm::mat4x4 matModel = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
				const glm::mat4x4 matMVP = matProj * matView * matModel;
				glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);
				glDrawElementsBaseVertex(meshList.Get(1).mode, meshList.Get(1).count, GL_UNSIGNED_SHORT, meshList.Get(1).indices, meshList.Get(1).baseVertex);*/
				progLighting.Draw(meshList.Get(1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));
			}
			else {
				const float x = std::cos(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
				const float z = std::sin(3.14f * 2 / (houseCount - 1) * i) * houseRadius;
				/*const glm::mat4x4 matModel = glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
				const glm::mat4x4 matMVP = matProj * matView * matModel;
				glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);
				glDrawElementsBaseVertex(meshList.Get(1).mode, meshList.Get(1).count, GL_UNSIGNED_SHORT, meshList.Get(1).indices, meshList.Get(1).baseVertex);*/
				progLighting.Draw(meshList.Get(1), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
			}
		}

		/*glUseProgram(shaderProgram);

		glActiveTexture(GL_TEXTURE0);*/
		//glBindTexture(GL_TEXTURE_2D, texId3);
		glBindTexture(GL_TEXTURE_2D, texRock);

		const float rockCount = 4;		// 岩の数.
		const float rockRadius = 5;		// 岩を置く円の半径.
		for (float i = 0; i < rockCount; i++) {
			const float x = std::cos(3.14f * 2 / rockCount * i) * rockRadius;
			const float z = std::sin(3.14f * 2 / rockCount * i) * rockRadius;
			/*const glm::mat4x4 matModel = glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
			const glm::mat4x4 matMVP = matProj * matView * matModel;
			glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);
			glDrawElementsBaseVertex(meshList.Get(2).mode, meshList.Get(2).count, GL_UNSIGNED_SHORT, meshList.Get(2).indices, meshList.Get(2).baseVertex);*/
			progLighting.Draw(meshList.Get(2), glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
		}

		//progLighting.Use();

		// 地面.		
		//progLighting.BindTexture(0, texId3);
		progLighting.BindTexture(0, texGround);
		progLighting.Draw(meshList.Get(3), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));
		
		// 人間.	
		progLighting.BindTexture(0, texHuman);
		progLighting.Draw(meshList.Get(4), glm::vec3(7, 0, 7), glm::vec3(0, 0, 0), glm::vec3(1));

		// ポイント・ライトの位置がわかるように適当なモデルを表示.
		{
			// 0番のポイント・ライトを移動する.
			const float speed = 10.0f * deltaTime;
			if (window.IsKeyPressed(GLFW_KEY_A)) {
				//pointLightPos[0].x -= speed;
				lights.point.position[0].x -= speed;
			}else if (window.IsKeyPressed(GLFW_KEY_D)) {
				//pointLightPos[0].x += speed;
				lights.point.position[0].x += speed;
			}
			if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				if (window.IsKeyPressed(GLFW_KEY_W)) {
					//pointLightPos[0].y += speed;
					lights.point.position[0].y += speed;
				}else if (window.IsKeyPressed(GLFW_KEY_S)) {
					//pointLightPos[0].y -= speed;
					lights.point.position[0].y -= speed;
				}
			}
			else {
				if (window.IsKeyPressed(GLFW_KEY_W)) {
					//pointLightPos[0].z -= speed;
					lights.point.position[0].z -= speed;
				}
				else if (window.IsKeyPressed(GLFW_KEY_S)) {
					//pointLightPos[0].z += speed;
					lights.point.position[0].z += speed;
				}
			}

			// モデルのy軸回転角を更新.
			static float angleY = 0;
			angleY += glm::radians(0.5f);
			if (angleY > glm::radians(360.0f)) {
				angleY -= glm::radians(360.0f);
			}

			// ライトの位置に木のモデルを表示.
			glUseProgram(shaderProgram);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texTree);
			for (int i = 0; i < 8; ++i) {
				//const glm::mat4 matModelT = glm::translate(glm::mat4(1), pointLightPos[i]);
				const glm::mat4 matModelT = glm::translate(glm::mat4(1), lights.point.position[i]);
				const glm::mat4 matModelR = glm::rotate(glm::mat4(1), angleY, glm::vec3(0, 1, 0));
				const glm::mat4 matModelS = glm::scale(glm::mat4(1), glm::vec3(0.5f, -0.5f, 0.5f));
				const glm::mat4 matMVP = matProj * matView * matModelT * matModelR * matModelS;
				glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);
				glDrawElementsBaseVertex(meshList.Get(0).mode, meshList.Get(0).count, GL_UNSIGNED_SHORT, meshList.Get(0).indices, meshList.Get(0).baseVertex);
			}
		}
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		window.SwapBuffers();

	}
	glDeleteTextures(1, &texHouse);
	glDeleteTextures(1, &texRock);
	glDeleteTextures(1, &texTree);
	glDeleteTextures(1, &texGround);
	glDeleteTextures(1, &texHuman);
	glDeleteTextures(1, &texId);
	glDeleteTextures(1, &texId2);
	glDeleteTextures(1, &texId3);
	glDeleteProgram(shaderProgram);

	return 0;
}