#include "Data.h"

void Data::clear() {
    cpu_usage = 0.0;
    mem_usage = 0.0;
    gpu_usage = 0.0;
}

void Data::merge(const Data& other) {
    cpu_usage += other.cpu_usage;
    mem_usage += other.mem_usage;
    gpu_usage += other.gpu_usage;
}
