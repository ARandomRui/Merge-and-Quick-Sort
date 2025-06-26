#include <bits/stdc++.h> 
/*Libraries: iostream, fstream, string, vector, sstream
NOTE: PLEASE USE G++ COMPILER OR U MIGHT NOT RUN THIS PROGRAM*/
using namespace std;

string filename = "quick_sort";
int printout_count = 0;

struct Record {
    int key;
    string value;
};

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

vector<Record> read_dataset(const string& filename) {
    vector<Record> records;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string key_str, val;
        if (getline(ss, key_str, ',') && getline(ss, val)) {
            try {
                int key = stoi(key_str);
                records.push_back({key, val});
            } catch (...) {
                cerr << "Warning: Skipping malformed line: " << line << endl;
            }
        }
    }
    return records;
}

void save_dataset(const vector<Record>& dataset, const string& output_filename) {
    ofstream file(output_filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open output file " << output_filename << endl;
        return;
    }

    for (size_t i = 0; i < dataset.size(); ++i) {
        file << dataset[i].key << "," << dataset[i].value;
        if (i < dataset.size() - 1) file << '\n';
    }
}

int partition(vector<Record>& dataset, int low, int high) {
    // Pivot is the last element's integer value
    int pivot_val = dataset[high].key;
    int i = (low - 1); // Index of smaller element

    for (int j = low; j < high; ++j) {
        // If current element is smaller than or equal to pivot
        if (dataset[j].key <= pivot_val) {
            i++; // Increment index of smaller element
            // Swap elements
            swap(dataset[i], dataset[j]);
        }
    }
    // Swap pivot to its correct position
    swap(dataset[i + 1], dataset[high]);
    return i + 1; // Return the partitioning index
}

void quick_sort(vector<Record>& dataset, int low, int high) {
    while (low < high) {
        int pi = partition(dataset, low, high);
        
        // Recurse on smaller half first to reduce stack depth
        if (pi - low < high - pi) {
            quick_sort(dataset, low, pi - 1);
            low = pi + 1; // Tail call optimization
        } else {
            quick_sort(dataset, pi + 1, high);
            high = pi - 1; // Tail call optimization
        }
    }
}


int main() {
    string dataset_input_filename;
    cout << "Enter dataset filename (e.g., dataset_1000000.csv): ";
    cin >> dataset_input_filename;

    // Read the dataset
    vector<Record> dataset = read_dataset(dataset_input_filename);
	
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