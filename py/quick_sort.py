import csv
import time
import sys

sys.setrecursionlimit(10**8)

def read_dataset(filename):
    """Reads a slice of the dataset, returning a list of (int, str) tuples."""
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if row:
                data.append((int(row[0]), row[1]))
    return data

def write_sorted_data(filename, sorted_data):
    """Writes the sorting steps to a file."""
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        for item in sorted_data:
            writer.writerow(item)

def quick_sort(arr, low, high):
    """Performs quick sort on array."""
    if low < high:
        pi = partition(arr, low, high)
        quick_sort(arr, low, pi - 1)
        quick_sort(arr, pi + 1, high)

def median_of_three(arr, low, high):
    """
    Selects the median of the first, middle, and last elements as the pivot.
    Moves this pivot to the 'high' position for the partition function.
    """
    mid = (low + high) // 2
    
    # Sort low, mid, high to put median at mid
    if arr[low][0] > arr[mid][0]:
        arr[low], arr[mid] = arr[mid], arr[low]
    if arr[low][0] > arr[high][0]:
        arr[low], arr[high] = arr[high], arr[low]
    if arr[mid][0] > arr[high][0]:
        arr[mid], arr[high] = arr[high], arr[mid]
    
    # Place the pivot (median) at the high position
    arr[mid], arr[high] = arr[high], arr[mid]
    return arr[high][0] # Return the pivot value

def partition(arr, low, high):
    """
    Partitions the array using the chosen pivot.
    """
    pivot = median_of_three(arr, low, high) # Use median-of-three pivot
    i = (low - 1)
    
    for j in range(low, high):
        if arr[j][0] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i] # Swap elements

    arr[i + 1], arr[high] = arr[high], arr[i + 1] # Place pivot in correct position
    return i + 1

if __name__ == "__main__":
    dataset_filename = input("Enter dataset filename (e.g., dataset_1000000.csv): ")
    output_filename_base = dataset_filename.replace("dataset_", "quick_sort_").replace(".csv", "")

    data = read_dataset(dataset_filename)
    
    start_time = time.perf_counter() # Exclude I/O time 
    # Create a mutable copy for sorting as quick_sort modifies in-place
    data_to_sort = list(data)
    quick_sort(data_to_sort, 0, len(data_to_sort) - 1)
    end_time = time.perf_counter()

    running_time = end_time - start_time
    print(f"Quick sort running time: {running_time:.6f} seconds") # Print running time

    # Dynamically set the output filename based on input filename
    output_filename = f"{output_filename_base}.csv"
    write_sorted_data(output_filename, data_to_sort)
    print(f"Sorted data saved to {output_filename}")
