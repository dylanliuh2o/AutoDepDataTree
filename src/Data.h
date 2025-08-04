#ifndef DATA_H
#define DATA_H

class Data {
public:
    void clear();
    void merge(const Data& other);

    double cpu_usage { 0.0 };
    double mem_usage { 0.0 };
    double gpu_usage { 0.0 };
} ;

#endif // DATA_H