#include <stdio.h>
#include <omp.h>
#include <memory.h>
#include <math.h>

int main() {
    float data[] = {1.3, 2.9, 0.4, 0.3, 1.3, 2.4, 1.7, 0.4};
    int num_values = sizeof(data) / sizeof(data[0]);
    int bin_maxes[] = {2, 4};
    int numbins = 2;
    int bin_count[numbins];
    memset(bin_count, 0, sizeof(bin_count));

    int thread_count = fmin(num_values, 12);
#   pragma omp parallel num_threads(thread_count)
    {
        int sizet = num_values / omp_get_num_threads();
        int startd = omp_get_thread_num() * sizet;
        int localbin_count[numbins];
        memset(localbin_count, 0, sizeof(localbin_count));

        for (int i = startd; i < startd + sizet; i++) {
            int bin_min = 0;
            for (int j = 0; j < sizeof(bin_count) / sizeof(bin_count[0]); j++) {
                if (bin_min <= data[i] && data[i] < bin_maxes[j]) {
                    localbin_count[j]++;
                }
                bin_min += bin_maxes[j];
            }
        }

#   pragma omp critical
        {
            for (int i = 0; i < numbins; i++) {
                bin_count[i]+=localbin_count[i];
            }
        }
    }

    printf("%d - %d : %d\n ", 0, bin_maxes[0], bin_count[0]);
    for (int k = 1; k < sizeof(bin_count) / sizeof(bin_count[0]); k++) {
        printf("%d - %d : %d\n ", bin_maxes[k - 1], bin_maxes[k], bin_count[k]);
    }

    return 0;
}