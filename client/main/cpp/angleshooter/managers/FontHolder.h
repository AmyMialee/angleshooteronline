#pragma once

class FontHolder final : public ResourceHolder<sf::Font>, public Singleton<FontHolder> {
	explicit FontHolder();
	std::unique_ptr<sf::Font>* createDefaultFont();
};