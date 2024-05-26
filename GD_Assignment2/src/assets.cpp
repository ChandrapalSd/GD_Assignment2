#include "assets.hpp"
#include <SFML/Audio.hpp>
#include <cassert>


sf::Texture& Assets::addTexture(std::string name, std::string path)
{
	sf::Texture texture;
	texture.loadFromFile(path);

	assert(m_textures.find(name) == m_textures.end() && "Duplicate texture asset");
	m_textures[name] = std::move(texture);
	return m_textures[name];
}

sf::Font& Assets::addFont(std::string name, std::string path)
{
	sf::Font font;
	font.loadFromFile(path);

	assert(m_fonts.find(name) == m_fonts.end() && "Duplicate font asset");
	m_fonts[name] = std::move(font);
	return m_fonts[name];
}

sf::Sound& Assets::addSound(std::string name, std::string path)
{
	assert(m_sounds.find(name) == m_sounds.end() && "Duplicate sound asset");
	m_sounds[name] = { sf::SoundBuffer(), sf::Sound()};
	auto& [sbuffer, sound] = m_sounds[name];
	sbuffer.loadFromFile(path);
	sound.setBuffer(sbuffer);
	return m_sounds[name].second;
}

sf::Texture& Assets::getTexture(std::string name)
{
	assert(m_textures.find(name) != m_textures.end() && "Texture asset not found");
	return m_textures[name];
}

sf::Font& Assets::getFont(std::string name)
{
	assert(m_fonts.find(name) != m_fonts.end() && "Font asset not found");
	return m_fonts[name];
}

sf::Sound& Assets::getSound(std::string name)
{
	assert(m_sounds.find(name) != m_sounds.end() && "sound asset not found");
	return m_sounds[name];
}

