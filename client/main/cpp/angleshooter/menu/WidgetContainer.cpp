#include "PreCompiledClient.h"
#include "WidgetContainer.h"

WidgetContainer::WidgetContainer() : selectedChild(-1) {}

void WidgetContainer::pack(const std::shared_ptr<Widget>& widget) {
    if (!widget) {
        Logger::error("WidgetContainer::pack: widget is null");
        return;
    }
    children.emplace_back(widget);
    if (!hasSelection() && widget->canBeSelected()) select(children.size() - 1);
}

void WidgetContainer::clear() {
    children.clear();
}

void WidgetContainer::handleEvent(const sf::Event& event) {
    if (selectedChild < 0 || selectedChild >= static_cast<int>(children.size())) Logger::error("Invalid selected child: " + selectedChild);
    if (hasSelection() && children[selectedChild]->isPressed()) {
        children[selectedChild]->handleEvent(event);
    } else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::W || keyPressed->scancode == sf::Keyboard::Scan::Up) {
            selectPrevious();
        } else if (keyPressed->scancode == sf::Keyboard::Scan::S || keyPressed->scancode == sf::Keyboard::Scan::Down) {
            selectNext();
        } else if (keyPressed->scancode == sf::Keyboard::Scan::Enter || keyPressed->scancode == sf::Keyboard::Scan::Space) {
            if (hasSelection()) children[selectedChild]->setPressed(true);
        }
    }
}

void WidgetContainer::tick(float deltaTime) {
    for (const auto& child : children) child->tick(deltaTime);
}

void WidgetContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    for (const auto& child : children) target.draw(*child, states);
}

bool WidgetContainer::hasSelection() const {
    return selectedChild >= 0;
}

int WidgetContainer::getSelectedChild() const {
    return selectedChild;
}

void WidgetContainer::select(std::size_t index) {
    if (index < children.size() && children[index]->canBeSelected()) {
        if (hasSelection()) children[selectedChild]->setSelected(false);
        children[index]->setSelected(true);
        selectedChild = static_cast<int>(index);
    }
}

void WidgetContainer::selectNext() {
    if (!hasSelection()) return;
    auto next = selectedChild;
    do {
        next = (next + 1) % children.size();
    } while (!children[next]->canBeSelected());
    select(next);
}

void WidgetContainer::selectPrevious() {
    if (!hasSelection()) return;
    auto prev = selectedChild;
    do {
        prev = (prev + children.size() - 1) % static_cast<int>(children.size());
    } while (!children[prev]->canBeSelected());
    select(prev);
}