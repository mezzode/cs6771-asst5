#include "BucketSort.h"
#include <cmath>
#include <future>
#include <algorithm>

#include <iostream>

unsigned int getDigit(const unsigned int& n, const unsigned int& power) {
    return static_cast<unsigned int>(n / std::pow(10, power)) % 10;
}

unsigned int getPower(unsigned int n) {
    unsigned int i = 0;
    while (n > 9) {
        n /= 10;
        ++i;
    }
    return i;
}

void BucketSort::radixSort(const iterator& begin, const iterator& end, const unsigned int& power) {
    if (begin == end) {
        return;
    }
    std::vector<unsigned int> finishedBucket; // bucket for elements which cannot be further done
    std::vector<std::vector<unsigned int>> buckets(10); // one for each digit

    for (auto it = begin; it != end; ++it) {
        const auto& num = *it;
        if (num < std::pow(10, power)) {
            // no more digits
            finishedBucket.emplace_back(num);
        } else {
            const auto result = getDigit(num, getPower(num) - power);
            buckets.at(result).emplace_back(num);
        }
    }

    const unsigned int nextPower = power + 1;
    auto it = std::copy(finishedBucket.begin(), finishedBucket.end(), begin);
    std::vector<std::future<void>> sortedBuckets;
    for (auto bucket : buckets) {
        if (bucket.size() == 0) {
            continue; // skip empty buckets
        }

        const auto bucketBegin = it;
        it = std::copy(bucket.begin(), bucket.end(), it);
        const auto bucketEnd = it;

        if (bucket.size() == 1) {
            continue; // no need to sort
        }

        // only spawn new thread if available
        auto policy = std::launch::deferred;
        {
            std::lock_guard<std::mutex> lg{threadCountLock};
            if (threadCount < numCores_) {
                policy = std::launch::async;
                ++threadCount;
            }
        }
        sortedBuckets.emplace_back(std::async(
            policy,
            [this] (const iterator& begin, const iterator& end, const unsigned int& power) {
                return radixSort(begin, end, power);
            },
            bucketBegin,
            bucketEnd,
            nextPower
        ));
    }

    for (std::future<void>& b : sortedBuckets) {
        b.get();
    }
}

void BucketSort::sort(unsigned int numCores) {
    numCores_ = numCores;
    radixSort(numbersToSort.begin(), numbersToSort.end(), 0);
}
