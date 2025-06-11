import csv
import time
import random

def read_sorted_dataset(filename):
    """Reads a sorted dataset, returning a list of (int, str) tuples."""
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if row:
                data.append((int(row[0]), row[1]))
    return data

def binary_search_time(arr, target):
    """Performs a single binary search."""
    low = 0
    high = len(arr) - 1

    while low <= high:
        mid = (low + high) // 2
        current_element = arr[mid]

        if current_element[0] == target:
            return mid
        elif current_element[0] < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1

def run_multiple_searches(arr, num_searches):
    """
    Performs 'num_searches' binary searches to get average time.
    For best/worst case, specific targets are chosen.
    """
    n = len(arr)
    
    # Best case: Target at the middle of the array
    best_case_target = arr[n // 2][0] if n > 0 else -1
    
    # Worst case: Target at either end or a non-existent value
    # We can choose values just outside the range or at the extremes.
    worst_case_targets = []
    if n > 0:
        worst_case_targets.append(arr[0][0]) # Smallest element
        worst_case_targets.append(arr[n-1][0]) # Largest element
        # A value known not to be in the array, outside the range
        worst_case_targets.append(arr[0][0] - 1)
        worst_case_targets.append(arr[n-1][0] + 1)
    else:
        worst_case_targets.append(1) # arbitrary for empty array

    # Average case: Randomly selected targets from the dataset
    average_case_targets = [random.choice(arr)[0] for _ in range(num_searches)] if n > 0 else []

    # Include some non-existent targets for average case as well
    for _ in range(num_searches // 10): # Add 10% non-existent targets
        if n > 0:
            random_non_existent = random.randint(arr[0][0], arr[n-1][0])
            while any(item[0] == random_non_existent for item in arr): # Ensure it's not present
                random_non_existent = random.randint(arr[0][0], arr[n-1][0])
            average_case_targets.append(random_non_existent)
        else:
            average_case_targets.append(random.randint(1, 100)) # arbitrary

    results = {}

    # Measure best case
    if n > 0:
        start_time = time.perf_counter()
        binary_search_time(arr, best_case_target)
        end_time = time.perf_counter()
        results["best_case_time"] = end_time - start_time
    else:
        results["best_case_time"] = 0.0

    # Measure worst case (average of few worst case scenarios)
    worst_times = []
    for target in worst_case_targets:
        start_time = time.perf_counter()
        binary_search_time(arr, target)
        end_time = time.perf_counter()
        worst_times.append(end_time - start_time)
    results["worst_case_time"] = sum(worst_times) / len(worst_times) if worst_times else 0.0


    # Measure average case by performing N searches 
    total_avg_time = 0.0
    for target in average_case_targets:
        start_time = time.perf_counter()
        binary_search_time(arr, target)
        end_time = time.perf_counter()
        total_avg_time += (end_time - start_time)
    results["average_case_time"] = total_avg_time / num_searches if num_searches > 0 else 0.0

    return results

if __name__ == "__main__":
    dataset_filename = input("Enter sorted dataset filename (e.g., merge_sort_1000000.csv): ")

    data = read_sorted_dataset(dataset_filename)
    n_searches = len(data) # Perform n searches where n is dataset size 

    # Handle cases where n is very small (e.g., less than 10 for meaningful stats)
    if n_searches < 100 and n_searches > 0: # Ensure enough searches for a reasonable average
        print("Dataset is too small to get meaningful average/worst case times by performing n searches. Adjusting n_searches to 1000 for demonstration.")
        n_searches = 1000
    elif n_searches == 0:
        print("Dataset is empty. No searches can be performed.")
        exit()


    results = run_multiple_searches(data, n_searches)

    output_filename = f"binary_search_{len(data)}.txt" 
    with open(output_filename, 'w') as f:
        f.write(f"Binary Search Performance for {len(data)} elements:\n")
        f.write(f"Best Case Time: {results['best_case_time']:.9f} seconds\n")
        f.write(f"Average Case Time (over {n_searches} searches): {results['average_case_time']:.9f} seconds\n")
        f.write(f"Worst Case Time: {results['worst_case_time']:.9f} seconds\n")
    print(f"Binary search performance saved to {output_filename}")