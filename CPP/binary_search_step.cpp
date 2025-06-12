#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility> // For std::pair

// Define type aliases for clarity
using DataRow = std::pair<int, std::string>;
using DataSet = std::vector<DataRow>;

/**
 * @brief Reads a sorted dataset from a CSV file.
 * @param filename The name of the file to read.
 * @return A vector of (int, string) pairs.
 */
DataSet read_sorted_dataset(const std::string& filename) {
    DataSet data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string key_str, value;
        
        if (std::getline(ss, key_str, ',') && std::getline(ss, value)) {
            try {
                data.emplace_back(std::stoi(key_str), value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Could not convert '" << key_str << "' to int. Skipping row." << std::endl;
            }
        }
    }
    file.close();
    return data;
}

/**
 * @brief Writes the recorded binary search path to a file.
 * @param filename The name of the output file.
 * @param search_path A vector of strings representing the search steps.
 */
void write_search_path(const std::string& filename, const std::vector<std::string>& search_path) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open output file " << filename << std::endl;
        return;
    }

    for (const auto& item : search_path) {
        outfile << item << '\n';
    }
    outfile.close();
}

/**
 * @brief Performs a binary search and records the steps.
 * @param arr The sorted dataset.
 * @param target The integer key to search for.
 * @param steps_list A vector (passed by reference) to store the search path.
 * @return The index of the target, or -1 if not found.
 */
int binary_search_step(const DataSet& arr, int target, std::vector<std::string>& steps_list) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Avoids potential overflow
        const DataRow& current_element = arr[mid];

        // Record the step
        steps_list.push_back(std::to_string(mid) + ": " + std::to_string(current_element.first) + "/" + current_element.second);

        if (current_element.first == target) {
            return mid; // Target found
        } else if (current_element.first < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    steps_list.push_back("-1"); // Target not found
    return -1; // Target not found
}

int main() {
    std::string dataset_filename;
    int target_value;

    std::cout << "Enter sorted dataset filename (e.g., quick_sort_1000.csv): ";
    std::cin >> dataset_filename;
    
    std::cout << "Enter target integer to search for: ";
    std::cin >> target_value;

    DataSet data = read_sorted_dataset(dataset_filename);
    if (data.empty()) {
        std::cout << "Dataset is empty or could not be read. Exiting." << std::endl;
        return 1;
    }

    std::vector<std::string> search_path;
    int result_index = binary_search_step(data, target_value, search_path);

    std::string output_filename = "binary_search_step_" + std::to_string(target_value) + ".txt";
    write_search_path(output_filename, search_path);

    if (result_index != -1) {
        std::cout << "Target " << target_value << " found at index " << result_index << ".\n";
    } else {
        std::cout << "Target " << target_value << " not found.\n";
    }
    std::cout << "Binary search steps saved to " << output_filename << std::endl;

    return 0;
}