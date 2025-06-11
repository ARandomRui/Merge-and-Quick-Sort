#include <bits/stdc++.h> 
/*Libraries: iostream, fstream, string, vector, sstream
NOTE: PLEASE USE G++ COMPILER OR U MIGHT NOT RUN THIS PROGRAM*/
using namespace std;

string filename = "quick_sort";
int printout_count = 0;

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
    return i + 1; // Return the partitioning index
}

// The recursive quick_sort function
void quick_sort(vector<vector<string>> &dataset, int low, int high) {
    if (low < high){
        int pi = partition(dataset, low, high); // Partitioning index
        quick_sort(dataset, low, pi - 1);       // Sort elements before partition
        quick_sort(dataset, pi + 1, high);      // Sort elements after partition
    }
}


int main() {
    string dataset_input_filename;
    cout << "Enter dataset filename (e.g., dataset_1000000.csv): ";
    cin >> dataset_input_filename;

    // Read the dataset
    vector<vector<string>> dataset = read_dataset(dataset_input_filename);
    
    if (dataset.empty()) {
        cerr << "No data read or file not found. Exiting." << endl;
        return 1;
    }

    // Prepare output filename (e.g., quick_sort_1000000.csv)
    // Find the position of "dataset_" and ".csv" to dynamically create the output filename
    string output_filename = dataset_input_filename;
    size_t dataset_prefix_pos = output_filename.find("dataset_");
    if (dataset_prefix_pos != string::npos) {
        output_filename.replace(dataset_prefix_pos, string("dataset_").length(), "quick_sort_");
    } else {
        // Fallback if "dataset_" prefix is not found
        output_filename = "quick_sort_" + output_filename;
    }

    cout << "Dataset size: " << dataset.size() << " elements." << endl;

    // Measure running time
    auto start_timer = chrono::high_resolution_clock::now();

    quick_sort(dataset, 0, dataset.size() - 1); // Call quick_sort on the entire dataset

    auto end_timer = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
    cout << "Quick sort running time: " << duration.count() << " ms" << endl;

    // Save the sorted dataset
    save_dataset(dataset, output_filename);
    cout << "Sorted data saved to " << output_filename << endl;

    return 0;
}