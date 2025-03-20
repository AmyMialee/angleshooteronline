#pragma once

class FontHolder final : public ResourceHolder<sf::Font>, public InstancedSingleton<FontHolder> {
	friend class InstancedSingleton;
	explicit FontHolder();
	std::unique_ptr<sf::Font> createDefaultFont();
};