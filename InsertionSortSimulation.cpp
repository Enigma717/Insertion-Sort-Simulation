#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <random>

#define GENERATED_NUMBERS_BASE 100
#define SIMULATION_REPETITIONS 1

using namespace std;
using namespace chrono;
typedef steady_clock Clock;

struct data {
    int comparisons;
    int swaps;
    double duration;
};

int mersenne_generator(const int& A, const int& B) {
    static unsigned seed = system_clock::now().time_since_epoch().count();
    static mt19937 generator(seed);
    static uniform_int_distribution<int> distribution;

    distribution.param(uniform_int_distribution<int>::param_type(A, B));
   
    return distribution(generator);
}

void fill_array(int array[], int n)
{
    for(int i = 0; i < n; i++)
    {
        array[i] = mersenne_generator(1, n);
    }
}

data insertion_sort(int array[], int n, int k)
{
    data simulationData[SIMULATION_REPETITIONS];
    int i, j, key;


    simulationData[k].comparisons = 0;
    simulationData[k].swaps = 0;


    auto start = Clock::now();

    for(i = 1; i < n; i++)
    {
        key = array[i];
        j = i - 1;
        simulationData[k].comparisons++;

        while(j >= 0 && array[j] > key)
        {
            array[j+1] = array[j];
            j--;
            simulationData[k].comparisons++;
            simulationData[k].swaps++;
        }
        array[j+1] = key;
    }

    auto finish = Clock::now();


    duration<double> time_elapsed = finish - start;
    simulationData[k].duration = time_elapsed.count();
    
    return simulationData[k];
}

void simulation(int array[], int n)
{
    data simulationData[SIMULATION_REPETITIONS];
    ofstream csvFile;
    int k;


    csvFile.open("SimulationDat12aK" + to_string(SIMULATION_REPETITIONS) + ".csv");

    csvFile << "n; comparisons";
    for(k = 0; k < SIMULATION_REPETITIONS; k++)
    {
        csvFile << "; ";
    }
    csvFile << "swaps";
    for(k = 0; k < SIMULATION_REPETITIONS; k++)
    {
        csvFile << "; ";
    }
    csvFile << "duration;\n";


    auto start = Clock::now();

    for(int i = 1; i <= 100; i++)
    {
        csvFile << n * i << "; ";

        for(k = 0; k < SIMULATION_REPETITIONS; k++)
        {
            simulationData[k] = {0, 0, 0.0};
            fill_array(array, n * i);
            simulationData[k] = insertion_sort(array, n * i, k);  
        }
        
        for(k = 0; k < SIMULATION_REPETITIONS; k++)
        {
            csvFile << simulationData[k].comparisons << "; ";
        }

        for(k = 0; k < SIMULATION_REPETITIONS; k++)
        {
            csvFile << simulationData[k].swaps << "; ";
        }

        for(k = 0; k < SIMULATION_REPETITIONS; k++)
        {
            csvFile.precision(6);
            csvFile << fixed << simulationData[k].duration << "; ";
        }
        csvFile << "\n";              
    }

    auto finish = Clock::now();


    csvFile.close();


    duration<double> time_elapsed = finish - start;
    double duration = time_elapsed.count();

    cout << "\nSIMULATION TIME ELAPSED FOR " << SIMULATION_REPETITIONS << " REPETITION(S): " << duration << "s\n\n"; 
}



int main()
{
    int array[10000];

    simulation(array, GENERATED_NUMBERS_BASE);

    return 0;
}