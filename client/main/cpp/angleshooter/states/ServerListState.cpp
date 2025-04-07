#include "PreCompiledClient.h"
#include "ServerListState.h"

const Identifier ServerListState::SERVER_LIST_ID("servers");

ServerListState::ServerListState() : remainingPorts(FontHolder::getInstance().getDefault()) {}

void ServerListState::init() {
	gui.clear();

	const auto optionsIp = std::make_shared<Button>();
	optionsIp->setPosition({80.f, 364.f});
	auto ipText = [&](const std::string& ip) { return "Options: " + ip; };
	optionsIp->setText(ipText(OptionsManager::get().getIp()));
	optionsIp->setCallback([this] {
		AngleShooterClient::get().foundAddress = sf::IpAddress::resolve(OptionsManager::get().getIp()).value();
		requestStackClear();
		requestStackPush(GameState::GAME_ID);
	});
	gui.pack(optionsIp);

	const auto localIp = std::make_shared<Button>();
	localIp->setPosition({80.f, 400.f});
	localIp->setText("localhost: 127.0.0.1");
	localIp->setCallback([this] {
		AngleShooterClient::get().foundAddress = sf::IpAddress(127, 0, 0, 1);
		requestStackClear();
		requestStackPush(GameState::GAME_ID);
	});
	gui.pack(localIp);

	std::thread receiverThread(&ServerListState::seekThread, this);
    receiverThread.detach();
	const auto backButton = std::make_shared<Button>();
	backButton->setPosition({300.f, 475.f});
	backButton->setText("Back");
	backButton->setCallback([this] { requestStackPop(); });
	gui.pack(backButton);
}

void ServerListState::seekThread() {
	std::vector<sf::IpAddress> activeServers;
	const auto localOptional = sf::IpAddress::getLocalAddress();
	if (!localOptional) {
		Logger::error("Failed to get local address");
		return;
	}
	const auto localIp = localOptional.value().toString();
	const auto baseIp = localIp.substr(0, localIp.find_last_of('.') + 1);
	Logger::info("Seeking through " + baseIp + "1 to " + baseIp + "255");
	auto remaining = 255;
	remainingPorts.setPosition({5, 5});
	remainingPorts.setString("Refreshing Server List...");
	remainingPorts.setScale({0.5f, 0.5f});
	auto index = 2;
	for (auto i = 1; i <= 255; ++i) {
		if (auto targetIp = sf::IpAddress::resolve(baseIp + std::to_string(i))) {
			sf::TcpSocket socket;
			socket.setBlocking(true);
			const auto status = socket.connect(targetIp.value(), AngleShooterCommon::PORT, sf::seconds(0.15f));
			if (StateManager::get().getStateId() != SERVER_LIST_ID) return;
			if (status != sf::Socket::Status::Done) continue;
			activeServers.push_back(targetIp.value());
			socket.disconnect();
			constexpr auto offset = 36.f;
			const auto button = std::make_shared<Button>();
			button->setPosition({80.f, 400.f - offset * index++});
			auto ipText = [&](const std::string& ip) { return "IP: " + ip; };
			button->setText(ipText(targetIp.value().toString()));
			button->setCallback([this, targetIp] {
				AngleShooterClient::get().foundAddress = targetIp.value();
				requestStackClear();
				requestStackPush(GameState::GAME_ID);
			});
			if (StateManager::get().getStateId() != SERVER_LIST_ID) return;
			gui.pack(button);
		}
		remaining--;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (StateManager::get().getStateId() != SERVER_LIST_ID) return;
	}
	remainingPorts.setString("");
	Logger::info("Found " + std::to_string(activeServers.size()) + " servers");
}

void ServerListState::loadAssets() {}

void ServerListState::render(float deltaTime) {
	static sf::Sprite background(TextureHolder::getInstance().get(MenuState::MENU_TEXTURE));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		background.setColor({200, 255, 255, 255});
		Util::centre(background);
		background.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
		background.setScale({2.f, 2.f});
	});
	auto& texture = AngleShooterClient::get().renderTexture;
	texture.draw(background);
	texture.draw(gui);
	texture.draw(this->remainingPorts);
}

bool ServerListState::shouldRenderNextState() const {
	return false;
}

bool ServerListState::tick(float deltaTime) {
	gui.tick(deltaTime);
	return false;
}

bool ServerListState::handleEvent(const sf::Event& event) {
	if (const auto keybinding = InputManager::get().getSelectedKeybinding()) {
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode != sf::Keyboard::Scan::Escape && keyPressed->scancode != sf::Keyboard::Scan::Enter && keyPressed->scancode != sf::Keyboard::Scan::Space) {
				keybinding->rebind(keyPressed->scancode);
				InputManager::get().setSelectedKeybinding(nullptr);
				const auto selected = gui.getSelectedChild();
				this->init();
				gui.setSelected(selected);
				gui.selectNext();
				gui.selectPrevious();
				return false;
			}
		}
	}
	gui.handleEvent(event);
	return false;
}

Identifier ServerListState::getId() {
	return SERVER_LIST_ID;
}

Identifier ServerListState::getStateId() {
	return SERVER_LIST_ID;
}