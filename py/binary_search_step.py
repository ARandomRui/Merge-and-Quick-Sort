import csv

def read_sorted_dataset(filename):
    """Reads a sorted dataset, returning a list of (int, str) tuples."""
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if row:
                data.append((int(row[0]), row[1]))
    return data

def write_search_path(filename, search_path):
    """Writes the binary search path to a file."""
    with open(filename, 'w') as f:
        for item in search_path:
            f.write(str(item) + '\n')

def binary_search_step(arr, target, steps_list):
    
    low = 0
    high = len(arr) - 1

    while low <= high:
        mid = (low + high) // 2
        current_element = arr[mid]
        steps_list.append(f"{mid}: {current_element[0]}/{current_element[1]}")

        if current_element[0] == target:
            return mid # Target found
        elif current_element[0] < target:
            low = mid + 1
        else:
            high = mid - 1
            
    steps_list.append("-1") # Target not found [cite: 44]
    return -1 # Target not found

if __name__ == "__main__":
    dataset_filename = input("Enter sorted dataset filename (e.g., quick_sort_1000.csv): ")
    target_value = int(input("Enter target integer to search for: "))

    data = read_sorted_dataset(dataset_filename)
    search_path = []
    
    result_index = binary_search_step(data, target_value, search_path)

    output_filename = f"binary_search_step_{target_value}.txt"
    write_search_path(output_filename, search_path)

    if result_index != -1:
        print(f"Target {target_value} found at index {result_index}.")
    else:
        print(f"Target {target_value} not found.")
    print(f"Binary search steps saved to {output_filename}")