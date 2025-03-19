#pragma once

class PostProcessing {
protected:
	static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);

public:
	PostProcessing() = default;
	virtual ~PostProcessing() = default;
	PostProcessing(const PostProcessing&) = delete;
	PostProcessing& operator=(const PostProcessing&) = delete;
	virtual void apply(sf::RenderTexture& input, sf::RenderTarget& output) = 0;
	[[nodiscard]] static bool areShadersSupported();
};