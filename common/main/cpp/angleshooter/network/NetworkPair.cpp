#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "NetworkPair.h"

NetworkPair::NetworkPair(SocketHolder& socketHolder, PortedIP& pip) : socketHolder(socketHolder), pip(pip) {
	auto packet = NetworkProtocol::PING->getPacket(this);
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
	if (const auto bytes = static_cast<const uint8_t*>(packet.getData()); PacketIdentifier::fromId(bytes[0])->isReliable()) {
		uint32_t sequence;
		std::memcpy(&sequence, bytes + 1, sizeof(uint32_t));
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

bool NetworkPair::setAcknowledgedSequence(uint32_t sequence) {
	if (this->acknowledgedSequence != sequence - 1) return false;
	this->acknowledgedSequence = sequence;
	return true;
}

uint32_t NetworkPair::getNextSequence() {
	return this->nextSequence++;
}

bool NetworkPair::shouldDisconnect() const {
	return this->lastResponse.getElapsedTime().asSeconds() > AngleShooterCommon::TIMEOUT;
}

PortedIP* NetworkPair::getPortedIP() const {
	return &pip;
}

void NetworkPair::sendPacketInternal(sf::Packet& packet) {
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = socketHolder.getSocket().send(packet, pip.ip, pip.port);
	if (status != sf::Socket::Status::Done) Logger::error("Send Error: " + pip.toString());
}