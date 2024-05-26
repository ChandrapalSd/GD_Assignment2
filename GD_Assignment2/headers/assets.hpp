#pragma once
#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Sound.hpp>

class Assets
{
public:
	Assets() = default;

	sf::Texture& addTexture(std::string name, std::string path);
	sf::Font& addFont(std::string name, std::string path);
	sf::Sound& addSound(std::string name, std::string path);
	// Animation& addAnimation(std::string name, Animation animation);
	
	sf::Texture& getTexture(std::string name);
	sf::Font& getFont(std::string name);
	sf::Sound& getSound(std::string name);
	//Animation& getAnimation(std::string name);

private:
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, sf::Font> m_fonts;
	std::map<std::string, std::pair<sf::SoundBuffer, sf::Sound>> m_sounds;
	//std::map<std::string, Animation> m_animations;
};
