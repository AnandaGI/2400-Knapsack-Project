/*
Authors: Ananda Irwin, Adam Light
Created: 7/25/2025
Modified 7/25/2025
This file contains the brute force solution algorithm for the knapsack program.
Algorithm written by Adam Light, lightly edited by Ananda Irwin
*/

#ifndef GREEDY_ALGO_H
#define GREEDY_ALGO_H

#include "input_generation.h"
#include <vector>
#include <algorithm>
#include <chrono>

bool compareItems(const Item a, const Item b) {
	return a.Item::ratio() > b.Item::ratio();
}

AlgorithmData greedy_knapsack(std::vector<Item> items, int knapsackCapacity){
    auto start_time_greedy = std::chrono::high_resolution_clock::now();
    int totalWeight = 0;
    int totalValue = 0;
    int count = 0;
    int n = items.size();
    int basic_ops = 0;

    std::sort(items.begin(), items.end(), compareItems);

    std::vector<Item> selected;
    for (const auto& i : items) {
        basic_ops++;
        if (count >= n) break;
        if (totalWeight + i.weight <= knapsackCapacity) {
            selected.push_back(i);
            totalWeight += i.weight;
            totalValue += i.value;
        }
        count++;
    }
    auto end_time_greedy = std::chrono::high_resolution_clock::now();
    auto duration_greedy = std::chrono::duration_cast<std::chrono::microseconds>(end_time_greedy - start_time_greedy);

    return AlgorithmData{totalValue, duration_greedy.count(), basic_ops};
}

// int main(){
//     std::vector<Item> items = {Item{1, 4}, Item{2, 5}, Item{3, 1}, Item{2, 4}, Item{1, 2}};
//     int capacity = 4;  //Capacity of knapsack

//     int totalValue;
//     double duration;
//     AlgorithmData data = greedy_knapsack(items, capacity);

//     std::cout << "Total Value: " << std::to_string(data.totalValue) << std::endl;
//     std::cout << "Total Duration (microseconds): " << std::to_string(data.duration) << std::endl;
//     std::cout << "Total Basic Operations: " << std::to_string(data.basicOps) << std::endl;

//     return 0;
// }

#endif
