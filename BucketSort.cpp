#include "BucketSort.h"
#include <cmath>
#include <future>
#include <algorithm>

#include <iostream>

// rather than string and needing to convert each time make it an array/vector of digits?
unsigned int ctoui(char c) {
    return c - '0';
}

std::vector<std::string> radixSort(std::vector<std::string> v, const unsigned int& power) {
    std::cout << power << "\n";
    if (v.empty()) {
        return std::vector<std::string>();
    } else if (v.size() == 1) {
        return v;
    }

    // should change to in place
    // "buckets" should store indices/iterators pointing to start/end of bucket instead?

    std::vector<std::vector<std::string>> buckets(10); // 1 bucket for each digit
    std::vector<std::string> endBucket;
    for (const auto& n : v) {
        if (power >= n.size()) {
            endBucket.emplace_back(n);
        } else {
            buckets.at(ctoui(n.at(power))).emplace_back(n);
        }
    }

    const unsigned int nextPower = power + 1;
    std::vector<std::future<std::vector<std::string>>> sortedBuckets;
    // for every bucket, create new thread and do radix sort again
    for (auto bucket : buckets) {
        if (bucket.size() > 0) { // no need to sort if 0 or 1 things in bucket
            sortedBuckets.emplace_back(std::async(
                std::launch::async,
                [] (std::vector<std::string> bucket, const unsigned int& power) {
                    return radixSort(bucket, power);
                },
                bucket,
                nextPower
            ));
        }
    }

    if (sortedBuckets.empty()) {
        return std::vector<std::string>();
    }
    return std::accumulate(
        sortedBuckets.begin(),
        sortedBuckets.end(),
        endBucket,
        [] (std::vector<std::string> a, std::future<std::vector<std::string>>& b) {
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
    std::vector<std::string> strings{numbersToSort.size()};
    std::transform(numbersToSort.begin(), numbersToSort.end(), strings.begin(), [] (const unsigned int& n) {
        return std::to_string(n);
    });

    strings = radixSort(strings, 0);

    // numbersToSort.clear();
    std::transform(strings.begin(), strings.end(), numbersToSort.begin(),  [] (const std::string& s) {
        return std::stoul(s);
    });

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
