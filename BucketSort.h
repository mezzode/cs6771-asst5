#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>
#include <atomic>

struct BucketSort {
    // vector of numbers
    std::vector<unsigned int> numbersToSort;

    std::atomic<unsigned int> threadCount{1};
    unsigned int numCores_;

    using iterator = decltype(numbersToSort.begin());

    void sort(unsigned int numCores);

    bool radixSort(const iterator& start, const iterator& end, const unsigned int& power);
};

#endif /* BUCKET_SORT_H */
