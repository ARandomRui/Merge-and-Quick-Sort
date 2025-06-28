#include <bits/stdc++.h> 
/*Libraries: iostream, fstream, string, vector, sstream
NOTE: PLEASE USE G++ COMPILER OR U MIGHT NOT RUN THIS PROGRAM*/
using namespace std;

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
        string key_str, value;
        if (getline(ss, key_str, ',') && getline(ss, value)) {
            try {
                int key = stoi(key_str);
                records.push_back({key, value});
            } catch (...) {
                cerr << "Skipping malformed line: " << line << endl;
            }
        }
    }
    return records;
}

void save_dataset(const vector<Record>& dataset, const string& filename) {
    ofstream file(filename); // open file in write mode
    for (size_t i = 0; i < dataset.size(); ++i) {
        file << dataset[i].key << "," << dataset[i].value;
        if (i + 1 != dataset.size()) file << '\n'; // Add newline for all but the last line
    }
}

void merge(vector<Record>& data, vector<Record>& temp, int left, int mid, int right) {
    //More efficient compared to creating new vector in like lecture slides
	int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (data[i].key <= data[j].key)
            temp[k++] = data[i++];
        else
            temp[k++] = data[j++];
    }
    while (i <= mid) temp[k++] = data[i++];
    while (j <= right) temp[k++] = data[j++];
    for (int l = left; l <= right; ++l) data[l] = temp[l];
}

// The recursive merge_sort function
void merge_sort(vector<Record>& data, vector<Record>& temp, int left, int right) {
    if (left < right) {//left is basically the start, 0 and right is the end, so in 
                        //most of the case right will always be bigger than
                        //left.. unless they're equal due to the mid equation
        int mid = (left + right) / 2; //so if the total is 5 it becomes 5/2 = 2 (cpp ignores decimals) 
        merge_sort(data, temp, left, mid);
        merge_sort(data, temp, mid + 1, right);
        merge(data, temp, left, mid, right);
    }
}

int main() {
    string dataset_input_filename;
    cout << "Enter dataset filename (e.g., dataset_1000000.csv): ";
    cin >> dataset_input_filename;

    // Read the dataset
    vector<Record> dataset = read_dataset(dataset_input_filename);
    
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

	vector<Record> temp(dataset.size());

    // Measure running time
    auto start_timer = chrono::high_resolution_clock::now();

    merge_sort(dataset, temp, 0, dataset.size() - 1);

    auto end_timer = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
    cout << "Merge sort running time: " << duration.count() << " ms" << endl;

    // Save the sorted dataset
    save_dataset(dataset, output_filename);
    cout << "Sorted data saved to " << output_filename << endl;

    return 0;
}