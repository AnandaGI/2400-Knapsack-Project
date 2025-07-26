/*
Authors: Ananda Irwin, Adam Light
Created: 7/25/2025
Modified 7/25/2025
This file contains the brute force solution algorithm for the knapsack program.
Algorithm written by Adam Light, wrapper written by Ananda Irwin
*/

#ifndef BRUTE_ALGO_H
#define BRUTE_ALGO_H

#include "input_generation.h"
#include <vector>
#include <chrono>


int knapsackBruteForce(const std::vector<Item>& items, int index, int maxIndex, int remainingWeight, int &basicOps) {

	if (index == maxIndex || remainingWeight == 0) {
		return 0;
	}

	const Item& current = items[index];

    basicOps++;
	if (current.weight > remainingWeight) {
		return knapsackBruteForce(items, index + 1, maxIndex, remainingWeight, basicOps);
	}
	else {
		int exclude = knapsackBruteForce(items, index + 1, maxIndex, remainingWeight, basicOps);
		if (exclude == -1) return -1;

		int include = knapsackBruteForce(items, index + 1, maxIndex, remainingWeight - current.weight, basicOps);
		if (include == -1) return -1;

		return std::max(current.value + include, exclude);
	}
}

AlgorithmData bruteKnapsackWrapper(const std::vector<Item>& items, int capacity){
    int basicOps = 0;

    auto startTimeBrute = std::chrono::high_resolution_clock::now();
    int totalValue = knapsackBruteForce(items, 0, items.size(), capacity, basicOps);
    auto endTimeBrute = std::chrono::high_resolution_clock::now();
    auto durationBrute = std::chrono::duration_cast<std::chrono::microseconds>(endTimeBrute - startTimeBrute);

    return AlgorithmData{totalValue, durationBrute.count(), basicOps};
}

int main(){
    std::vector<Item> items = {Item{1, 4}, Item{2, 5}, Item{3, 1}, Item{2, 4}, Item{1, 2}};

    AlgorithmData data = bruteKnapsackWrapper(items, 4);

    std::cout << "Total Value: " << std::to_string(data.totalValue) << std::endl;
    std::cout << "Total Duration (microseconds): " << std::to_string(data.duration) << std::endl;
    std::cout << "Total Basic Operations: " << std::to_string(data.basicOps) << std::endl;
    
    return 0;
}

#endif
