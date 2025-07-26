#include "input_generation.h"
using namespace std;

int knapsack(int W, vector<item> &it) {
    int n = it.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1));

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else {
                int pick = 0;
                if(it[i - 1].weight <= j)
                    pick = it[i - 1].value + dp[i - 1][j - it[i - 1].weight];
                int notPick = dp[i - 1][j];
                dp[i][j] = max(pick, notPick);
            }
        }
    }
    return dp[n][W];
}

int main(void){
    
    vector<item> items = retrieve_arr("inputFile.txt", 65536);

    return 0;
}