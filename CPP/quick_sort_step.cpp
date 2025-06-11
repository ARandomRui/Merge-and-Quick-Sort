#include <bits/stdc++.h> 
/*Libraries: iostream, fstream, string, vector, sstream
NOTE: PLEASE USE G++ COMPILER OR U MIGHT NOT RUN THIS PROGRAM*/
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

int partition(vector<vector<string>> &dataset, int low, int high) {
    // Pivot is the last element's integer value
    int pivot_val = stoi(dataset[high][0]);
    int i = (low - 1); // Index of smaller element

    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (stoi(dataset[j][0]) <= pivot_val) {
            i++; // Increment index of smaller element
            // Swap elements
            swap(dataset[i], dataset[j]);
        }
    }
    // Swap pivot to its correct position
    swap(dataset[i + 1], dataset[high]);

    // Record the array state after partitioning
    // Match Python's format: "pi = X" then the array state
    step_output_file << "pi = " << (i + 1) << "\n";
    print_dataset_state(dataset);

    return i + 1; // Return the partitioning index
}

// The recursive quick_sort function
void quick_sort_steps(vector<vector<string>> &dataset, int low, int high) {
    if (low < high){
        int pi = partition(dataset, low, high); // Partitioning index
        quick_sort_steps(dataset, low, pi - 1);       // Sort elements before partition
        quick_sort_steps(dataset, pi + 1, high);      // Sort elements after partition
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

    // Read the specified slice from the user-provided filename
    vector<vector<string>> dataset_slice = read_dataset_slice(start_row, end_row, dataset_input_filename);
    
    if (dataset_slice.empty()) {
        cerr << "No data read or file not found. Exiting." << endl;
        return 1;
    }

    // Construct the output filename as required
    string output_filename = "quick_sort_step_" + to_string(start_row) + "_" + to_string(end_row) + ".txt";
    step_output_file.open(output_filename); // Open the global file stream

    if (!step_output_file.is_open()) {
        cerr << "Error: Could not open output file " << output_filename << endl;
        return 1;
    }

    cout << "Lines Captured: " << dataset_slice.size() << endl;
    cout << "\nInitial Dataset Slice will be printed as the first step.\n";
    
    // Print the initial state of the dataset slice as the first step
    print_dataset_state(dataset_slice);

    // Perform quick sort on the slice. The partition function will handle printing steps.
    quick_sort_steps(dataset_slice, 0, dataset_slice.size() - 1);
    
    cout << "\nQuick sort steps saved to " << output_filename << endl;

    step_output_file.close(); // Close the output file after all steps are written

    return 0;
}