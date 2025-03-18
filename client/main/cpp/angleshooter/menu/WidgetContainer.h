#pragma once
#include "Widget.h"

class WidgetContainer final : public Widget {
	std::vector<Pointer> children;
	int selectedChild;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	typedef std::shared_ptr<WidgetContainer> Pointer;

	WidgetContainer();
	void pack(const Widget::Pointer& widget);
	void clear();
	void handleEvent(const sf::Event& event) override;
	void tick(float deltaTime) override;
	[[nodiscard]] int getSelectedChild() const;
	void select(std::size_t index);
	[[nodiscard]] bool hasSelection() const;
	void selectNext();
	void selectPrevious();
};