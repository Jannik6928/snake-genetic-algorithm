#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include "Snake.h"
using namespace std;
struct Node {
	double bias = 0.0;
	double value = 0.0;
	vector <double> input_weight;
	double activate(double x, const char* type);
};
struct Model {
	vector <vector <Node>> network;
	double fitness = 0.0;
	Snake snake;
	void initialize(vector <int>& lengthVector);
	void forward_propagate();
	void reset();
	unsigned get_max_output();
	void extract_snake_data();
	Model cross(Model& parent, int cross_point_count);
	Model() {
		Snake snake;
	}
};
struct Population {
	vector <vector <Model>> population;
	int current_index = 0;
	vector <int> length;
	void populate_initial(unsigned population_size);
	double total_fitness(vector <Model> generation);
	unsigned lowest_fitness_index() {
		unsigned index = 0;
		for (unsigned i = 0; i < population.back().size(); i++) {
			if (population.back()[i].fitness < population.back()[index].fitness) index = i;
		}
		return index;
	}
	vector <Model> adjust_generation() {
		vector <Model> adjusted_generation = population.back();
		double lowest_fitness = adjusted_generation[lowest_fitness_index()].fitness;
		for (unsigned i = 0; i < adjusted_generation.size(); i++) {
			adjusted_generation[i].fitness -= lowest_fitness;
		}
		return adjusted_generation;
	}
	unsigned roulette_selection(vector <Model> parent_generation) {
		double total = total_fitness(parent_generation);
		double threshold = (double)rand() / RAND_MAX;
		double probability_selected = 0.0;
		for (unsigned i = 0; i < parent_generation.size(); i++) {
			probability_selected += parent_generation[i].fitness / total;
			if (probability_selected > threshold) return i;
		}
		return 0;
	}
	void mutate(double mutation_chance) {
		for (unsigned i = 0; i < population.back().size(); i++) {
			for (unsigned j = 0; j < population.back()[i].network.size(); j++) {
				for (unsigned k = 0; k < population.back()[i].network[j].size(); k++) {
					for (unsigned l = 0; l < population.back()[i].network[j][k].input_weight.size(); l++) {
						double r = (double)rand() / RAND_MAX;
						if (r < mutation_chance) {
							population.back()[i].network[j][k].input_weight[l] += 2 * ((double)rand() / RAND_MAX) - 1;
						}
					}
				}
			}
		}
	}
};