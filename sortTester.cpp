/*
 * Sample User Code
 *
 * Test cases for this assignment will involve user code creating large vectors of
 * random numbers and calling the sort method. Your code will be limited by time
 * and memory with tests increasing in difficultly through decreasing time and
 * memory limits and increasing vector sizes. 
 *
 * The following sample test case shows how this is done. For the given single
 * threaded code, this sort takes around about 23 seconds to run on the CSE server
 * williams (with 8 cores). In comparison the model multithreaded solution runs for
 * about 6 seconds (without any optimisation being applied).
 */

#include <iostream>
#include <random>
#include <thread>

#include "BucketSort.h"

int main() {
    std::vector<unsigned int> v = {32, 3, 53, 3, 134, 643, 5, 5, 12, 52, 501, 98};
    BucketSort bs;
    bs.numbersToSort = v;
    const unsigned int numCores = std::thread::hardware_concurrency();
    bs.sort(numCores);
    for (auto n : bs.numbersToSort) {
        std::cout << n << "-";
    }
}

// int main() {
//     unsigned int totalNumbers = 500000;
//     unsigned int printIndex = 259000;

//     // use totalNumbers required as the seed for the random
//     // number generator. 
//     std::mt19937 mt(totalNumbers);
//     std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());

//     // create a sort object
//     BucketSort pbs;

//     // insert random numbers into the sort object
//     for (unsigned int i=0; i < totalNumbers; ++i) {
//         pbs.numbersToSort.push_back(dist(mt));
//     }
    
//     // call sort giving the number of cores available.
//     const unsigned int numCores = std::thread::hardware_concurrency();
//     pbs.sort(numCores);

//     std::cout << "number of cores used: " << numCores << std::endl;

//     // print certain values from the buckets
//     std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;
//     std::cout << pbs.numbersToSort[0] << " " << pbs.numbersToSort[printIndex - 10000]
//         << " " << pbs.numbersToSort[printIndex] << " " << pbs.numbersToSort[pbs.numbersToSort.size() - 1]
//         << std::endl;
// }
