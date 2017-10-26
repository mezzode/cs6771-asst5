#include "BucketSort.h"
#include <cmath>
#include <future>
#include <algorithm>

#include <iostream>

int getDigit(const unsigned int& n, const unsigned int& power) {
    if (n < std::pow(10, power)) {
        return -1; // no more digits
    }
    return static_cast<int>(n / std::pow(10, power)) % 10;
}

unsigned int getPower(unsigned int n) {
    unsigned int i = 0;
    while (n > 9) {
        n /= 10;
        ++i;
    }
    return i;
}
// is this a good idea?
// prolly could cache this?

std::vector<unsigned int> radixSort(std::vector<unsigned int> v, const unsigned int& power) {
    std::cout << power << "\n";
    if (v.empty()) {
        return std::vector<unsigned int>();
    } else if (v.size() == 1) {
        return v;
    }
    std::vector<std::vector<unsigned int>> buckets(10); // 1 bucket for each digit
    std::vector<unsigned int> endBucket;
    for (const auto& n : v) {
        const auto result = getDigit(n, getPower(n) - power);
        if (result == -1) {
            endBucket.emplace_back(n);
        } else {
            // add number to bucket
            buckets.at(result).emplace_back(n);
        }
    }

    const unsigned int nextPower = power + 1;
    std::vector<std::future<std::vector<unsigned int>>> sortedBuckets;
    // for every bucket, create new thread and do radix sort again
    for (std::vector<unsigned int> bucket : buckets) {
        if (bucket.size() > 0) { // no need to sort if 0 or 1 things in bucket
            sortedBuckets.emplace_back(std::async(
                std::launch::async,
                [] (std::vector<unsigned int> bucket, const unsigned int& power) {
                    return radixSort(bucket, power);
                },
                bucket,
                nextPower
            ));
        }
    }

    if (sortedBuckets.empty()) {
        return std::vector<unsigned int>();
    }
    return std::accumulate(
        sortedBuckets.begin(),
        sortedBuckets.end(),
        endBucket,
        [] (std::vector<unsigned int> a, std::future<std::vector<unsigned int>>& b) {
            auto v = b.get();
            a.insert(a.end(), v.begin(), v.end());
            return a;
        }
    );
    // wait until all spawned threads return, then append results (tho if done in place do nothing)
    // return appended buckets
}

// theres an issue if duplicates where infinite looping since bucket always has more than 1 thing in it
// deal with what happens if power becomes larger than number of digits?

void BucketSort::sort(unsigned int numCores) {
    // radix sort
    // bucket by digit and recursively do same for each bucket and append sorted buckets
    // numbersToSort
    // can parallelise sorting of each bucket
    // each sort would create separate threads for each bucket it spawns?

    numbersToSort = radixSort(numbersToSort, 0);

    // to minimise space usage could use iterators to track bucket borders i.e. where to insert elems
    // i.e. rather than a separate data structure, just removing and reinserting into vector
    // so have map of digits to iterators so given the digit, know where to insert the thingo
    // if doing this way, would define radix sort using begin and end iterators which would indicate 
    // the start and end of the bucket
    // although may need to be careful of iterator invalidation...
    // should be fine if only using iterators inside the bucket?
    // but what about end? since would be part of a different bucket so may be changed during since different thread?
    // one way to do this is to have it be last iterator in bucket instead i.e. --end
}
// in place sort?
// start and end iterators and recursive lock? or just copy section of vector?
