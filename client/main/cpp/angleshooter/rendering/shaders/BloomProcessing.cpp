#include "PreCompiledClient.h"
#include "BloomProcessing.h"

const Identifier BloomProcessing::BRIGHTNESS = Identifier("brightness");
const Identifier BloomProcessing::DOWNSAMPLE = Identifier("downsample");
const Identifier BloomProcessing::GAUSSIAN = Identifier("gaussian");
const Identifier BloomProcessing::ADD = Identifier("add");

BloomProcessing::BloomProcessing() {
	const auto fullpass = Identifier("fullpass.vert");
	ShaderHolder::get().load(BRIGHTNESS, fullpass, Identifier("brightness.frag"));
	ShaderHolder::get().load(DOWNSAMPLE, fullpass, Identifier("downsample.frag"));
	ShaderHolder::get().load(GAUSSIAN, fullpass, Identifier("gaussian.frag"));
	ShaderHolder::get().load(ADD, fullpass, Identifier("add.frag"));
}

void BloomProcessing::prepareTextures(sf::Vector2u size) {
	if (brightness.getSize() != size) {
		if (!brightness.resize({size.x, size.y})) Logger::errorOnce("Bloom Brightness resizing failed");
		brightness.setSmooth(true);
		if (!firstPasses[0].resize({size.x / 2, size.y / 2})) Logger::errorOnce("Bloom firstPasses[0] resizing failed");
		firstPasses[0].setSmooth(true);
		if (!firstPasses[1].resize({size.x / 2, size.y / 2})) Logger::errorOnce("Bloom firstPasses[1] resizing failed");
		firstPasses[1].setSmooth(true);
		if (!secondPasses[0].resize({size.x / 4, size.y / 4})) Logger::errorOnce("Bloom secondPasses[0] resizing failed");
		secondPasses[0].setSmooth(true);
		if (!secondPasses[1].resize({size.x / 4, size.y / 4})) Logger::errorOnce("Bloom secondPasses[1] resizing failed");
		secondPasses[1].setSmooth(true);
	}
}

void BloomProcessing::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output) {
	const auto brightness = ShaderHolder::get().getPointer(BRIGHTNESS);
	brightness->setUniform("source", input.getTexture());
	applyShader(*brightness, output);
	output.display();
}

void BloomProcessing::blurMultiPass(RenderTextureArray& renderTextures) {
	const auto textureSize = renderTextures[0].getSize();
	for (std::size_t count = 0; count < 2; ++count) {
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / static_cast<float>(textureSize.y)));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / static_cast<float>(textureSize.x), 0.f));
	}
}

void BloomProcessing::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor) {
	const auto gaussianBlur = ShaderHolder::get().getPointer(GAUSSIAN);
	gaussianBlur->setUniform("source", input.getTexture());
	gaussianBlur->setUniform("offsetFactor", offsetFactor);
	applyShader(*gaussianBlur, output);
	output.display();
}

void BloomProcessing::downSample(const sf::RenderTexture& input, sf::RenderTexture& output) {
	const auto downSampler = ShaderHolder::get().getPointer(DOWNSAMPLE);
	downSampler->setUniform("source", input.getTexture());
	downSampler->setUniform("sourceSize", sf::Vector2f(input.getSize()));
	applyShader(*downSampler, output);
	output.display();
}

void BloomProcessing::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target) {
	const auto adder = ShaderHolder::get().getPointer(ADD);
	adder->setUniform("source", source.getTexture());
	adder->setUniform("bloom", bloom.getTexture());
	applyShader(*adder, target);
}

void BloomProcessing::apply(sf::RenderTexture& input, sf::RenderTarget& output) {
	const auto view = input.getView();
	input.setView(input.getDefaultView());
	output.setView(output.getDefaultView());
	prepareTextures(input.getSize());
	filterBright(input, brightness);
	downSample(brightness, firstPasses[0]);
	blurMultiPass(firstPasses);
	downSample(firstPasses[0], secondPasses[0]);
	blurMultiPass(secondPasses);
	add(firstPasses[0], secondPasses[0], firstPasses[1]);
	firstPasses[1].display();
	add(input, firstPasses[1], output);
	input.setView(view);
	output.setView(view);
}