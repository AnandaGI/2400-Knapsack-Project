/*
This file contains the item structure that will be stored in the input arrays.
It additionally contains the algorithm for generating these input array.
*/
#ifndef INPUT_GEN_H
#define INPUT_GEN_H

#include <fstream>
#include <string>
#include <iostream>


struct item{
    uint8_t weight;
    uint8_t value;
};

//Implement algorithm to create input arrays
item** retrieve_arr(std::string filepath, int size){
    std::ifstream dataFile(filepath);
    std::string output;
    item* *item_arr = new item*;
    int i = 0;
    int delimPos;
    int weight;
    int val;

    while (std::getline(dataFile, output) && i < size){
        delimPos = output.find(' ');
        weight = stoi(output.substr(0, delimPos));
        val = stoi(output.substr(delimPos+1));

        item_arr[i] = new item;
        item_arr[i]->weight = weight;
        item_arr[i]->value = val;
        i++;
    }
    return item_arr;
};

void test_allocation(int size, std::string path){
    item** item_arr = retrieve_arr(path, size);

    for(int i = 0; i < size; i++){
        std::cout << std::to_string(item_arr[i]->weight) + " " + std::to_string(item_arr[i]->value) + "\n";
        delete item_arr[i];
    }
    delete [] item_arr;
}

int main(){
    int size;
    std::string path;

    std::cout << "What size array would you like to test for? ";
    std::cin >> size;
    std::cout << "What is the file path? ";
    std::cin >> path;

    test_allocation(size, path);
}

#endif
