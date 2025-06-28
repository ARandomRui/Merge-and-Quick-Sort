import csv
import time
import random
import os

# A class to hold the benchmark results
class SearchResults:
    def __init__(self):
        self.best_case_time = 0.0
        self.worst_case_time = 0.0
        self.average_case_time = 0.0

def read_sorted_dataset(filename: str) -> list[tuple[int, str]]:
    """
    Reads a sorted dataset from a CSV file.
    Args:
        filename: The name of the file to read.
    Returns:
        A list of (int, string) pairs.
    """
    data = []
    try:
        with open(filename, 'r', newline='', encoding='utf-8') as file:
            reader = csv.reader(file)
            for i, row in enumerate(reader):
                if not row:
                    continue
                try:
                    key = int(row[0])
                    value = row[1] if len(row) > 1 else ""
                    data.append((key, value))
                except ValueError:
                    print(f"Warning: Could not convert '{row[0]}' to int in row {i+1}. Skipping row.")
                except IndexError:
                    print(f"Warning: Row {i+1} does not have enough columns. Skipping row.")
    except FileNotFoundError:
        print(f"Error: Could not open file {filename}")
    return data

def binary_search(arr: list[tuple[int, str]], target: int) -> int:
    """
    Performs a single binary search on the dataset.
    Args:
        arr: The sorted dataset (list of (int, str) tuples).
        target: The integer key to search for.
    Returns:
        The index of the target, or -1 if not found.
    """
    low = 0
    high = len(arr) - 1

    while low <= high:
        mid = low + (high - low) // 2  # Avoids potential overflow
        current_key = arr[mid][0]

        if current_key == target:
            return mid
        elif current_key < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1

def run_multiple_searches(arr: list[tuple[int, str]], num_searches: int) -> SearchResults:
    """
    Performs multiple binary searches to measure average time for different cases.
    Args:
        arr: The sorted dataset.
        num_searches: The number of searches to perform for the average case.
    Returns:
        A SearchResults object with the timing information.
    """
    results = SearchResults()
    n = len(arr)

    if n == 0:
        return results

    # --- Best Case: Target is the middle element ---
    best_case_target = arr[n // 2][0]
    start = time.perf_counter()
    binary_search(arr, best_case_target)
    end = time.perf_counter()
    results.best_case_time = end - start

    # --- Worst Case: Target at ends or non-existent ---
    worst_case_targets = []
    worst_case_targets.append(arr[0][0])            # Smallest element
    worst_case_targets.append(arr[n - 1][0])        # Largest element
    worst_case_targets.append(arr[0][0] - 1)        # Non-existent (smaller)
    worst_case_targets.append(arr[n - 1][0] + 1)    # Non-existent (larger)
    
    total_worst_time = 0.0
    for target in worst_case_targets:
        start = time.perf_counter()
        binary_search(arr, target)
        end = time.perf_counter()
        total_worst_time += (end - start)
    results.worst_case_time = total_worst_time / len(worst_case_targets)

    # --- Average Case: Randomly selected targets ---
    average_case_targets = []
    
    # Add targets from the dataset
    for _ in range(num_searches):
        average_case_targets.append(arr[random.randint(0, n - 1)][0])
    
    # Add 10% non-existent targets
    if n > 1: # Ensure there's a range to pick from
        min_val = arr[0][0]
        max_val = arr[n-1][0]
        for _ in range(num_searches // 10):
            while True:
                random_non_existent = random.randint(min_val, max_val)
                # Simple check to try and get a non-existent number.
                # For very dense datasets, this might loop a lot.
                if binary_search(arr, random_non_existent) == -1:
                    average_case_targets.append(random_non_existent)
                    break
    
    total_avg_time = 0.0
    for target in average_case_targets:
        start = time.perf_counter()
        binary_search(arr, target)
        end = time.perf_counter()
        total_avg_time += (end - start)
    results.average_case_time = total_avg_time / len(average_case_targets)

    return results

if __name__ == "__main__":
    dataset_filename = input("Enter sorted dataset filename (e.g., merge_sort_1000000.csv): ")

    data = read_sorted_dataset(dataset_filename)
    if not data:
        print("Dataset is empty or could not be read. Exiting.")
        exit(1)
    
    n_searches = len(data)

    # Handle cases where n is very small for meaningful stats
    if n_searches < 100 and n_searches > 0:
        print("Dataset is too small to get meaningful average/worst case times by performing n searches. Adjusting n_searches to 1000 for demonstration.")
        n_searches = 1000

    results = run_multiple_searches(data, n_searches)

    output_filename = f"binary_search_{len(data)}.txt"
    try:
        with open(output_filename, 'w', encoding='utf-8') as outfile:
            outfile.write(f"Binary Search Performance for {len(data)} elements:\n")
            outfile.write(f"Best Case Time: {results.best_case_time:.9f} seconds\n")
            outfile.write(f"Average Case Time (over {n_searches} searches): {results.average_case_time:.9f} seconds\n")
            outfile.write(f"Worst Case Time: {results.worst_case_time:.9f} seconds\n")
        print(f"Binary search performance saved to {output_filename}")
    except IOError as e:
        print(f"Error: Could not write to output file {output_filename}. {e}")

