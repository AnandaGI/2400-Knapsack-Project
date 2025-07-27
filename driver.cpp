/*
Authors: Ananda Irwin, Adam Light
Created: 7/26/2025
Modified 7/26/2025
*/

#include "greedy_algo.h"
#include "brute_algo.h"
#include "dp_algo.h"
#include "memoization_algo.h"
#include "input_generation.h"

#include <functional>
#include <cmath>
#include <fstream>


namespace constants{
    const int MAXSIZE = 65536; //Max vector size
    const string INPUTFILEPATH = "inputFile.txt";   //Filepath for the input textfile
    const int TRIALS = 500; //Number of times to run the algorithm per input size
    const long long MAXTIME = 5400000000000; //1.5 hours
    const uint8_t numInputs = 5; //Modify to suit needs. Can be 1 (n=4) to 15 (n=65536). Each increases max vector size reached in main loops.
    const string CSVHEADER = ",Brute,Greedy,Dynamic Programming,Memo,FPTAS";
}

AlgorithmData runAlgorithm(std::vector<Item> items, int weight, int trials, function<AlgorithmData(std::vector<Item>, int)> funcKnap){
    uint64_t duration = 0;
    AlgorithmData temp;

    for (int i = 0; i < trials; i++){
        temp = funcKnap(items, weight);
        duration += temp.duration;
        
        //If run time takes greater than 1.5 hours
        if (temp.duration >= constants::MAXTIME){
            return AlgorithmData{temp.totalValue, temp.duration, temp.basicOps};
        }
    }

    return AlgorithmData{temp.totalValue, static_cast<long long>(ceil(duration/trials)), temp.basicOps};
}

int main(){
    std::vector<Item> items = retrieve_arr(constants::INPUTFILEPATH, constants::MAXSIZE);
    int weight;
    int currSize = 2;
    std::string delimiter = ",";    //Sleep deprived brain can't think of a better way right now (you'll see)
    AlgorithmData bruteData;
    AlgorithmData greedyData;
    AlgorithmData dynamicProgData;
    AlgorithmData memoData;
    AlgorithmData FPTASData;
    std::vector<Item> tempItems;
    AlgorithmData* data[5] = {&bruteData, &greedyData, &dynamicProgData, &memoData, &FPTASData};
    std::ofstream runtimeFile("output/runtime.csv");
    std::ofstream basicOpsFile("output/basicOps.csv");
    std::ofstream solutionFile("output/solutions.csv"); //Mostly useful for checking how greedy and FPTAS differ
    

    runtimeFile << constants::CSVHEADER << "\n";
    basicOpsFile << constants::CSVHEADER << "\n";
    solutionFile << constants::CSVHEADER << "\n";

    for(int i = 0; i < constants::numInputs; i++){
        int currSize = currSize * 2;    //Start at 4, up to MAXSIZE (2^16);
        tempItems = {items.begin(), items.begin() + currSize};
        weight = 125 * currSize;

        if (i < 3){ //Past input size 16, brute force just takes far too long.
        //BRUTE FORCE
        bruteData = runAlgorithm(tempItems, weight, constants::TRIALS, &bruteKnapsackWrapper);
        }

        //GREEDY
        greedyData = runAlgorithm(tempItems, weight, constants::TRIALS, &greedyKnapsack);

        //DYNAMIC PROGRAMMING
        dynamicProgData = runAlgorithm(tempItems, weight, constants::TRIALS, &dpWrapper);

        //MEMOIZATION
        memoData = runAlgorithm(tempItems, weight, constants::TRIALS, &memoWrapper);

        //Write to File
        

        std::cout << "Input size: " << std::to_string(currSize) << std::endl;
        runtimeFile << std::to_string(currSize) << delimiter;
        basicOpsFile << std::to_string(currSize) << delimiter;
        solutionFile << std::to_string(currSize) << delimiter;

        for(int i = 0; i < 4; i++){
            if(i == 3)
                delimiter = "\n";
            runtimeFile << std::to_string(data[i]->duration) << delimiter;
            basicOpsFile << std::to_string(data[i]->basicOps) << delimiter;
            solutionFile << std::to_string(data[i]->totalValue) << delimiter;
            delimiter = ",";
        }
    }

    //Close Files
    runtimeFile.close();
    basicOpsFile.close();
    solutionFile.close();



    return 0;
}