import csv
import time

def read_dataset(filename):
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if row:
                data.append((int(row[0]), row[1]))
    return data

def write_sorted_data(filename, sorted_data):
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        for item in sorted_data:
            writer.writerow(item)

def quick_sort(arr, low, high):
    if low < high:
        pi = partition(arr, low, high)
        quick_sort(arr, low, pi - 1)
        quick_sort(arr, pi + 1, high)

def partition(arr, low, high):
    pivot = arr[high][0] # Pivot based on integer value
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
    
    start_time = time.perf_counter() # Exclude I/O time [cite: 14]
    # Create a mutable copy for sorting as quick_sort modifies in-place
    data_to_sort = list(data)
    quick_sort(data_to_sort, 0, len(data_to_sort) - 1)
    end_time = time.perf_counter()

    running_time = end_time - start_time
    print(f"Quick sort running time: {running_time:.6f} seconds") # Print running time [cite: 22]

    # Dynamically set the output filename based on input filename
    output_filename = f"{output_filename_base}.csv"
    write_sorted_data(output_filename, data_to_sort)
    print(f"Sorted data saved to {output_filename}")