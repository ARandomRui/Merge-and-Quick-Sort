#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>   // For measuring time
#include <vector>   // To store integers and allow shuffling
#include <algorithm> // For std::shuffle and std::find
#include <set>

using namespace std;

// Global random number generator
static random_device seed;
static mt19937 gen(seed());

// Function to generate random string with lowercase letters
string generate_random_string(int length_min, int length_max) {
    uniform_int_distribution<> length_dist(length_min, length_max);
    uniform_int_distribution<> char_dist(0, 25); // 'a' through 'z'

    string result = "";
    int length = length_dist(gen);
    for(int i = 0; i < length; ++i) {
        result += (char)('a' + char_dist(gen));
    }
    return result;
}

// Function to generate dataset
void generate_dataset(int size, const string& filename) {

	int num;
    cout << "Generating dataset of size " << size << "..." << endl;

    // Start time measurement
    auto start_time = chrono::high_resolution_clock::now();

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    
    uniform_int_distribution<> int_dist(1, 1000000000);

    cout << "DEBUG: File '" << filename << "' opened successfully for writing." << endl;

    for (int i = 0; i< size; i++) {
        // Generate a random string (e.g., 5-10 lowercase letters)
		num = int_dist(gen);
        string random_string = generate_random_string(5, 10); 
        file << num << "," << random_string << "\n";
    }

    file.close();
    cout << "Dataset saved to " << filename << endl;

    // End time measurement
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    cout << "Dataset generation took " << duration.count() << " milliseconds." << endl;
}


int main() {
    cout << "Enter dataset size (e.g., 1000000): ";
    int dataset_size;
    cin >> dataset_size;

    string output_filename = "dataset_" + to_string(dataset_size) + ".csv";
    generate_dataset(dataset_size, output_filename);

    return 0;
}