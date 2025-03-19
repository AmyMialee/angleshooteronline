#include "PreCompiledClient.h"
#include "Label.h"

Label::Label(const std::string& text) : text(FontHolder::get().getDefault(), text, 16) {}

void Label::setText(const std::string& string) {
    this->text.setString(string);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(text, states);
}