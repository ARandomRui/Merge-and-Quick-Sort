import csv
import time

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

def merge_sort(arr):
    """Performs merge sort on array."""
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left_half = arr[:mid]
    right_half = arr[mid:]

    left_half = merge_sort(left_half)
    right_half = merge_sort(right_half)

    return merge(left_half, right_half)

def merge(left, right):
    """Merges two sorted lists."""
    result = []
    i = j = 0

    while i < len(left) and j < len(right):
        if left[i][0] < right[j][0]: # Compare based on integer value
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    result.extend(left[i:])
    result.extend(right[j:])
    return result

if __name__ == "__main__":
    dataset_filename = input("Enter dataset filename (e.g., dataset_1000000.csv): ")
    output_filename_base = dataset_filename.replace("dataset_", "merge_sort_").replace(".csv", "")

    data = read_dataset(dataset_filename)
    
    start_time = time.perf_counter() # Exclude I/O time
    sorted_data = merge_sort(data)
    end_time = time.perf_counter()

    running_time = end_time - start_time
    print(f"Merge sort running time: {running_time:.6f} seconds") # Print running time

    # Dynamically set the output filename based on input filename
    output_filename = f"{output_filename_base}.csv"
    write_sorted_data(output_filename, sorted_data)
    print(f"Sorted data saved to {output_filename}")