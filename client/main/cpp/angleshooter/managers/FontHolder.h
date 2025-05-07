#pragma once

class FontHolder final : public ResourceHolder<sf::Font> {
	std::unique_ptr<sf::Font> createDefaultFont();

protected:
	explicit FontHolder();
	~FontHolder() = default;

public:
	FontHolder(const FontHolder&) = delete;
	void operator=(const FontHolder&) = delete;
	static FontHolder& get() {
		static FontHolder instance;
		return instance;
	}
};