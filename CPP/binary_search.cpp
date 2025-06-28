#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>      // For std::pair
#include <chrono>       // For timing
#include <random>       // For random number generation
#include <numeric>      // For std::accumulate
#include <iomanip>      // For std::fixed and std::setprecision

using namespace std;

// Define type aliases for clarity, similar to Python's structure
using DataRow = std::pair<int, std::string>;
using DataSet = std::vector<DataRow>;

// A struct to hold the benchmark results
struct SearchResults {
    double best_case_time;
    double worst_case_time;
    double average_case_time;
};

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
        
        // Split the line by the comma
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
 * @brief Performs a single binary search on the dataset.
 * @param arr The sorted dataset.
 * @param target The integer key to search for.
 * @return The index of the target, or -1 if not found.
 */
int binary_search(const DataSet& arr, int target) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Avoids potential overflow
        int current_key = arr[mid].first;

        if (current_key == target) {
            return mid;
        } else if (current_key < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

/**
 * @brief Performs multiple binary searches to measure average time for different cases.
 * @param arr The sorted dataset.
 * @param num_searches The number of searches to perform for the average case.
 * @return A SearchResults struct with the timing information.
 */
SearchResults run_multiple_searches(const DataSet& arr, int num_searches) {
    SearchResults results = {0.0, 0.0, 0.0};
    size_t n = arr.size();

    if (n == 0) {
        return results;
    }

    // --- Best Case: Target is the middle element ---
	int best_case_target = arr[n / 2].first;
	const int best_case_trials = 10000;
	double total_best_time = 0.0;
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < best_case_trials; ++i) {
		start = std::chrono::high_resolution_clock::now();
		binary_search(arr, best_case_target);
		end = std::chrono::high_resolution_clock::now();
		total_best_time += std::chrono::duration<double>(end - start).count();
	}

	results.best_case_time = total_best_time / best_case_trials;

    // --- Worst Case: Target at ends or non-existent ---
    std::vector<int> worst_case_targets;
    worst_case_targets.push_back(arr[0].first - 1);        // Non-existent (smaller)
    worst_case_targets.push_back(arr[n - 1].first + 1);    // Non-existent (larger)
    
    double total_worst_time = 0.0;
    for (int target : worst_case_targets) {
        start = std::chrono::high_resolution_clock::now();
        binary_search(arr, target);
        end = std::chrono::high_resolution_clock::now();
        total_worst_time += std::chrono::duration<double>(end - start).count();
    }
    results.worst_case_time = total_worst_time / worst_case_targets.size();

    // --- Average Case: Randomly selected targets ---
    // Setup modern C++ random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, n - 1);
    std::uniform_int_distribution<> value_distrib(arr[0].first, arr[n-1].first);

    std::vector<int> average_case_targets;
    average_case_targets.reserve(num_searches);
    
    for (int i = 0; i < num_searches; ++i) {
        average_case_targets.push_back(arr[distrib(gen)].first);
    }
    // Add 10% non-existent targets
    for (int i = 0; i < num_searches / 10; ++i) {
        int random_non_existent = value_distrib(gen);
        // This simple check is usually sufficient for large datasets.
        // A perfect check would involve ensuring it's not in the dataset.
        if (binary_search(arr, random_non_existent) != -1) {
            i--; // try again
        } else {
             average_case_targets.push_back(random_non_existent);
        }
    }

    double total_avg_time = 0.0;
    for (int target : average_case_targets) {
        start = std::chrono::high_resolution_clock::now();
        binary_search(arr, target);
        end = std::chrono::high_resolution_clock::now();
        total_avg_time += std::chrono::duration<double>(end - start).count();
    }
    results.average_case_time = total_avg_time / average_case_targets.size();

    return results;
}

int main() {
    std::string dataset_filename;
    std::cout << "Enter sorted dataset filename (e.g., merge_sort_1000000.csv): ";
    std::cin >> dataset_filename;

    DataSet data = read_sorted_dataset(dataset_filename);
    if (data.empty()) {
        std::cout << "Dataset is empty or could not be read. Exiting." << std::endl;
        return 1;
    }
    
    int n_searches = data.size();

    // Handle cases where n is very small for meaningful stats
    if (n_searches < 100 && n_searches > 0) {
        std::cout << "Dataset is too small to get meaningful average/worst case times by performing n searches. Adjusting n_searches to 1000 for demonstration." << std::endl;
        n_searches = 1000;
    }

    SearchResults results = run_multiple_searches(data, n_searches);

    std::string output_filename = "binary_search_" + std::to_string(data.size()) + ".txt";
    std::ofstream outfile(output_filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open output file " << output_filename << std::endl;
        return 1;
    }

    outfile << std::fixed << std::setprecision(9);
    outfile << "Binary Search Performance for " << data.size() << " elements:\n";
    outfile << "Best Case Time: " << results.best_case_time << " seconds\n";
    outfile << "Average Case Time (over " << n_searches << " searches): " << results.average_case_time << " seconds\n";
    outfile << "Worst Case Time: " << results.worst_case_time << " seconds\n";
    
    outfile.close();

    std::cout << "Binary search performance saved to " << output_filename << std::endl;

    return 0;
}