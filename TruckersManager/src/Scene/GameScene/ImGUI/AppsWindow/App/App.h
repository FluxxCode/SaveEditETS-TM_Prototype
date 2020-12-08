#pragma once

struct App
{
public:
	GLuint* preview = nullptr;
	bool active = false;
	float posX = 0;
	float posY = 0;
	std::string name = "None";
};