#include "PreCompiledClient.h"
#include "WorldRenderer.h"

#include "../game/ClientWorld.h"

WorldRenderer::WorldRenderer() {
	registerRenderer<PlayerEntity>(PlayerEntity::ID, [this](const std::shared_ptr<PlayerEntity>& player, float deltaTime) {
		if (player->deathTime > 0) return;
		static sf::Sprite player1(TextureHolder::getInstance().get(Identifier("player.png")));
		static std::once_flag flag;
		std::call_once(flag, [&] {
			Util::centre(player1);
			const auto textureSize1 = player1.getTexture().getSize();
			player1.setScale(player->getScale().componentWiseDiv({static_cast<float>(textureSize1.x), static_cast<float>(textureSize1.y)}));
		});
		const auto pos = player->getPosition() + player->getVelocity() * deltaTime;
		auto sprite = player1;
		sprite.setPosition(pos);
		sprite.setRotation(player->getRotation());
		sprite.setColor(player->getColour());
		AngleShooterClient::get().renderTexture.draw(sprite);
		if (player->getImmunityTime() > 0) {
			auto circle = sf::CircleShape(player->getScale().x / 2.f + 2);
			circle.setPosition(pos - player->getScale() / 2.f - sf::Vector2f{2.f, 2.f});
			circle.setFillColor(sf::Color(0, 255, 255, 120));
			AngleShooterClient::get().renderTexture.draw(circle);
		}
		const auto bound = player->getBoundingBox();
		sf::RectangleShape shape;
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(0.5f);
		shape.setPosition(bound.position + player->getVelocity() * deltaTime + sf::Vector2f{0, bound.size.y + 2});
		shape.setSize({bound.size.x, 2});
		shape.setFillColor(sf::Color::Black);
		AngleShooterClient::get().renderTexture.draw(shape);
		shape.setSize({bound.size.x * (static_cast<float>(player->getHealth()) / 8.f), 2});
		shape.setFillColor(sf::Color::Green);
		AngleShooterClient::get().renderTexture.draw(shape);
		if (player->getImmunityTime() <= 0 && player->getBulletCharge() < 120) {
			shape.setPosition(bound.position + player->getVelocity() * deltaTime + sf::Vector2f{0, bound.size.y + 4.5f});
			shape.setSize({bound.size.x, 2});
			shape.setFillColor(sf::Color::Black);
			AngleShooterClient::get().renderTexture.draw(shape);
			shape.setSize({bound.size.x * (static_cast<float>(player->getBulletCharge()) / 120.f), 2});
			shape.setFillColor(sf::Color::Cyan);
			AngleShooterClient::get().renderTexture.draw(shape);
		}
		static sf::Text text(FontHolder::getInstance().getDefault());
		text.setCharacterSize(6);
		text.setFillColor(sf::Color::White);
		text.setString(player->getName());
		text.setPosition({pos.x - text.getGlobalBounds().size.x / 2, pos.y - 16});
		AngleShooterClient::get().renderTexture.draw(text);
	});
	registerRenderer<BulletEntity>(BulletEntity::ID, [this](const std::shared_ptr<BulletEntity>& bullet, float deltaTime) {
		static sf::Sprite bulletCore(TextureHolder::getInstance().get(Identifier("bullet_core.png")));
		static sf::Sprite bulletRing(TextureHolder::getInstance().get(Identifier("bullet_ring.png")));
		static std::once_flag flag;
		std::call_once(flag, [&] {
			Util::centre(bulletCore);
			Util::centre(bulletRing);
			const auto textureSizeCore = bulletCore.getTexture().getSize();
			const auto textureSizeRing = bulletRing.getTexture().getSize();
			bulletCore.setScale(bullet->getScale().componentWiseDiv({static_cast<float>(textureSizeCore.x), static_cast<float>(textureSizeCore.y)}));
			bulletRing.setScale(bullet->getScale().componentWiseDiv({static_cast<float>(textureSizeRing.x), static_cast<float>(textureSizeRing.y)}));
		});
		const auto pos = bullet->getPosition() + bullet->getVelocity() * deltaTime;
		bulletCore.setPosition(pos);
		bulletRing.setPosition(pos);
		bulletRing.setColor(bullet->colour);
		AngleShooterClient::get().renderTexture.draw(bulletCore);
		AngleShooterClient::get().renderTexture.draw(bulletRing);
	});
}

void WorldRenderer::render(float deltaTime) {
	auto minX = std::numeric_limits<float>::max();
	auto minY = std::numeric_limits<float>::max();
	auto maxX = std::numeric_limits<float>::min();
	auto maxY = std::numeric_limits<float>::min();
	for (const auto& gameObject : ClientWorld::get().getEntities()) {
		if (const auto player = dynamic_cast<PlayerEntity*>(gameObject.get()); player != nullptr) {
			const auto position = player->getPosition() + player->input * deltaTime;
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
	auto view = AngleShooterClient::get().renderTexture.getView();
	view.setCenter(center);
	auto average = 160 + std::max((viewSize.size.x - viewSize.position.x) / 2, (viewSize.size.y - viewSize.position.y) / 2) * 3.2f;
	view.setSize({average, average / static_cast<float>(AngleShooterClient::get().renderTexture.getSize().x) * static_cast<float>(AngleShooterClient::get().renderTexture.getSize().y)});
	AngleShooterClient::get().renderTexture.setView(view);
	if (ClientWorld::get().mapRenderer != nullptr) ClientWorld::get().mapRenderer->render(deltaTime);
	for (const auto entity : ClientWorld::get().getEntities()) {
		if (auto renderer = renderRegistry.find(entity->getEntityType().getHash()); renderer != renderRegistry.end()) renderer->second(entity, deltaTime);
	}
	if (OptionsManager::get().isDebugEnabled()) {
		sf::RectangleShape shape;
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);
		for (const auto entity : ClientWorld::get().getEntities()) {
			const auto rect = entity->getBoundingBox();
			shape.setPosition(rect.position);
			shape.setSize(rect.size);
			AngleShooterClient::get().renderTexture.draw(shape);
			const auto pos = entity->getPosition();
			const auto rotation = entity->getRotation().asRadians();
			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0].position = pos;
			line[0].color = sf::Color::Red;
			line[1].position = pos + sf::Vector2f(std::cos(rotation) * 16, std::sin(rotation) * 16);
			line[1].color = sf::Color::Red;
			AngleShooterClient::get().renderTexture.draw(line);
		}
	}
	static auto bloomProcessing = BloomProcessing();
	bloomProcessing.apply(AngleShooterClient::get().renderTexture, AngleShooterClient::get().renderTexture);
	this->lastViewSize = viewSize;
}

template<typename T> void WorldRenderer::registerRenderer(const Identifier& id, std::function<void(std::shared_ptr<T>, float)> renderer) {
    renderRegistry[id.getHash()] = [renderer](const std::shared_ptr<Entity>& entity, float deltaTime) {
        renderer(std::static_pointer_cast<T>(entity), deltaTime);
    };
}