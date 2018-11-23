#include "pch.h"

int main(int, char**) {
    #pragma omp parallel
    {
        printf("Hdeello\n");
    }
    return 0;
}
