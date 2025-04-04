#include "PreCompiledServer.h"
#include "ServerPlayerEntity.h"

ServerPlayerEntity::ServerPlayerEntity(World* world, const std::string& name) : PlayerEntity(world, name) {}

void ServerPlayerEntity::tick(float deltaTime) {
	PlayerEntity::tick(deltaTime);
	static Identifier shootSound("bullet.ogg");
	if (isFiring && this->bulletCharge >= 12) {
		this->bulletCharge -= 12;
		auto x = std::cos(this->getRotation().asRadians());
		auto y = std::sin(this->getRotation().asRadians());
		x += Util::randomNormalFloat(0.025f);
		y += Util::randomNormalFloat(0.025f);
		const auto velocity = sf::Vector2f(x, y);
		const auto bullet = dynamic_cast<ServerWorld*>(this->world)->spawnBullet(this->color, this->getPosition(), velocity * 8.f);
		bullet->setRotation(this->getRotation());
		this->world->playSound(shootSound, .6f, Util::randomFloat(1.f, 1.6f));
	}
	if (this->world->getAge() % 12 == 0) {
		auto packet = NetworkProtocol::S2C_PLAYER_POSITION_SYNC.getPacket();
		packet << this->getName();
		packet << this->getPosition().x;
		packet << this->getPosition().y;
		AngleShooterServer::get().sendToAll(packet);
		for (const auto& client : AngleShooterServer::get().clients) {
			if (client->name == this->getName()) continue;
			AngleShooterServer::get().send(client->socket, packet);
		}
	}
}