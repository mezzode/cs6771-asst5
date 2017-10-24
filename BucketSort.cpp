#include "BucketSort.h"
#include <cmath>
#include <future>
#include <algorithm>

#include <iostream>

unsigned int getDigit(const unsigned int& n, const unsigned int& power) {
    return static_cast<unsigned int>(n / std::pow(10, power)) % 10;
}

unsigned int getPower(unsigned int n) {
    std::cout << n << "\n";
    unsigned int i = 0;
    while (n > 9) {
        n /= 10;
        ++i;
    }
    std::cout << i << "\n";
    return i;
}
// is this a good idea?
// prolly could cache this?

std::vector<unsigned int> radixSort(std::vector<unsigned int> v, const unsigned int& power) {
    // std::cout << power << "\n";
    // std::cout << v.size() << "\n";
    // for (auto n : v) {
    //     std::cout << n << " ";
    // }
    // std::cout << "\n";
    if (v.empty()) {
        return std::vector<unsigned int>();
    } else if (v.size() == 1) {
        return v;
    }
    std::vector<std::vector<unsigned int>> buckets(10); // 1 bucket for each digit
    for (const auto& n : v) {
        // add number to bucket
        buckets.at(getDigit(n, getPower(n) - power)).emplace_back(n); // emplace_back?
    }

    const unsigned int nextPower = power + 1;
    std::vector<std::future<std::vector<unsigned int>>> sortedBuckets;
    // for every bucket, create new thread and do radix sort again
    for (std::vector<unsigned int> bucket : buckets) {
        for (auto n : bucket) {
            std::cout << n << " ";
        }
        std:: cout << "- " << bucket.size();
        std::cout << "\n";
        if (bucket.size() > 0) { // no need to sort if 0 or 1 things in bucket
            // radixSort(bucket, power + 1);
            std:: cout << "is greater than 1: " << bucket.size();
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

    std::cout << "asd " << sortedBuckets.size() << "\n";

    if (sortedBuckets.empty()) {
        return std::vector<unsigned int>();
    }
    return std::accumulate(
        std::next(sortedBuckets.begin()),
        sortedBuckets.end(),
        sortedBuckets.at(0).get(),
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

    for (auto n : numbersToSort) {
        std::cout << n << " ";
    }
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
