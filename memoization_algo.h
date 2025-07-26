/*
Authors: Ananda Irwin, Adam Light
Created: 7/25/2025
Modified 7/25/2025
This file contains the dynbamic programming memoization solution algorithm for the knapsack program.
*/

#ifndef MEMO_ALGO_H
#define MEMO_ALGO_H

#include "input_generation.h"
#include <vector>
#include <chrono>

int memoKnapsack( 
    std::vector<Item> &items,
    int W,  //Weight aka Capacity
    int n, 
    std::vector<std::vector<int>> &memo,
    int &basicOps) {

    int basic_operations = 0;

    // Base Case
    if (n == 0 || W == 0)
        return 0;

    // Check if we have previously calculated the same subproblem
    if(memo[n][W] != -1)
        return memo[n][W];

    int pick = 0;
    basicOps++; //Comparison below is basic operation
    if (items[n - 1].weight <= W)
        pick = items[n - 1].value + memoKnapsack(items, W - items[n - 1].weight, n - 1, memo, basicOps);

    int notPick = memoKnapsack(items, W, n - 1, memo, basicOps);

    return memo[n][W] = std::max(pick, notPick);
}

AlgorithmData memoWrapper(std::vector<Item> items, int capacity) {
    int n = items.size();
    int basicOps = 0;
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1)); // Memoization table to store the results
    
    auto startTimeMemo = std::chrono::high_resolution_clock::now();
    int totalValue = memoKnapsack(items, capacity, n, memo, basicOps);
    auto endTimeMemo = std::chrono::high_resolution_clock::now();
    auto durationMemo = std::chrono::duration_cast<std::chrono::microseconds>(endTimeMemo - startTimeMemo);

    return AlgorithmData{totalValue, durationMemo.count(), basicOps};
}

int main(){
    std::vector<Item> items = {Item{1, 4}, Item{2, 5}, Item{3, 1}, Item{2, 4}, Item{1, 2}};

    AlgorithmData data = memoWrapper(items, 4);

    std::cout << "Total Value: " << std::to_string(data.totalValue) << std::endl;
    std::cout << "Total Duration (microseconds): " << std::to_string(data.duration) << std::endl;
    std::cout << "Total Basic Operations: " << std::to_string(data.basicOps) << std::endl;
    
    return 0;
}

#endif
