#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

string filename_output = "binary_search";
int printout_count = 0;

vector<string> split(const string& str) {
    vector<string> lines;
    stringstream ss(str);
    string line_part;

    while (getline(ss, line_part, '/')) {
        lines.push_back(line_part);
    }

    return lines;
}

vector<vector<string>> read_dataset(string &file_name) {
    vector<vector<string>> lines;
    ifstream file(file_name);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_name << endl;
        return lines;
    }

    string line;

    while (getline(file, line)) {
        if (line.empty()) {  // Skip empty lines although it should not happen
            continue;
        }
        vector<string> line_split = split(line);
        lines.push_back(line_split);
    }

    file.close();
    return lines;
}



void binary_search(vector<vector<string>> &dataset, int target) {
    int left = 0;
    int right = dataset.size() - 1;
    int mid;
    int checkint;
    while (left <= right) {
        mid = (left + right) / 2;
        checkint = stoi(dataset[mid][0]);
        if (checkint == target) {
            cout << "Target found at index: " << mid << endl;
            return;
        }
        else if (checkint < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
}


int main() {

    //Enter file name
    string file_name;
    cout << "Enter the file name: ";
    cin >> file_name;

    // Read the dataset and put it into a vector
    vector<vector<string>> dataset = read_dataset(file_name);

    if (dataset.empty()) {
        cerr << "Error: Dataset is empty." << endl;
        return 1;
    }
    
    // Print the total number of lines read
    cout << "Lines Captured: " << dataset.size() << endl;
    
    // Print starting and ending lines
    cout << "\nDataset starting from line:" << dataset[0][0] << "," << dataset[0][1] << endl;
    cout << "Dataset ending at line:" << dataset[dataset.size()-1][0] << "," << dataset[dataset.size()-1][1] << endl;

    filename_output = "binary_search_" + to_string(dataset.size()) + ".txt";

    //Enter the target
    int target;
    cout << "Enter the target value: ";
    cin >> target;

    // Measure runtime
    auto start_time = chrono::high_resolution_clock::now();

    binary_search(dataset, target);

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    //Save runtime 
    ofstream file(filename_output, ios::app);
    file << "Runtime: " << duration.count() << " microseconds" << endl; //To get best case get the middle element, get the first or last element to get worst case.
    file.close();

    return 0;
} 