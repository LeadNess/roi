#include "configurator.hpp"

int main(int argc, char const *argv[]) {
    args::parse<AlgorithmArgs>(argc, argv);
    return 0;
}
