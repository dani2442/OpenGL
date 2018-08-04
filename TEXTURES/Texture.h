#pragma once
#include <string.h>
#include <iostream>

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileloc);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	char* fileLocation;
};

