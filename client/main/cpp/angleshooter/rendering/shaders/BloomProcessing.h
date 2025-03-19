#pragma once

class BloomProcessing final : PostProcessing {
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;
	sf::RenderTexture brightness;
	RenderTextureArray firstPasses;
	RenderTextureArray secondPasses;
	void prepareTextures(sf::Vector2u size);
	void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
	void blurMultiPass(RenderTextureArray& renderTextures);
	void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
	void downSample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);

public:
	static const Identifier BRIGHTNESS;
	static const Identifier DOWNSAMPLE;
	static const Identifier GAUSSIAN;
	static const Identifier ADD;
	explicit BloomProcessing();
	void apply(sf::RenderTexture& input, sf::RenderTarget& output) override;
};