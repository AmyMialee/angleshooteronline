#include "PreCompiledClient.h"
#include "SplashState.h"

const Identifier SplashState::TITLE_ID("splash");
const Identifier SplashState::SPLASH_TEXTURE("sfml_logo.png");
float SplashState::totalTime = 4.f;

void SplashState::completeSplash() {
    this->requestStackPop();
    this->requestStackPush(MenuState::getId());
    if (!ClientContext::get()->getOptionsManager()->isOnboarded()) this->requestStackPush(OnboardingState::getId());
}

void SplashState::init() {
    fadeTime = 0;
}

void SplashState::loadAssets() {
    ClientContext::get()->getTextureHolder()->load(SPLASH_TEXTURE);
}

void SplashState::render(float deltaTime) {
    static sf::Sprite background(ClientContext::get()->getTextureHolder()->get(SPLASH_TEXTURE));
    static std::once_flag flag;
    std::call_once(flag, [&] {
        Util::centre(background);
        background.setPosition(ClientContext::get()->getRenderTexture()->getView().getSize() / 2.f);
    });
    const auto alpha = std::clamp(255 * std::pow(std::sin(Util::toRadians(160 * std::pow(fadeTime / totalTime, 2))), 2), 0., 255.);
    background.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
    const auto scale = .4f + .4f * fadeTime / totalTime;
    background.setScale({scale, scale});
    auto& texture = *ClientContext::get()->getRenderTexture();
    texture.draw(background);
}

bool SplashState::shouldRenderNextState() const {
    return false;
}

bool SplashState::tick(float deltaTime) {
    fadeTime += static_cast<float>(AngleShooterClient::timePerTick) + deltaTime;
    if (fadeTime >= totalTime) completeSplash();
    return false;
}

bool SplashState::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        completeSplash();
    }
    return false;
}

Identifier SplashState::getId() {
    return TITLE_ID;
}

Identifier SplashState::getStateId() {
    return TITLE_ID;
}