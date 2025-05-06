#include "PreCompiledClient.h"

int main() {
	Logger::info("Client Start");
	sf::UdpSocket socket;
	auto status = socket.bind(sf::Socket::AnyPort);
	socket.setBlocking(false);
	const auto localIp = sf::IpAddress::getLocalAddress();
	const auto localPort = socket.getLocalPort();
	if (!localIp.has_value()) return -1;
	std::cout << "Client on " << localIp.value() << ":" << localPort << " ready. Type a message and Enter to ping.\n";
	const auto serverIp = sf::IpAddress::resolve("127.0.0.1");
	while (true) {
		std::optional<sf::IpAddress> sender;
		unsigned short port;
		if (sf::Packet packet; socket.receive(packet, sender, port) == sf::Socket::Status::Done) {
			std::string msg;
			packet >> msg;
			if (sender.has_value()) {
				std::cout << "Received from " << sender.value() << ":" << port << " -> " << msg << '\n';
			} else {
				std::cout << "Received from ???:" << port << " -> " << msg << '\n';
			}
		}
        if (std::cin.peek() != -1) {
			std::string userInput;
			std::getline(std::cin, userInput);
			if (!serverIp.has_value()) {
				std::cerr << "Failed to resolve server IP\n";
				continue;
			}
			std::ostringstream oss;
			oss << "ping: " << localIp.value().toString() << ":" << localPort << " - " << userInput;
			sf::Packet ping;
			ping << oss.str();
			auto status = socket.send(ping, serverIp.value(), AngleShooterCommon::PORT);
			std::cout << "Sent " << oss.str() << " to " << serverIp.value() << ":" << AngleShooterCommon::PORT << '\n';
		}
		sleep(sf::milliseconds(10));
	}
}