#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>

struct BucketSort {
    // vector of numbers
    std::vector<unsigned int> numbersToSort;

    using iterator = typename std::vector<std::vector<unsigned int>>::iterator;

    // std::atomic<int> threads = 0;

    void sort(unsigned int numCores);

    bool radixSort(const iterator& start, const iterator& end, const unsigned int& power);
};

#endif /* BUCKET_SORT_H */
