#How to Run
Project can be run by activating the executable or running it in windows commandline.

Use the multithreaded executable for supported CPUs to decrease runtime. Otherwise, the
default executable with run the experiment linearly.

#How the experiment/code works.
The experiment consists of running each algorithm 500 times and averaging the result for a given input range.
For example, 5 total runs of the algorithms will make each algorithm run with an input size of 4, 8, 16, 32,
and 64, 500 times each, then average the resulting runtime and output the basic operations performed and the
solution the algorithm yielded.

All of the data is output to a csv file as it comes in, allowing you to use it easily with excel or another
application that can easily convert this data.

# 2400-Knapsack-Project
Project for CSC 2400, focusing on and analyzing algorithms that attempt tto solve the knapsack problem.
(Don't push to main)

Input for the algorithms will be an array containing _n_ elements. Each element will be a struct with _weight_ and _value_ attributes.
The struct will be of the following form:
`struct Item{
  int weight;
  int value;
};`

The Chrono library will be used for the timer, using its high_resolution_clock. To get the time, you can use:
`auto curr_time = chrono::high_resolution_clock::now();`
This can be used to get the start and ending time, then subtract for total time.

The input will be randomly generated using an algorithm. 
Assume the array size is in between 4 and 2^n with structures containing 1 8bit (`uint8_t`) integer
for weight and another 16bit (`uint16_t`) integer for value.
One array of the maximum size to be tested will be generated, and all algorithms will use successive
partitions of the array. (i.e. 0..3, 0..7, 0..15, .. , 0..2^n-1)

Write the algorithm such that it takes in the following:
1. An array of size _n_
2. The total weight capacity
3. Anything else the specific algorithm might call for

Each algorithm should return the following:
1. Solution to the knapsack problem.
2. The time it takes for the algorithm to execute in full.
3. The number of basic operations performed during execution.
