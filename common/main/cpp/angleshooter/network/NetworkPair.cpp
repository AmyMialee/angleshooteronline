#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "NetworkPair.h"

NetworkPair::NetworkPair(SocketHolder& socketHolder, PortedIP pip) : socketHolder(socketHolder), pip(pip) {
	auto packet = NetworkProtocol::PING->getPacket();
	this->send(packet);
	this->lastResponse.restart();
}

void NetworkPair::update() {
	const auto now = std::chrono::steady_clock::now();
	for (auto& pair : sentPackets | std::views::values) {
		if (auto& [packet, timestamp] = pair; now - timestamp > resendInterval) {
			sendPacketInternal(packet);
			timestamp = now;
		}
	}
}

void NetworkPair::send(sf::Packet& packet) {
	const auto bytes = static_cast<const uint8_t*>(packet.getData());
	const auto type = bytes[0];
	if (const auto id = PacketIdentifier::fromId(bytes[0]); id->isReliable()) {
		const auto body = static_cast<const uint8_t*>(packet.getData()) + 1;
		const auto bodySize = packet.getDataSize() - 1;
		sf::Packet updated;
		updated << type;
		const auto sequence = this->getNextSequence();
		updated << sequence;
		updated.append(body, bodySize);
		packet = updated;
		sentPackets[sequence] = {packet, std::chrono::steady_clock::now()};
	}
	sendPacketInternal(packet);
}

void NetworkPair::acceptAcknowledgment(uint32_t sequence) {
	this->sentPackets.erase(sequence);
}

void NetworkPair::startRoundTripTimer() {
	this->roundTripTimer.restart();
}

float NetworkPair::stopRoundTripTimer() {
	this->rtt = this->roundTripTimer.getElapsedTime().asSeconds();
	return this->rtt;
}

float NetworkPair::getTimeoutRemaining() const {
	return AngleShooterCommon::TIMEOUT - this->lastResponse.getElapsedTime().asSeconds();
}

void NetworkPair::resetTimeout() {
	this->lastResponse.restart();
}

uint32_t NetworkPair::getAcknowledgedSequence() const {
	return this->acknowledgedSequence;
}

bool NetworkPair::setAcknowledgedSequence(uint32_t sequence) {
	if (this->acknowledgedSequence != sequence) return false;
	this->acknowledgedSequence++;
	return true;
}

uint32_t NetworkPair::getNextSequence() {
	return this->nextSequence++;
}

bool NetworkPair::shouldDisconnect() const {
	return this->lastResponse.getElapsedTime().asSeconds() > AngleShooterCommon::TIMEOUT;
}

PortedIP NetworkPair::getPortedIP() const {
	return pip;
}

void NetworkPair::sendPacketInternal(sf::Packet& packet) {
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = socketHolder.getSocket().send(packet, pip.ip, pip.port);
	if (status != sf::Socket::Status::Done) Logger::error("Send Error: " + pip.toString());
}