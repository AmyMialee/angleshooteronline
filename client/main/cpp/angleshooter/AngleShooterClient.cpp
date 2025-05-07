#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

int main(int, char*[]) {
	try {
		AngleShooterClient::get().run();
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}

AngleShooterClient::AngleShooterClient() :
	window(sf::VideoMode({960, 540}), "Angle Shooter", sf::Style::Titlebar | sf::Style::Close),
	renderTexture({480, 270})
{
	NetworkProtocol::initialize();
	window.clear();
	window.setKeyRepeatEnabled(false);
	socket.setBlocking(false);
	if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) throw std::runtime_error("Failed to bind to port, no ports are remaining???");
	Logger::info("Client started on port " + std::to_string(socket.getLocalPort()));
	registerPacket(NetworkProtocol::PING, [this](sf::Packet&, NetworkPair* sender) {
		Logger::debug("Ping! from " + sender->getPortedIP()->toString());
		auto pong = NetworkProtocol::PONG->getPacket(sender);
		send(pong);
	});
	registerPacket(NetworkProtocol::PONG, [this](sf::Packet&, NetworkPair* sender) {
		const auto rtt = sender->stopRoundTripTimer();
		Logger::debug("Pong! from " + sender->getPortedIP()->toString() + " in " + Util::toRoundedString(rtt * 1000, 0) + "ms");
	});
	registerPacket(NetworkProtocol::ACK, [this](sf::Packet& packet, NetworkPair* sender) {
		uint32_t sequence;
		packet >> sequence;
		sender->acceptAcknowledgment(sequence);
	});
	registerPacket(NetworkProtocol::CHAT_MESSAGE, [this](sf::Packet& packet, const NetworkPair*) {
		std::string message;
		packet >> message;
		Logger::info(message);
	});
}

void AngleShooterClient::handlePacket(sf::Packet& packet, NetworkPair* sender) {
	sender->resetTimeout();
	uint8_t packetType;
	packet >> packetType;
	if (PacketIdentifier::fromId(packetType)->isReliable()) {
		uint32_t sequence;
		packet >> sequence;
		if (sequence < sender->getAcknowledgedSequence()) {
			auto ack = NetworkProtocol::ACK->getPacket(sender);
			ack << sequence;
			send(ack);
			Logger::debug("Received redundant sequence: " + std::to_string(sequence) + " from " + sender->getPortedIP()->toString());
			return;
		}
		if (!sender->setAcknowledgedSequence(sequence)) {

			Logger::debug("Received premature sequence: " + std::to_string(sequence) + " expected " + std::to_string(sender->getAcknowledgedSequence()) + " from " + sender->getPortedIP()->toString());
		} else {
			auto ack = NetworkProtocol::ACK->getPacket(sender);
			ack << sequence;
			send(ack);
		}
	}
	if (packetHandlers.contains(packetType)) {
		packetHandlers[packetType](packet, sender);
		if (packet.getReadPosition() != packet.getDataSize()) {
			Logger::error("Packet " + packetIds[packetType]->toString() + " has unused data");
			packet.clear();
		}
		return;
	}
	Logger::error("Received unknown packet id: " + std::to_string(packetType) + " from " + sender->getPortedIP()->toString());
}

void AngleShooterClient::registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, NetworkPair* sender)>& handler) {
	this->packetHandlers.emplace(packetType->getId(), handler);
	this->packetIds.emplace(packetType->getId(), packetType);
	Logger::debug("Registered packet: " + packetType->toString() + " (" + std::to_string(packetType->getId()) + ")");
}

void AngleShooterClient::run() {
	Logger::debug("Starting AngleShooter Client");
    std::thread receiverThread(&AngleShooterClient::runReceiver, this);
	sf::Clock deltaClock;
	auto frameTime = 0.;
	auto tickTime = 0.;
	auto secondTime = 0.;
	auto frames = 0;
	auto ticks = 0;
	auto loops = 0;
	while (this->running && window.isOpen()) {
		const auto deltaTime = deltaClock.restart().asSeconds();
		tickTime += deltaTime;
		frameTime += deltaTime;
		secondTime += deltaTime;
		InputManager::get().handleInput(window);
		if (tickTime > 1.) {
			Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK) + " ticks (" + Util::toRoundedString(tickTime) + " seconds), skipping ahead");
			tickTime = AngleShooterCommon::TIME_PER_TICK;
		}
		while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
			tickTime -= AngleShooterCommon::TIME_PER_TICK;
			this->tick();
			++ticks;
		}
		if (const auto timePerFrame = AngleShooterCommon::TIME_PER_TICK; frameTime >= timePerFrame) {
			frameTime -= timePerFrame;
			render(static_cast<float>(tickTime / AngleShooterCommon::TIME_PER_TICK));
			while (frameTime >= timePerFrame) frameTime -= timePerFrame;
			++frames;
		}
		++loops;
		if (secondTime >= .1f) {
			tps = ticks / secondTime;
			fps = frames / secondTime;
			lps = loops / secondTime;
			ticks = 0;
			frames = 0;
			loops = 0;
			secondTime = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(6));
	}
}

void AngleShooterClient::tick() {
}

void AngleShooterClient::render(float deltaTime) {
	window.clear();
	renderTexture.clear();
	{
		auto offset = 0;
		auto fill = [&](const std::string& words) {
			static auto text = sf::Text(FontHolder::get().getDefault(), "", 12);
			text.setString(words);
			text.setFillColor(sf::Color::Black);
			text.setPosition(sf::Vector2f{4.f, 4.f + offset} + sf::Vector2f{1.f, 1.f});
			renderTexture.draw(text);
			text.setFillColor(sf::Color::White);
			text.setPosition({4.f, 4.f + offset});
			renderTexture.draw(text);
			offset += 14;
		};
		fill("TPS: " + Util::toRoundedString(tps, 2));
		fill("FPS: " + Util::toRoundedString(fps, 2));
		fill("LPS: " + Util::toRoundedString(lps, 2));
		fill("Sequence: " + std::to_string(this->server ? this->server->getAcknowledgedSequence() : 0));
	}
	renderTexture.display();
	sf::Sprite sprite(renderTexture.getTexture());
	sprite.setScale({2.f, 2.f});
	window.draw(sprite);
	window.display();
}

void AngleShooterClient::runReceiver() {
	Logger::info("Starting Client Network Handler");
	const auto ip = sf::IpAddress::resolve("127.0.0.1");
	auto pip = PortedIP{.ip= ip.value(), .port= AngleShooterCommon::PORT};
	connect(pip);
	while (this->running && window.isOpen()) {
		if (!this->server) {
			sleep(sf::milliseconds(128));
			continue;
		}
		std::optional<sf::IpAddress> sender;
		unsigned short port;
		if (sf::Packet packet; socket.receive(packet, sender, port) == sf::Socket::Status::Done) {
			if (!sender.has_value()) continue;
			if (auto pip = PortedIP{.ip= sender.value(), .port= port}; pip != *this->server->getPortedIP()) {
				Logger::warn("Received packet from non-server address: " + pip.toString());
				continue;
			}
			handlePacket(packet, this->server);
			continue;
		}
		this->server->update();
		if (this->server->shouldDisconnect()) {
			Logger::info("Disconnected from server " + this->server->getPortedIP()->toString());
			//TODO: Send disconnect packet
			delete(this->server);
			this->server = nullptr;
		}
		sleep(sf::milliseconds(6));
	}
}

void AngleShooterClient::send(sf::Packet& packet) {
	this->server->send(packet);
}

void AngleShooterClient::connect(PortedIP& server) {
	this->server = new NetworkPair(*this, server);
	//TODO: Send connection request packet
}

sf::UdpSocket& AngleShooterClient::getSocket() {
	return this->socket;
}