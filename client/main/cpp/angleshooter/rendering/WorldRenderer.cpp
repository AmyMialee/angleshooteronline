#include "PreCompiledClient.h"
#include "WorldRenderer.h"

void WorldRenderer::render(float deltaTime) {
	const auto world = ClientContext::get()->getWorld();
	auto minX = std::numeric_limits<float>::max();
	auto minY = std::numeric_limits<float>::max();
	auto maxX = std::numeric_limits<float>::min();
	auto maxY = std::numeric_limits<float>::min();
	for (const auto& gameObject : world->getGameObjects()) {
		if (const auto player = dynamic_cast<PlayerEntity*>(gameObject.get()); player != nullptr) {
			const auto position = player->getPosition() + player->getVelocity() * deltaTime;
			minX = std::min(minX, position.x);
			minY = std::min(minY, position.y);
			maxX = std::max(maxX, position.x);
			maxY = std::max(maxY, position.y);
		}
	}
	const sf::FloatRect newViewSize = {{minX, minY}, {maxX, maxY}};
	const sf::FloatRect viewSize = {
		{lastViewSize.position.x * (1 - deltaTime) + newViewSize.position.x * deltaTime, lastViewSize.position.y * (1 - deltaTime) + newViewSize.position.y * deltaTime},
		{lastViewSize.size.x * (1 - deltaTime) + newViewSize.size.x * deltaTime, lastViewSize.size.y * (1 - deltaTime) + newViewSize.size.y * deltaTime}};
	const sf::Vector2f center = {viewSize.position.x / 2 + viewSize.size.x / 2, viewSize.position.y / 2 + viewSize.size.y / 2};
	const auto texture = ClientContext::get()->getRenderTexture();
	auto view = texture->getView();
	view.setCenter(center);
	auto average = 160 + std::max((viewSize.size.x - viewSize.position.x) / 2, (viewSize.size.y - viewSize.position.y) / 2) * 3.2f;
	view.setSize({average, average / static_cast<float>(texture->getSize().x) * static_cast<float>(texture->getSize().y)});
	texture->setView(view);
	world->getMap().render(deltaTime);
	for (const auto& gameObject : world->getGameObjects()) gameObject->baseRender(deltaTime);
	static auto bloomProcessing = BloomProcessing();
	bloomProcessing.apply(*ClientContext::get()->getRenderTexture(), *ClientContext::get()->getRenderTexture());
	this->lastViewSize = viewSize;
}