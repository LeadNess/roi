#include "configurator.hpp"

int main(int argc, char const *argv[]) {
	args::parse<MapReduceAlgorithmArgs>(argc, argv);
	return 0;
}

