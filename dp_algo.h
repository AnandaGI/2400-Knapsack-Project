/*
Authors: Brent Maxwell, Ananda Irwin
Created: 7/25/2025
Modified 7/25/2025
This file contains the dynbamic programming solution algorithm for the knapsack program.
*/

#include "input_generation.h"
#include <vector>
#include <chrono>
using namespace std;

#ifndef DP_ALGO_H
#define DP_ALGO_H

int dpKnapsack(vector<Item> &it, int W, int &basicOps) {
    int n = it.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1));

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else {
                int pick = 0;
                basicOps++;
                if(it[i - 1].weight <= j)
                    pick = it[i - 1].value + dp[i - 1][j - it[i - 1].weight];
                int notPick = dp[i - 1][j];
                dp[i][j] = max(pick, notPick);
            }
        }
    }
    return dp[n][W];
}

AlgorithmData dpWrapper(vector<Item> items, int capacity){
    int basicOps = 0;

    auto startTimeBrute = std::chrono::high_resolution_clock::now();
    int totalValue = dpKnapsack(items, capacity, basicOps);
    auto endTimeBrute = std::chrono::high_resolution_clock::now();
    auto durationBrute = std::chrono::duration_cast<std::chrono::microseconds>(endTimeBrute - startTimeBrute);

    return AlgorithmData{totalValue, durationBrute.count(), basicOps};
}

// int main(){
//     std::vector<Item> items = {Item{1, 4}, Item{2, 5}, Item{3, 1}, Item{2, 4}, Item{1, 2}};

//     AlgorithmData data = dpWrapper(items, 4);

//     std::cout << "Total Value: " << std::to_string(data.totalValue) << std::endl;
//     std::cout << "Total Duration (microseconds): " << std::to_string(data.duration) << std::endl;
//     std::cout << "Total Basic Operations: " << std::to_string(data.basicOps) << std::endl;
    
//     return 0;
// }

#endif
