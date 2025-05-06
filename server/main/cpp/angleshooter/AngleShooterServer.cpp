#include "PreCompiledServer.h"

int main() {
	Logger::info("Server Start");
	sf::UdpSocket socket;
	auto status = socket.bind(AngleShooterCommon::PORT);
	socket.setBlocking(false);
	Logger::info("Server is running on port 54000. Waiting for pings...");
	while (true) { // Check for incoming packets
		std::optional<sf::IpAddress> sender;
		unsigned short port;
		if (sf::Packet packet; socket.receive(packet, sender, port) == sf::Socket::Status::Done) {
			std::string msg;
			packet >> msg;
			if (sender.has_value()) {
				Logger::info("Received from " + sender.value().toString() + ":" + std::to_string(port) + " -> " + msg);
			} else {
				Logger::info("Received from ???:" + std::to_string(port) + " -> " + msg);
			}
			if (msg.starts_with("ping")) {
				if (sender.has_value()) {
					std::ostringstream oss;
					oss << "pong: " << sender.value().toString() << ":" << port;
					sf::Packet reply;
					reply << oss.str();
					auto status = socket.send(reply, sender.value(), port);
					Logger::info("Sent " + oss.str() + " to " + sender.value().toString() + ":" + std::to_string(port));
				} else {
					Logger::info("No client to send pong to yet.");
				}
			}
		}
		sleep(sf::milliseconds(10));
	}
}