#include "PreCompiledClient.h"
#include "Button.h"

Button::Button() :
    defaultTextureRef(TextureHolder::get().get(defaultTexture)),
    selectedTextureRef(TextureHolder::get().get(selectedTexture)),
    pressedTextureRef(TextureHolder::get().get(pressedTexture)),
    sprite(defaultTextureRef),
    text(FontHolder::get().getDefault(), "", 16),
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
        AudioManager::get().playSound(buttonSound);
    } else {
        if (!isToggle) return;
        sprite.setTexture(this->isSelected() ? selectedTextureRef : defaultTextureRef);
        AudioManager::get().playSound(buttonSound, 1.0f, 0.8f);
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    target.draw(text, states);
}