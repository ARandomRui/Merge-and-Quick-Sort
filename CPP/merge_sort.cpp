#include <bits/stdc++.h> 
/*Libraries: iostream, fstream, string, vector, sstream
NOTE: PLEASE USE G++ COMPILER OR U MIGHT NOT RUN THIS PROGRAM*/
using namespace std;

vector<string> split(const string& str) {
    vector<string> parts;
    stringstream ss(str);
    string part;
    
    // Get the integer part before the first comma
    getline(ss, part, ',');
    parts.push_back(part); 
    
    // Get the string part (rest of the line)
    getline(ss, part);     
    parts.push_back(part);
    return parts;
}

vector<vector<string>> read_dataset(const string& filename) {
    vector<vector<string>> lines;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return lines;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) { // Skip empty lines
            continue;
        }
        vector<string> line_split = split(line);
        if (line_split.size() == 2) { // Ensure line has both integer and string
            lines.push_back(line_split);
        } else {
            cerr << "Warning: Skipping malformed line: " << line << endl;
        }
    }

    file.close();
    return lines;
}

void save_dataset(const vector<vector<string>>& dataset, const string& output_filename) {
    ofstream file(output_filename); // open file in write mode

    if (!file.is_open()) {
        cerr << "Error: Could not open output file " << output_filename << endl;
        return;
    }

    for (size_t i = 0; i < dataset.size(); ++i) {
        file << dataset[i][0] << "," << dataset[i][1];
        if (i < dataset.size() - 1) { // Add newline for all but the last line
            file << endl;
        }
    }
    file.close();
}

void merge(vector<vector<string>> &dataset, int left, int mid, int right) {
    // Create temporary arrays for the two halves
    vector<vector<string>> L_half;
    for (int i = left; i <= mid; ++i) {
        L_half.push_back(dataset[i]);
    }
    vector<vector<string>> R_half;
    for (int i = mid + 1; i <= right; ++i) {
        R_half.push_back(dataset[i]);
    }

    int i = 0, j = 0, k = left;

    while (i < L_half.size() && j < R_half.size()) {
        // Compare based on integer value (first element of the pair)
        if (stoi(L_half[i][0]) <= stoi(R_half[j][0])){ 
            dataset[k] = L_half[i];
            i++;
        } else {
            dataset[k] = R_half[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L_half[], if any
    while (i < L_half.size()) {
        dataset[k] = L_half[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R_half[], if any
    while (j < R_half.size()) {
        dataset[k] = R_half[j];
        j++;
        k++;
    }
}

// The recursive merge_sort function
void merge_sort(vector<vector<string>> &dataset, int left, int right) {
    if (left < right){//left is basically the start, 0 and right is the end, so in 
                        //most of the case right will always be bigger than
                        //left.. unless they're equal due to the mid equation
        int mid = left + (right - left) / 2; //so if its X.5 it becomes X 5/2 = 2 
        merge_sort(dataset, left, mid);
        merge_sort(dataset, mid + 1, right);
        merge(dataset, left, mid, right);
    }
}

int main() {
    string dataset_input_filename;
    cout << "Enter dataset filename (e.g., dataset_1000000.csv): ";
    cin >> dataset_input_filename;

    // Read the dataset
    vector<vector<string>> dataset = read_dataset(dataset_input_filename);
    
    if (dataset.empty()) {
        cerr << "No data read. Exiting." << endl;
        return 1;
    }

    // Prepare output filename (e.g., merge_sort_1000000.csv)
    // Find the position of "dataset_" and ".csv" to replace dynamically
    string output_filename_base = dataset_input_filename;
    size_t dataset_pos = output_filename_base.find("dataset_");
    if (dataset_pos != string::npos) {
        output_filename_base.replace(dataset_pos, string("dataset_").length(), "merge_sort_");
    } else {
        // Fallback if "dataset_" is not found, just prepend "merge_sort_"
        output_filename_base = "merge_sort_" + output_filename_base;
    }
    string output_filename = output_filename_base; // This will likely be "merge_sort_N.csv"

    cout << "Dataset size: " << dataset.size() << " elements." << endl;

    // Measure running time
    auto start_timer = chrono::high_resolution_clock::now();

    merge_sort(dataset, 0, dataset.size() - 1);

    auto end_timer = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
    cout << "Merge sort running time: " << duration.count() << " ms" << endl;

    // Save the sorted dataset
    save_dataset(dataset, output_filename);
    cout << "Sorted data saved to " << output_filename << endl;

    return 0;
}