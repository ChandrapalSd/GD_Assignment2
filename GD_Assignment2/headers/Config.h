#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>

class Config
{
public:
	Config(std::string filePath);
public:
	size_t windowWidth;
	size_t windowHeight;
	size_t windowFPS;
	bool windowFullScreen;

	std::string fontFilePath;
	size_t fontSize;
	uint8_t fontR, fontG, fontB;

	size_t playerSR; // Shape Radius
	size_t playerCR; // Collision Radius
	float playerSpeed;
	uint8_t playerFR, playerFG, playerFB; // Fill Color
	uint8_t playerOR, playerOG, playerOB; // Outline Color
	size_t playerOT; // Outline Thickness
	size_t playerVertices; // Shape vertices count or point count
	
	size_t enemySR; // Shape Radius
	size_t enemyCR; // Collision Radius
	float enemySMin, enemySMax; // Speed range
	uint8_t enemyFR, enemyFG, enemyFB; // Fill Color
	uint8_t enemyOR, enemyOG, enemyOB; // Outline Color
	size_t enemyOT; // Outline Thickness
	size_t enemyVertMin, enemyVertMax; // Shape vertices count or point count
	size_t enemyL; // Small lifespan
	size_t enemySP; // Spawn interval

	size_t bulletSR; // Shape Radius
	size_t bulletCR; // Collision Radius
	float bulletSpeed;
	uint8_t bulletFR, bulletFG, bulletFB; // Fill Color
	uint8_t bulletOR, bulletOG, bulletOB; // Outline Color
	size_t bulletOT; // Outline Thickness
	size_t bulletVertices; // Shape vertices count or point count
	size_t bulletLifespan;
};

