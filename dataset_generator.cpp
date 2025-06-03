#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

using namespace std;

// Structure to hold dataset settings
struct Settings {
    int start_number;
    int end_number;
    int string_length_min;
    int string_length_max;
};

// Function to generate random string, given min max of string length
string generate_random_string(int length_min, int length_max) {
    const string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    random_device seed; //Creates a seed
    mt19937 gen(seed()); //Creates a random number generator
    uniform_int_distribution<> dist(0, letters.size() - 1); //So that it will choose a number between 0 and the size of letters
    
    string result;
    for(int i = 0; i < length_min; ++i) {
        result += letters[dist(gen)];
    }
    for (int i = length_min; i < length_max; ++i) {
        if (rand()%2 == 0) {
            result += letters[dist(gen)];
        }
    }
    return result;
}

// Function to generate random number, given min max of number
string generate_random_number(int start, int end) {
    random_device seed;
    mt19937 gen(seed());
    uniform_int_distribution<> dist(start, end);
    return to_string(dist(gen));
}

// Function to generate dataset
void generate_dataset(const Settings& settings, const string& output_file) {
    int num_to_generate;
    cout << "Enter the number of records to generate: ";
    cin >> num_to_generate;

    ofstream file(output_file);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << output_file << endl;
        return;
    }

    for(int i = 0; i < num_to_generate; ++i) {
        string random_number = generate_random_number(settings.start_number, settings.end_number);
        string random_string = generate_random_string(settings.string_length_min, settings.string_length_max);
        
        file << random_number << "," << random_string << "\n";
    }

    file.close();
}

// Function to set custom settings
Settings setting_dataset_type() {
    Settings settings;
    cout << "Enter the start number: ";
    cin >> settings.start_number;
    cout << "Enter the end number: ";
    cin >> settings.end_number;
    cout << "Enter the minimum length of the string: ";
    cin >> settings.string_length_min;
    cout << "Enter the maximum length of the string: ";
    cin >> settings.string_length_max;
    return settings;
}

int main() {
    // Default settings
    Settings default_settings = {1, 1000000000, 4, 5}; //1 to 1000000000, 4 to 5 characters
    
    cout << "Enter the type of dataset to generate:\n"
         << "1. default (assignment)\n"
         << "2. settings\n"
         << "Input: ";
    
    string generate_type;
    cin >> generate_type;

    Settings settings = default_settings;
    if (generate_type == "2") {
        settings = setting_dataset_type();
    }
    //if 1, generate a dataset with default settings
    generate_dataset(settings, "dataset_1000000.csv");
    cout << "Dataset generated successfully in dataset_1000000.csv" << endl;

    return 0;
} 