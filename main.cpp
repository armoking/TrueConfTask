#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <cassert>

constexpr unsigned int MAX_N = 30; // number of adding elements in map and vector

constexpr int MIN_LIMIT_TO_GENERATE_VALUES = 1;
constexpr int MAX_LIMIT_TO_GENERATE_VALUES = 9;

constexpr unsigned int MIN_LIMIT_TO_GENERATE_AMOUNT_OF_ITEMS_TO_DELETE = 0;
constexpr unsigned int MAX_LIMIT_TO_GENERATE_AMOUNT_OF_ITEMS_TO_DELETE = 15;

constexpr bool DEBUG = true;

void LogResult(std::vector<int>& vector) {
    if constexpr (DEBUG) {
        std::cout << "vector constains next data: (index, value)\n";
        for (unsigned int i = 0; i < std::size(vector); ++i) {
            std::cout << "{" << i << ", " << vector[i] << "} ";
        }
        std::cout << '\n';
    }
}

void LogResult(std::map<unsigned int, int>& map) {
    if constexpr (DEBUG) {
        std::cout << "map constains next data: (key, value)\n";
        for (const auto& [key, value] : map) {
            std::cout << "{" << key << ", " << value << "} ";
        }
        std::cout << '\n';
    }
}

int main() {
    std::vector<int> vector(MAX_N);
    std::map<unsigned int, int> map;
    
    std::mt19937 mt19937_random(std::random_device{}()); // set random-device-seed for mt19937 generator
    std::uniform_int_distribution<int> distributionForGenerating(
                                                    MIN_LIMIT_TO_GENERATE_VALUES,
                                                    MAX_LIMIT_TO_GENERATE_VALUES
                                                );
    std::uniform_int_distribution<unsigned int> distributionForDeleting(
                                                    MIN_LIMIT_TO_GENERATE_AMOUNT_OF_ITEMS_TO_DELETE,
                                                    MAX_LIMIT_TO_GENERATE_AMOUNT_OF_ITEMS_TO_DELETE
                                                );
                                                
    auto generateRandomValue = [&mt19937_random] (auto& distribution) {
        return distribution(mt19937_random);
    };
    
    for (unsigned int i = 0; i < MAX_N; ++i) {
        vector[i] = generateRandomValue(distributionForGenerating);
        map[i] = generateRandomValue(distributionForGenerating);
    }
    
    LogResult(vector);
    LogResult(map);
    
    const unsigned int numberOfItemsToDeleteInVector = std::min(MAX_N, generateRandomValue(distributionForDeleting));
    const unsigned int numberOfItemsToDeleteInMap = std::min(MAX_N, generateRandomValue(distributionForDeleting));
    
    std::cout << "Number of items to delete in vector = " << numberOfItemsToDeleteInVector << '\n';
    std::cout << "Number of items to delete in map = " << numberOfItemsToDeleteInMap << '\n';
    
    for (unsigned int i = 0; i < numberOfItemsToDeleteInVector; ++i) { // remove last numberOfItemsToDeleteInVector elements from vector
        vector.pop_back();
    }
    
    for (unsigned int i = 0; i < numberOfItemsToDeleteInMap; ++i) { // remove last numberOfItemsToDeleteInMap elements from map
        map.erase(std::size(map) - 1);
    }
    
    LogResult(vector);
    LogResult(map);
    
    std::vector<bool> indexToDeleteVector(std::size(vector), false);
    std::vector<bool> indexToDeleteMap(MAX_N, false);
    
    std::vector<std::vector<unsigned int>> indexesOfValuesVector(MAX_LIMIT_TO_GENERATE_VALUES + 1); // [value] - contains all indexes i such that vector[i] == value
    std::vector<std::vector<unsigned int>> indexesOfValuesMap(MAX_LIMIT_TO_GENERATE_VALUES + 1); // [value]- contains all indexes i such that map[i] == value
    
    for (unsigned int index = 0; index < std::size(vector); ++index) {
        indexesOfValuesVector[vector[index]].push_back(index);
    }
    for (const auto& [index, value] : map) {
        indexesOfValuesMap[value].push_back(index);
    }
    
    for (int value = MIN_LIMIT_TO_GENERATE_VALUES; value <= MAX_LIMIT_TO_GENERATE_VALUES; ++value) {
        auto& vectorIndexes = indexesOfValuesVector[value];
        auto& mapIndexes = indexesOfValuesMap[value];
        
        while(std::size(vectorIndexes) < std::size(mapIndexes)) {
            indexToDeleteMap[mapIndexes.back()] = true;
            mapIndexes.pop_back();
        }
    
        while(std::size(mapIndexes) < std::size(vectorIndexes)) {
            indexToDeleteVector[vectorIndexes.back()] = true;
            vectorIndexes.pop_back();
        }
    }
        
    {
        unsigned int j = 0;
        for (unsigned int i = 0; i < std::size(vector); ++i) {
            if (!indexToDeleteVector[i]) {
                std::swap(vector[j], vector[i]);
                ++j;
            }
        }
        vector.resize(j);
    }
    
    for (unsigned int i = 0; i < MAX_N; ++i) {
        if (indexToDeleteMap[i]) {
            map.erase(i);
        }
    }
    
    LogResult(vector);
    LogResult(map);
    
    // check result
    {
        std::vector<int> vectorValues = vector;
        std::sort(std::begin(vectorValues), std::end(vectorValues));
        
        std::vector<int> mapValues;
        mapValues.reserve(std::size(map));
        for (auto& [key, value] : map) {
            mapValues.push_back(value);
        }
        std::sort(std::begin(mapValues), std::end(mapValues));
        
        for_each(std::begin(vectorValues), std::end(vectorValues), [&] (const auto& value) { 
            std::cout << value << ' ';
        });    
        std::cout << '\n';
        
        for_each(std::begin(mapValues), std::end(mapValues), [&] (const auto& value) {
            std::cout << value << ' ';
        });    
        std::cout << '\n';
        
        
        assert(std::size(vectorValues) == std::size(mapValues));
        assert(vectorValues == mapValues);
        std::cout << "Correct\n";
    }
}
