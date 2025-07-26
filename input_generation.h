/*
Authors: Ananda Irwin, Adam Light
Created: 7/22/2025
Modified 7/23/2025
This file contains the Item structure that will be stored in the input arrays.
It additionally contains the algorithm for generating these input array.
*/

#ifndef INPUT_GEN_H
#define INPUT_GEN_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <random>


struct Item{
    uint8_t weight;
    uint8_t value;

    double ratio() const{
        return static_cast<double>(value) / weight; //ratio needed for greedy algorithm 
    }
};

struct AlgorithmData{
    int totalValue;
    long long duration;
    int basicOps;
};

std::vector<Item> retrieve_arr(std::string filepath, int size){
    std::ifstream dataFile(filepath);
    std::string output;
    std::vector<Item> item_arr;
    int i = 0;
    int delimPos;
    uint8_t weight;
    uint8_t val;

    while (std::getline(dataFile, output) && i < size){
        delimPos = output.find(' ');
        weight = stoi(output.substr(0, delimPos));
        val = stoi(output.substr(delimPos+1));
        Item tempItem{weight, val};

        item_arr.push_back(tempItem);
        i++;
    }
    return item_arr;
};

void create_data(std::string output_dir, int size){
    std::vector<Item> structArray(size);

    std::mt19937 randomEngine(std::random_device{}());
    std::uniform_int_distribution<unsigned short> dist(1, 255); // Must be 0-255 for uint8_t, at least 1 for item/value

    // Populate structArray with random data
    for (size_t i = 0; i < size; ++i) {
        structArray[i].weight = static_cast<uint8_t>(dist(randomEngine));
        structArray[i].value = static_cast<uint8_t>(dist(randomEngine));
    }

    // Write structArray to file
    std::ofstream output_file(output_dir);
    if (output_file.is_open()) {
        for (size_t i = 0; i < size; ++i) {
            output_file << static_cast<int>(structArray[i].weight) << " "
                << static_cast<int>(structArray[i].value) << "\n";
        }
        output_file.close();
        std::cout << "Struct data written to output file successfully.\n";
    }
    else {
        std::cerr << "Error: Unable to open file for writing.\n";
    }
}

void test_allocation(int size, std::string path){
    create_data(path, size);
    std::vector<Item> item_arr = retrieve_arr(path, size);

    for(int i = 0; i < size; i++){
        std::cout << std::to_string(item_arr[i].weight) + " " + std::to_string(item_arr[i].value) + "\n";
    }
}

#endif
