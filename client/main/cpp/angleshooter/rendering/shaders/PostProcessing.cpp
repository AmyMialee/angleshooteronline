#include "PreCompiledClient.h"
#include "PostProcessing.h"

bool PostProcessing::areShadersSupported() {
	return sf::Shader::isAvailable();
}

void PostProcessing::applyShader(const sf::Shader& shader, sf::RenderTarget& output) {
	auto outputSize = static_cast<sf::Vector2f>(output.getSize());
	sf::VertexArray vertices(sf::PrimitiveType::TriangleStrip, 4);
	vertices[0] = {.position= {0, 0}, .color= sf::Color::White, .texCoords= {0, 1}};
	vertices[1] = {.position= {outputSize.x, 0}, .color= sf::Color::White, .texCoords= {1, 1}};
	vertices[2] = {.position= {0, outputSize.y}, .color= sf::Color::White, .texCoords= {0, 0}};
	vertices[3] = {.position= outputSize, .color= sf::Color::White, .texCoords= {1, 0}};
	sf::RenderStates states;
	states.shader = &shader;
	states.blendMode = sf::BlendNone;
	output.draw(vertices, states);
}