#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <string>
#include <vector>

void ping_a();
void ping_b();

int hpx_main(int argc, char **argv)
{
    std::cout << "main\n";
    ping_a();
    // ping_b();
    std::cout << "finalize\n";
    return hpx::finalize();
}

int main(int argc, char **argv)
{
    // We want HPX to run hpx_main() on all localities to avoid the
    // initial overhead caused by broadcasting the work from one to
    // all other localities:
    std::vector<std::string> config(1, "hpx.run_hpx_main!=1");

    return hpx::init(argc, argv, config);
}
