#include <bits/stdc++.h>
/*Libraries: iostream, fstream, string, vector, sstream, chrono
NOTE: USE G++ COMPILER OR U MIGHT NOT RUN THIS PROGRAM*/
using namespace std;

ofstream step_output_file;

string format_element(const vector<string>& element) {
    if (element.size() == 2) {
        return element[0] + "/" + element[1];
    }
    return "INVALID_FORMAT"; // Should not happen with correctly parsed input
}

void print_dataset_state(const vector<vector<string>>& dataset_slice) {
    step_output_file << "[";
    for (size_t i = 0; i < dataset_slice.size(); ++i) {
        step_output_file << format_element(dataset_slice[i]);
        if (i < dataset_slice.size() - 1) {
            step_output_file << ", ";
        }
    }
    step_output_file << "]\n";
}

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

vector<vector<string>> read_dataset_slice(int start, int end, const string& filename) {
    vector<vector<string>> lines;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return lines;
    }

    string line;
    int current_line_num = 0;
    while (getline(file, line)) {
        current_line_num++;
        if (line.empty()) { // Skip empty lines
            continue;
        }
        if (current_line_num >= start && current_line_num <= end) {
            vector<string> line_split = split(line);
            if (line_split.size() == 2) { // Ensure line has both integer and string
                lines.push_back(line_split);
            } else {
                cerr << "Warning: Skipping malformed line " << current_line_num << ": " << line << endl;
            }
        }
        if (current_line_num > end) { // Stop reading after the end line
            break;
        }
    }

    file.close();
    return lines;
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

    // After each merge, print the current state of the *entire* dataset slice to the file.
    // This is the core change to match the step-by-step output format.
    print_dataset_state(dataset);
}

// The recursive merge_sort function
void merge_sort(vector<vector<string>> &dataset, int left, int right) {
    if (left < right){
        int mid = left + (right - left) / 2; // Robust way to calculate mid to prevent overflow
        merge_sort(dataset, left, mid);
        merge_sort(dataset, mid + 1, right);
        merge(dataset, left, mid, right);
    }
}

int main() {
    string dataset_input_filename;
    int start_row, end_row;
    cout << "Enter the dataset filename (e.g., dataset_sample_1000.csv): ";
    cin >> dataset_input_filename;
    cout << "Enter the starting row for the slice (e.g., 1): ";
    cin >> start_row;
    cout << "Enter the ending row for the slice (e.g., 7): ";
    cin >> end_row;

    // Read the specified slice from dataset_sample_1000.csv
    vector<vector<string>> dataset_slice = read_dataset_slice(start_row, end_row, dataset_input_filename);
    
    if (dataset_slice.empty()) {
        cerr << "No data read. Exiting." << endl;
        return 1;
    }

    // Construct the output filename as required
    string output_filename = "merge_sort_step_" + to_string(start_row) + "_" + to_string(end_row) + ".txt";
    step_output_file.open(output_filename); // Open the global file stream

    if (!step_output_file.is_open()) {
        cerr << "Error: Could not open output file " << output_filename << endl;
        return 1;
    }

    cout << "Lines Captured: " << dataset_slice.size() << endl;
    cout << "\nInitial Dataset Slice will be printed as the first step.\n";
    
    // Print the initial state of the dataset slice as the first step
    print_dataset_state(dataset_slice);

    // Perform merge sort on the slice. The merge function will handle printing steps.
    merge_sort(dataset_slice, 0, dataset_slice.size() - 1);
    
    cout << "\nMerge sort steps saved to " << output_filename << endl;

    step_output_file.close(); // Close the output file after all steps are written

    return 0;
}