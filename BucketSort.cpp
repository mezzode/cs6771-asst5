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

bool BucketSort::radixSort(const iterator& begin, const iterator& end, const unsigned int& power) {
    if (begin == end) {
        return true;
    }
    // unordered map vs array vs vector for buckets?
    std::vector<unsigned int> finishedBucket; // bucket for elements which cannot be further done
    std::vector<std::vector<unsigned int>> buckets(10); // one for each digit

    for (auto it = begin; it != end; ++it) {
        const auto& num = *it;
        const auto result = getDigit(num, getPower(num) - power);
        if (result == -1) {
            finishedBucket.emplace_back(num);
        } else {
            buckets.at(result).emplace_back(num);
        }
    }

    const unsigned int nextPower = power + 1;
    auto it = std::copy(finishedBucket.begin(), finishedBucket.end(), begin);
    std::vector<std::future<bool>> sortedBuckets;
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
        sortedBuckets.emplace_back(std::async(
            std::launch::async,
            [this] (const iterator& begin, const iterator& end, const unsigned int& power) {
                return radixSort(begin, end, power);
            },
            bucketBegin,
            bucketEnd,
            nextPower
        ));
    }

    return std::all_of(
        sortedBuckets.begin(),
        sortedBuckets.end(),
        [] (std::future<bool>& success) {
            return success.get();
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

    radixSort(numbersToSort.begin(), numbersToSort.end(), 0);

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
