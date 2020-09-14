#include "configurator.hpp"

int main(int argc, char const *argv[]) {
    args::parse<GeneratorArgs>(argc, argv);
}
