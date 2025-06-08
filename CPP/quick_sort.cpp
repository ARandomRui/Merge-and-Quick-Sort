#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

string filename = "quick_sort";
int printout_count = 0;

vector<string> split(const string& str) {
    vector<string> lines;
    stringstream ss(str);
    string line_part;

    while (getline(ss, line_part, ',')) {
        lines.push_back(line_part);
    }

    return lines;
}

vector<vector<string>> read_dataset(int start, int end, const string& filename = "dataset_1000000.csv") {
    vector<vector<string>> lines;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return lines;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        count++;
        if (line.empty()) {  // Skip empty lines although it should not happen
            continue;
        }
          if (count >= start) {
            vector<string> line_split = split(line);
            lines.push_back(line_split);
        }
        if (count >= end) {
            break;
        }
    }

    file.close();
    return lines;
}

void save_dataset(vector<vector<string>> &dataset) {
    ofstream file(filename, ios::app);  // open file in append mode

    for (int i = 0; i < dataset.size() - 1; i++) {
        file << dataset[i][0] << "/" << dataset[i][1] << endl;
    }
    file << dataset[dataset.size()-1][0] << "/" << dataset[dataset.size()-1][1];
    file.close(); 
}

int partition(vector<vector<string>> &dataset, int left, int right) {
    vector<vector<string>> L, E, G;
    int p = stoi(dataset[right][0]);  // Pivot is the last element
    int e;
    for (int i = left; i < right; i++) {
        e = stoi(dataset[i][0]);
        if (e < p)
            L.push_back(dataset[i]);
        else if (e == p)
            E.push_back(dataset[i]);
        else
            G.push_back(dataset[i]);
    }
    E.push_back(dataset[right]); //put back the pivot to the last element
    // Put everything back into S in order: L + E + G
    int index = left;
    for (const auto& item : L)
        dataset[index++] = item;

    int pi = index; // Start of E

    // Copy E
    for (const auto& item : E)
        dataset[index++] = item;

    // Copy G
    for (const auto& item : G)
        dataset[index++] = item;


    return pi; // pivot index in the new S
}

void quick_sort(vector<vector<string>> &dataset, int left, int right) {
    if (left < right){  //left is basically the start, 0 and right is the end, so in 
                        //most of the case right will always be bigger than
                        //left.. unless they're equal due to the mid equation 
        int pi = partition(dataset, left, right);
        quick_sort(dataset, left, pi-1);
        quick_sort(dataset, pi+1, right);
    }
}


int main() {

    // Ask user to read from which line till which line
    int start, end;
    cout << "Enter the starting line: ";
    cin >> start;
    cout << "Enter the ending line: ";
    cin >> end;

    // Read the dataset and put it into a vector
    vector<vector<string>> dataset = read_dataset(start,end);
    
    // Print the total number of lines read
    cout << "Lines Captured: " << dataset.size() << endl;
    
    // Print starting and ending lines
    cout << "\nDataset starting from line:" << dataset[0][0] << "," << dataset[0][1] << endl;
    cout << "Dataset ending at line:" << dataset[dataset.size()-1][0] << "," << dataset[dataset.size()-1][1] << endl;

    filename = "quick_sort_" + to_string(dataset.size()) + ".csv";

    auto start_timer = chrono::high_resolution_clock::now();


    quick_sort(dataset,0,dataset.size()-1);

    auto end_timer = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
    cout << "Running time: " << duration.count() << " ms" << endl;

    save_dataset(dataset);

    return 0;
} 