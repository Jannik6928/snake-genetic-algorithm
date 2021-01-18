#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

struct Vector2 {
	int x = 0, y = 0;
	bool operator==(Vector2 v) {
		if (v.x == x && v.y == y) return 1;
		else return 0;
	}
	void operator=(Vector2 v) {
		x = v.x;
		y = v.y;
	}
	Vector2 operator-(Vector2 v) {
		return Vector2((x - v.x), (y - v.y));
	}
	Vector2(int new_x, int new_y) {
		x = new_x;
		y = new_y;
	}
	Vector2() {
		x = 0;
		y = 0;
	}
	double magnitude() { return sqrt(x * x + y * y); }
	double dot_product(Vector2 v) { return (x * v.x + y * v.y); }
};

struct Food {
	Vector2 position;
	void move(int lower_x, int lower_y, int upper_x, int upper_y);
	void draw(sf::RenderWindow& window, sf::RectangleShape& square);
	Food();
};

using namespace std;
struct Snake {
	vector <Vector2> position;
	Food fruit;
	int direction = 3;
	int steps_left = 200;
	Snake();
	void move();
	void draw(sf::RenderWindow& window, sf::RectangleShape& square);
	bool touching_food();
	bool touching_wall(int lower_x, int lower_y, int upper_x, int upper_y);
	bool touching_self();
	bool is_clear(int shift);
	double locate_food_vertical();
	double locate_food_horizontal();
};