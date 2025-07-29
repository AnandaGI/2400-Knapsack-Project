/*
Authors: Ananda Irwin
Created: 7/27/2025
Modified 7/28/2025
Runs the experiment for all algorithms a specified number of times and outputs all data to a file.
Multithreaded
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
#include <queue>

namespace constants{
    const int MAX_SIZE = 65535; //Max vector size
    const string INPUT_FILE_PATH = "inputFile.txt";   //Filepath for the input textfile
    const int TRIALS = 500; //Number of times to run the algorithm per input size
    const long long MAX_TIME = 5400000000000; //1.5 hours
    const uint8_t NUM_INPUTS = 10; //Modify to suit needs. Can be 1 (2*2^1 -> n = 4) to 15 (2*2^15 -> n = 65536). Each increases max vector size reached in main loops.
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
    std::vector<Item> items = retrieve_arr(constants::INPUT_FILE_PATH, constants::MAX_SIZE);
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
    AlgorithmData tempData;
    std::future<AlgorithmData> greedy;
    std::future<AlgorithmData> dp;
    std::future<AlgorithmData> memo;
    std::future<AlgorithmData> fptas;
    std::future<AlgorithmData> brute;
    std::vector<Item> tempItems;
    std::vector<Item> fptasInput;
    std::queue<std::future<AlgorithmData>*> asyncData;
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

        //Gen FPTAS input beforehand
        fptasInput = generateFPTASInput(0.05, items, fptasInput.size(), currSize, maxVal);

        //GREEDY
        greedy = std::async(runAlgorithm, tempItems, weight, constants::TRIALS, &greedyKnapsack);

        //DYNAMIC PROGRAMMING
        dp = std::async(runAlgorithm, tempItems, weight, constants::TRIALS, &dpWrapper);

        //MEMOIZATION
        memo = std::async(runAlgorithm, tempItems, weight, constants::TRIALS, &memoWrapper);

        //FPTAS APPROXIMATION
        if (i < 10){    //Past input size 2048, this algorithm can cause issues due to 16bit int limits
            fptas = std::async(runAlgorithm, fptasInput, weight, constants::TRIALS, &dpWrapper);
        }

        //BRUTE FORCE - Run Last
        if (i < 3){     //Past input size 16, brute force just isn't viable
            brute = std::async(runAlgorithm, tempItems, weight, constants::TRIALS, &bruteKnapsackWrapper);
        }

        greedyData = greedy.get();
        dynamicProgData = dp.get();
        memoData = memo.get();
        if(i < 3)
            bruteData = brute.get();
        if(i < 10){
            FPTASData = fptas.get();
            FPTASData.totalValue = FPTASData.totalValue * ((.05 * maxVal) / currSize);
        }
        

        //Write to File
        percentDone = ((i+1) * 100)/(constants::NUM_INPUTS);
        std::cout << std::to_string(percentDone) << "% Complete, Input size: " << std::to_string(currSize) << std::endl;

        runtimeFile << std::to_string(currSize) << delimiter;
        basicOpsFile << std::to_string(currSize) << delimiter;
        solutionFile << std::to_string(currSize) << delimiter;

        for(int j = 0; j < 5; j++){
            if(j == 4)
                delimiter = "\n";
            runtimeFile << std::to_string(data[j]->duration) << delimiter;
            basicOpsFile << std::to_string(data[j]->basicOps) << delimiter;
            solutionFile << std::to_string(data[j]->totalValue) << delimiter;
            delimiter = ",";
        }
    }


    //Print out total program duration
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "\n\nTotal Time: " << std::to_string(duration.count()) << std::endl;

    //Close Files
    runtimeFile.close();
    basicOpsFile.close();
    solutionFile.close();


}