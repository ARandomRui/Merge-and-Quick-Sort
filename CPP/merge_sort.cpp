#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

string filename = "merge_sort";
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
    ofstream file(filename);  // open file in write mode
    for (int i = 0; i < dataset.size()-1; i++) {
        file << dataset[i][0] << "/" << dataset[i][1] << endl;
    }
    file << dataset[dataset.size()-1][0] << "/" << dataset[dataset.size()-1][1];

    file.close(); 
}

void merge(vector<vector<string>> &dataset, int left, int mid, int right) {
    vector<vector<string>> L(dataset.begin() + left, dataset.begin() + mid + 1);
    vector<vector<string>> R(dataset.begin() + mid + 1, dataset.begin() + right + 1);
    int k = left, L_int = 0, R_int = 0;
    while (L.size() > L_int && R.size() > R_int) {
        if (stoi(L[L_int][0]) < stoi(R[R_int][0])){
            dataset[k] = L[L_int];
            L_int++;
        }
        else{
            dataset[k] = R[R_int];
            R_int++;
        }
        k++;
    }
    while (L.size() > L_int) { //fill the dataset with the left vector if still have elements
        dataset[k++] = L[L_int];
        L_int++;
    }       
    while (R.size() > R_int) { //same here
        dataset[k++] = R[R_int];
        R_int++;
    }

    //save_dataset_step(dataset); 
    if (printout_count %1000== 0) {
        cout << "Printout count: " << printout_count << endl;
    }
    printout_count++;
}

void merge_sort(vector<vector<string>> &dataset, int left, int right) {
    if (left < right){  //left is basically the start, 0 and right is the end, so in 
                        //most of the case right will always be bigger than
                        //left.. unless they're equal due to the mid equation
        int mid = (left + right)/2;  //so if its X.5 it becomes X 5/2 = 2 
        merge_sort(dataset, left, mid);
        merge_sort(dataset, mid+1, right);
        merge(dataset, left, mid, right);
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

    filename = "merge_sort_" + to_string(dataset.size()) + ".csv";

    auto start_timer = chrono::high_resolution_clock::now();


    merge_sort(dataset, 0, dataset.size()-1);

    auto end_timer = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
    cout << "Running time: " << duration.count() << " ms" << endl;

    save_dataset(dataset);

    return 0;
} 