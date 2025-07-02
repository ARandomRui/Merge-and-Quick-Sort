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
    """Writes the sorted data to a file."""
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        for item in sorted_data:
            writer.writerow(item)

def merge_sort(dataset, left, right, steps):
    #Basically this keeps seperating the dataset to little segments
    #Afterwards begin merging subarrays from the bottom up
    if left < right:
        mid = (left + right) // 2
        merge_sort(dataset, left, mid, steps)
        merge_sort(dataset, mid + 1, right, steps)
        merge(dataset, left, mid, right, steps)

def merge(dataset, left, mid, right, steps):
    #This arranges the individual element/sorted segments into left and right
    #Then immediately write them back to the dataset
    L_half = dataset[left:mid + 1]
    R_half = dataset[mid + 1:right + 1]

    i = j = 0
    k = left

    while i < len(L_half) and j < len(R_half):
        if L_half[i][0] <= R_half[j][0]:
            dataset[k] = L_half[i]
            i += 1
        else:
            dataset[k] = R_half[j]
            j += 1
        k += 1

    while i < len(L_half):
        dataset[k] = L_half[i]
        i += 1
        k += 1
    while j < len(R_half):
        dataset[k] = R_half[j]
        j += 1
        k += 1

    # Step tracking preserved, though not written out
    steps.append(list(dataset))

if __name__ == "__main__":
    dataset_filename = input("Enter dataset filename (e.g., dataset_1000000.csv): ")
    output_filename_base = dataset_filename.replace("dataset_", "merge_sort_").replace(".csv", "")

    data = read_dataset(dataset_filename)
    steps = [list(data)]  # Initial step

    start_time = time.perf_counter()  # Exclude I/O time
    merge_sort(data, 0, len(data) - 1, steps)
    end_time = time.perf_counter()

    running_time = end_time - start_time
    print(f"Merge sort running time: {running_time:.6f} seconds")

    output_filename = f"{output_filename_base}.csv"
    write_sorted_data(output_filename, data)
    print(f"Sorted data saved to {output_filename}")
