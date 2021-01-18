#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Snake.h"
#include <vector>
#include <cmath>
#include "Model.h"
using namespace std;

int main() {
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(500, 500), "Snake AI");
	window.setFramerateLimit(15);
	sf::RectangleShape square(sf::Vector2f(10, 10));

	bool draw = true;

	vector <int> length;
	length.push_back(4);
	length.push_back(8);
	length.push_back(8);
	length.push_back(3);

	Population pool;
	pool.length = length;
	pool.populate_initial(100);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::N) window.setFramerateLimit(15);
				if (event.key.code == sf::Keyboard::M) window.setFramerateLimit(0);
				if (event.key.code == sf::Keyboard::B) if (draw == false) draw = true; else draw = false;
			}
		}

		double distance = (pool.population.back()[pool.current_index].snake.position.front() -
			pool.population.back()[pool.current_index].snake.fruit.position).magnitude();

		pool.population.back()[pool.current_index].extract_snake_data();
		pool.population.back()[pool.current_index].forward_propagate();

		if (pool.population.back()[pool.current_index].get_max_output() == 1) 
			pool.population.back()[pool.current_index].snake.direction = (pool.population.back()[pool.current_index].snake.direction + 5) % 4;
		if (pool.population.back()[pool.current_index].get_max_output() == 2)
			pool.population.back()[pool.current_index].snake.direction = (pool.population.back()[pool.current_index].snake.direction + 3) % 4;

		pool.population.back()[pool.current_index].snake.move();
		pool.population.back()[pool.current_index].snake.steps_left--;

		if (pool.population.back()[pool.current_index].snake.touching_food() == 1) {
			pool.population.back()[pool.current_index].fitness += 1.2;
			pool.population.back()[pool.current_index].snake.steps_left += 100;
			pool.population.back()[pool.current_index].snake.fruit.move(0, 0, 50, 50);
			pool.population.back()[pool.current_index].snake.position.push_back(Vector2(10000, 10000));
		}
		if (pool.population.back()[pool.current_index].snake.touching_wall(0, 0, 500, 500) == 1
			|| pool.population.back()[pool.current_index].snake.touching_self() == 1
			|| pool.population.back()[pool.current_index].snake.steps_left < 0) {
			pool.population.back()[pool.current_index].fitness -= 1.5;
			pool.current_index++;
			if (pool.current_index > pool.population.back().size() - 1) {
				pool.current_index = 0;
				vector <Model> new_generation;
				vector <Model> adjusted_generation = pool.adjust_generation();
				while (new_generation.size() != 100) {
					Model new_network;
					unsigned index_1 = pool.roulette_selection(adjusted_generation);
					unsigned index_2 = pool.roulette_selection(adjusted_generation);
					new_network = pool.population.back()[index_1].cross(pool.population.back()[index_2], 1);
					new_generation.push_back(new_network);
				}
				pool.population.clear();
				pool.population.push_back(new_generation);
				pool.mutate(0.5);
			}
		}

		double new_distance = (pool.population.back()[pool.current_index].snake.position.front() -
			pool.population.back()[pool.current_index].snake.fruit.position).magnitude();

		if (new_distance < distance) pool.population.back()[pool.current_index].fitness += 0.1;
		else pool.population.back()[pool.current_index].fitness -= 0.2;

		if (draw == true) {
			window.clear();
			pool.population.back()[pool.current_index].snake.draw(window, square);
			pool.population.back()[pool.current_index].snake.fruit.draw(window, square);
			window.display();
		}
	}

	return 0;
}
