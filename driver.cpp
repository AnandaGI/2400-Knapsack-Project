/*
Authors: Ananda Irwin
Created: 7/26/2025
Modified: 7/27/2025
*/

#include "greedy_algo.h"
#include "brute_algo.h"
#include "dp_algo.h"
#include "memoization_algo.h"
#include "input_generation.h"

#include <functional>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <thread>
#include <future>

namespace constants{
    const int MAX_SIZE = 65535; //Max vector size
    const string INPUT_FILE_PATH = "inputFile.txt";   //Filepath for the input textfile
    const int TRIALS = 500; //Number of times to run the algorithm per input size
    const long long MAX_TIME = 5400000000000; //1.5 hours
    const uint8_t NUM_INPUTS = 15; //Modify to suit needs. Can be 1 (n=4) to 15 (n=65536). Each increases max vector size reached in main loops.
    const string CSV_HEADER = ",Brute,Greedy,Dynamic Programming,Memo,FPTAS";
}

AlgorithmData runAlgorithm(std::vector<Item> items, int weight, int trials, function<AlgorithmData(std::vector<Item>, int)> funcKnap){
    uint64_t duration = 0;
    AlgorithmData temp;

    for (int i = 0; i < trials; i++){
        temp = funcKnap(items, weight);
        duration += temp.duration;
        
        //If run time takes greater than 1.5 hours
        if (temp.duration >= constants::MAX_TIME){
            return AlgorithmData{temp.totalValue, temp.duration, temp.basicOps};
        }
    }

    return AlgorithmData{temp.totalValue, static_cast<long long>(ceil(duration/trials)), temp.basicOps};
}

int main(){
    std::vector<Item> items = retrieve_arr(constants::INPUT_FILE_PATH, 256);
    uint16_t maxVal = 0;
    int weight;
    int currSize = 2;
    int percentDone;
    std::string delimiter = ",";    //Sleep deprived brain can't think of a better way right now (you'll see)
    AlgorithmData bruteData;
    AlgorithmData greedyData;
    AlgorithmData dynamicProgData;
    AlgorithmData memoData;
    AlgorithmData FPTASData;
    std::vector<Item> tempItems;
    std::vector<Item> fptasInput;
    AlgorithmData* data[5] = {&bruteData, &greedyData, &dynamicProgData, &memoData, &FPTASData};
    std::ofstream runtimeFile("output/runtime.csv");
    std::ofstream basicOpsFile("output/basicOps.csv");
    std::ofstream solutionFile("output/solutions.csv"); //Mostly useful for checking how greedy and FPTAS differ
    

    runtimeFile << constants::CSV_HEADER << "\n";
    basicOpsFile << constants::CSV_HEADER << "\n";
    solutionFile << constants::CSV_HEADER << "\n";

    //Start Timer
    auto startTime = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < constants::NUM_INPUTS; i++){
        int currSize = currSize * 2;    //Start at 4, up to MAX_SIZE (2^16);
        tempItems = {items.begin(), items.begin() + currSize};
        weight = 125 * currSize;

        if (i < 3){ //Past input size 16, brute force just takes far too long.
        //BRUTE FORCE
        bruteData = runAlgorithm(tempItems, weight, constants::TRIALS, &bruteKnapsackWrapper);
        } else {
            bruteData = {0, 0, 0};
        }

        //GREEDY
        greedyData = runAlgorithm(tempItems, weight, constants::TRIALS, &greedyKnapsack);

        //DYNAMIC PROGRAMMING
        dynamicProgData = runAlgorithm(tempItems, weight, constants::TRIALS, &dpWrapper);

        //MEMOIZATION
        memoData = runAlgorithm(tempItems, weight, constants::TRIALS, &memoWrapper);

        //FPTAS APPROXIMATION
        if (i < 10){ //Past input size 2048, this algorithm can cause issues due to 16bit int limits
            fptasInput = generateFPTASInput(0.05, items, fptasInput.size(), currSize, maxVal);
            FPTASData = runAlgorithm(fptasInput, weight, constants::TRIALS, dpWrapper);
            FPTASData.totalValue = FPTASData.totalValue * ((.05 * maxVal) / 16);
        } else {
            FPTASData = {0, 0, 0};
        }

        //Write to File
        percentDone = ((i+1) * 100)/(constants::NUM_INPUTS);
        std::cout << std::to_string(percentDone) << "% Complete, Input size: " << std::to_string(currSize) << std::endl;

        runtimeFile << std::to_string(currSize) << delimiter;
        basicOpsFile << std::to_string(currSize) << delimiter;
        solutionFile << std::to_string(currSize) << delimiter;

        for(int i = 0; i < 5; i++){
            if(i == 4)
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


    //Print out total program duration
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "\n\nTotal Time: " << std::to_string(duration.count()) << std::endl;

    return 0;
}
