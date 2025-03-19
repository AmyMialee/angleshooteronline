#pragma once

class WidgetContainer final : public Widget {
	std::vector<std::shared_ptr<Widget>> children;
	int selectedChild;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
public:
	WidgetContainer();
	void pack(const std::shared_ptr<Widget>& widget);
	void clear();
	void handleEvent(const sf::Event& event) override;
	void tick(float deltaTime) override;
	[[nodiscard]] int getSelectedChild() const;
	void select(std::size_t index);
	[[nodiscard]] bool hasSelection() const;
	void selectNext();
	void selectPrevious();
};