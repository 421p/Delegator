#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int argc, const char *const argv[])
{
    return Catch::Session().run(argc, argv);
}