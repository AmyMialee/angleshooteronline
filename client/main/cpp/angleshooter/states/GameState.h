#pragma once

class GameState final : public State {
public:
	static const Identifier GAME_ID;
	GameState();
	void init() override;
	void destroy() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();

	void updateBroadcastMessage(float deltaTime);
	void handlePacket(int packetType, sf::Packet& packet);

	std::map<int, std::function<void(int packet_type, sf::Packet& packet)>> packetHandlers;

	sf::TcpSocket socket;
	bool connected;

	std::vector<std::string> broadcasts;
	sf::Clock failedConnectionClock;

	bool activeState;
	bool gameStarted;
	float clientTimeout;
	float timeSinceLastPacket;
};