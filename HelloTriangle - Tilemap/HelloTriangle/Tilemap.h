#pragma once

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"

#include "stb_image.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAP_WIDTH 7
#define MAP_HEIGHT 7

class Tilemap
{
public:
	Tilemap() {}
	void initialize(string mapPath);
	void initializePlayer(string spritePath, int nFrames, int nAnimations);
	void update();
	void draw();
	void drawPlayer();
	void finish();
	inline void setShader(Shader *shader) { this->shader = shader;  }
	inline void setCharacterPos(glm::vec2 pos) { this->pos = pos;  }
	bool isWalkable(int y, int x); //olha se pode andar no tile
	bool isKillable(int y, int x); //olha se o tile mata o personagem

protected:
	void loadMap(string mapPath);
	GLuint generateTexture(string filePath);
	int setupSprite();
	int setupSpritePlayer(int nFrames, int nAnimations, float &dx, float &dy);

	//Tileset
	vector <GLuint> tileset; //armazena os IDs de textura dos tiles
	glm::vec2 tileDimensions;

	//Buffer de geometria dos tiles (pode ser o mesmo)
	GLuint VAOTile;
	GLuint VAOPlayer;
	GLuint texID3;

	//Tilemap
	int map[MAP_WIDTH][MAP_HEIGHT]; //ATENÇÃO! Pode ser melhorado com alocação dinâmica

	//Ponteiro para o shader, para poder usá-lo aqui
	Shader* shader;

	//Pos do personagem (indice de qual tile ele está)
	glm::vec2 pos;

	//Variáveis do player
	float dx, dy;
	int iFrame;
	int iAnimation;
};

