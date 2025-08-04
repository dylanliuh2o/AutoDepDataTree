#include <iostream>

#include <tabulate/table.hpp>

int main()
{
    tabulate::Table table;
    table.add_row({"PID", "Level", "CPU %", "Memory", "CPU % (local)", "Memory (local)"});
    table.add_row({
        "0",
        "0",
        "100.0",
        "2000",
        "100.0",
        "2000"
    });
    std::cout << table << std::endl;

    return 0;
}
