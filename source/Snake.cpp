#include "Snake.h"

void Food::move(int lower_x, int lower_y, int upper_x, int upper_y) {
	position.x = (rand() % (upper_x - lower_x)) * 10 - lower_x;
	position.y = (rand() % (upper_y - lower_y)) * 10 - lower_y;
}
void Food::draw(sf::RenderWindow & window, sf::RectangleShape & square) {
	square.setFillColor(sf::Color::Red);
	square.setPosition(position.x, position.y);
	window.draw(square);
}
Food::Food() {
	move(0, 0, 50, 50);
}

Snake::Snake() {
	position.push_back(Vector2(250, 250));
	position.push_back(Vector2(10000, 10000));
	position.push_back(Vector2(10000, 10000));
}
void Snake::move() {
	for (unsigned i = position.size() - 1; i > 0; i--) { position[i] = position[i - 1]; }
	if (direction == 0) position.front().y -= 10;
	if (direction == 1) position.front().x -= 10;
	if (direction == 2) position.front().y += 10;
	if (direction == 3) position.front().x += 10;
}
void Snake::draw(sf::RenderWindow & window, sf::RectangleShape & square) {
	square.setFillColor(sf::Color::White);
	for (unsigned i = 0; i < position.size(); i++) {
		square.setPosition(position[i].x, position[i].y);
		window.draw(square);
	}
}
bool Snake::touching_food() {
	if (position.front() == fruit.position) return 1;
	return 0;
}
bool Snake::touching_wall(int lower_x, int lower_y, int upper_x, int upper_y) {
	if (position.front().x < lower_x) return 1;
	if (position.front().x >= upper_x) return 1;
	if (position.front().y < lower_y) return 1;
	if (position.front().y >= upper_y) return 1;
	return 0;
}
bool Snake::touching_self() {
	for (unsigned i = 1; i < position.size(); i++) {
		if (position.front() == position[i]) return 1;
	}
	return 0;
}
bool Snake::is_clear(int shift) {
	unsigned newDirection = (direction + shift) % 4;
	Vector2 checkedPosition = position.front();

	if (newDirection == 0) checkedPosition.y -= 10;
	else if (newDirection == 1) checkedPosition.x -= 10;
	else if (newDirection == 2) checkedPosition.y += 10;
	else if (newDirection == 3) checkedPosition.x += 10;

	for (unsigned i = 1; i < position.size(); i++) { if (position[i] == checkedPosition) return 0; }
	if (checkedPosition.x >= 500 || checkedPosition.x < 0) return 0;
	if (checkedPosition.y >= 500 || checkedPosition.y < 0) return 0;
	return 1;
}
double Snake::locate_food_vertical() { // returns cosine of angle btw. snake velocity and difference of snake and fruit positions
	Vector2 difference = position.front() - fruit.position;
	Vector2 snake_velocity;
	if (direction == 0) snake_velocity = Vector2(0, -1);
	if (direction == 1) snake_velocity = Vector2(-1, 0);
	if (direction == 2) snake_velocity = Vector2(0, 1);
	if (direction == 3) snake_velocity = Vector2(1, 0);
	return (snake_velocity.dot_product(difference) / snake_velocity.magnitude() / difference.magnitude());
}
double Snake::locate_food_horizontal() { // returns cosine (of perpendicular movement vector)
	Vector2 difference = position.front() - fruit.position;
	Vector2 snake_velocity;
	if (direction == 0) snake_velocity = Vector2(-1, 0);
	if (direction == 1) snake_velocity = Vector2(0, -1);
	if (direction == 2) snake_velocity = Vector2(1, 0);
	if (direction == 3) snake_velocity = Vector2(0, 1);
	return (snake_velocity.dot_product(difference) / snake_velocity.magnitude() / difference.magnitude());
}


