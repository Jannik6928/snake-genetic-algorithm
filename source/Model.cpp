#include "Model.h"
double Node::activate(double x, const char* type) {
	if (type == "sigmoid") { return 1.0 / (1.0 + exp(0.0 - x)); }
	if (type == "tanh") { return tanh(x); }
	if (type == "relu") {
		if (x > 0) return x;
		else return 0.0;
	}
	return x; // defaults to linear activation
}

void Model::initialize(vector <int>& lengthVector) {
	network.clear();
	for (unsigned i = 0; i < lengthVector.size(); i++) {
		vector <Node> newLayer;
		for (int j = 0; j < lengthVector[i]; j++) {
			Node newNeuron;
			newLayer.push_back(newNeuron);
		}
		network.push_back(newLayer);
	}
	for (unsigned i = 1; i < network.size(); i++) {
		for (unsigned j = 0; j < network[i].size(); j++) {
			for (unsigned k = 0; k < network[i - 1].size(); k++) {
				double r = -1.0 + 2 * ((double)rand() / RAND_MAX);
				network[i][j].input_weight.push_back(r);
			}
		}
	}
}
void Model::forward_propagate() {
	for (unsigned i = 1; i < network.size(); i++) {
		for (unsigned j = 0; j < network[i].size(); j++) {
			for (unsigned k = 0; k < network[i][j].input_weight.size(); k++) {
				network[i][j].value += network[i][j].input_weight[k] * network[i - 1][k].value;
			}
			network[i][j].value = network[i][j].activate(network[i][j].value, "sigmoid");
		}
	}
}
void Model::reset() {
	fitness = 0.0;
	for (unsigned i = 0; i < network.size(); i++) {
		for (unsigned j = 0; j < network[i].size(); j++) {
			network[i][j].value = 0.0;
		}
	}
}
unsigned Model::get_max_output() {
	unsigned max_index = 0;
	for (unsigned i = 0; i < network.back().size(); i++) {
		if (network.back()[i].value > network.back()[max_index].value) max_index = i;
	}
	return max_index;
}
void Model::extract_snake_data() {
	network.front()[0].value = snake.is_clear(0);
	network.front()[1].value = snake.is_clear(1);
	network.front()[2].value = snake.is_clear(-1);

	Vector2 difference = snake.position.front() - snake.fruit.position;
	network.front()[3].value = (atan2(difference.y, difference.x) + snake.direction * M_PI / 2) / M_PI;
	while (network.front()[3].value > 1) network.front()[3].value -= 2.0;
	while (network.front()[3].value <= -1) network.front()[3].value += 2.0;
}
Model Model::cross(Model& parent, int cross_point_count) {
	Model offspring;
	vector <int> offspring_length;
	for (unsigned i = 0; i < network.size(); i++) {
		offspring_length.push_back(network[i].size());
	}
	offspring.initialize(offspring_length);
	for (unsigned i = 0; i < cross_point_count; i++) {
		for (unsigned j = 1; j < network.size(); j++) { // iterates through layers
			for (unsigned k = 0; k < network[j].size(); k++) { // iterates through nodes of each layer
				unsigned slice_index = rand() % network[j][k].input_weight.size();
				for (unsigned a = 0; a < slice_index; a++) {
					offspring.network[j][k].input_weight[a] = network[j][k].input_weight[a];
				}
				for (unsigned b = slice_index; b < network[j][k].input_weight.size(); b++) {
					offspring.network[j][k].input_weight[b] = parent.network[j][k].input_weight[b];
				}
			}
		}
	}
	return offspring;
}

void Population::populate_initial(unsigned population_size) {
	population.clear();
	vector<Model> initial_population;
	while (initial_population.size() != population_size) {
		Model new_network;
		new_network.initialize(length);
		initial_population.push_back(new_network);
	}
	population.push_back(initial_population);
}
double Population::total_fitness(vector <Model> generation) {
	double total_fitness = 0.0;
	for (unsigned i = 0; i < generation.size(); i++) {
		total_fitness += generation[i].fitness;
	}
	return total_fitness;
}