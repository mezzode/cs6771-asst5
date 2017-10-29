#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>

struct BucketSort {
    // vector of numbers
    std::vector<unsigned int> numbersToSort;

    using iterator = decltype(numbersToSort.begin());

    void sort(unsigned int numCores);

    bool radixSort(const iterator& start, const iterator& end, const unsigned int& power);
};

#endif /* BUCKET_SORT_H */
