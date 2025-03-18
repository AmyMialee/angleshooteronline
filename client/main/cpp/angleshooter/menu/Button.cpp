#include "PreCompiledClient.h"
#include "Button.h"

Identifier Button::defaultTexture("buttonnormal.png");
Identifier Button::selectedTexture("buttonselected.png");
Identifier Button::pressedTexture("buttonpressed.png");
Identifier Button::buttonSound("ui_button_click.ogg");

Button::Button() :
    defaultTextureRef(ClientContext::get()->getTextureHolder()->get(defaultTexture)),
    selectedTextureRef(ClientContext::get()->getTextureHolder()->get(selectedTexture)),
    pressedTextureRef(ClientContext::get()->getTextureHolder()->get(pressedTexture)),
    sprite(defaultTextureRef),
    text(ClientContext::get()->getFontHolder()->getDefault(), "", 16),
    isToggle(false)
{
    const auto bounds = sprite.getLocalBounds();
    text.setPosition({bounds.size.x / 2, bounds.size.y / 2});
}

void Button::setCallback(const Callback& callback) {
    this->callback = callback;
}

void Button::setText(const std::string& text) {
    this->text.setString(text);
    const auto bounds = this->text.getLocalBounds();
    this->text.setOrigin({std::floor(bounds.position.x + bounds.size.x / 2.f), std::floor(bounds.position.y + bounds.size.y / 2.f)});
}

void Button::setToggle(bool toggle) {
    isToggle = toggle;
}

bool Button::canBeSelected() const {
    return true;
}

void Button::setSelected(bool selected) {
    if (this->isSelected() == selected) return;
    Widget::setSelected(selected);
    sprite.setTexture(selected ? selectedTextureRef : defaultTextureRef);
}

void Button::setPressed(bool pressed) {
    if (this->isPressed() == pressed) return;
    Widget::setPressed(pressed);
    if (pressed) {
        if (isToggle) sprite.setTexture(pressedTextureRef);
        if (callback) callback();
        if (!isToggle) setPressed(false);
        ClientContext::get()->getAudioManager()->playSound(buttonSound);
    } else {
        if (!isToggle) return;
        sprite.setTexture(this->isSelected() ? selectedTextureRef : defaultTextureRef);
        ClientContext::get()->getAudioManager()->playSound(buttonSound, 1.0f, 0.8f);
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    target.draw(text, states);
}