import csv

def read_dataset_slice(filename, start_row, end_row):
    """Reads a slice of the dataset, returning a list of (int, str) tuples."""
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader):
            if start_row <= i + 1 <= end_row:
                if row:
                    data.append((int(row[0]), row[1]))
            elif i + 1 > end_row:
                break
    return data

def write_steps_to_file(filename, steps_list):
    """Writes the sorting steps to a file."""
    with open(filename, 'w') as f:
        for step in steps_list:
            f.write(str(step) + '\n')

def quick_sort_steps(arr, low, high, steps_list):
    """
    Performs quick sort on arr and records steps.
    Steps show the array state after each partition.
    """
    if low < high:
        pi = partition(arr, low, high, steps_list)
        quick_sort_steps(arr, low, pi - 1, steps_list)
        quick_sort_steps(arr, pi + 1, high, steps_list)

def partition(arr, low, high, steps_list):
    """
    Partitions the array using the last element as pivot
    Records the array state after partitioning.
    """
    pivot = arr[high][0] # Pivot based on integer value
    i = (low - 1)
    
    for j in range(low, high):
        if arr[j][0] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i] # Swap elements

    arr[i + 1], arr[high] = arr[high], arr[i + 1] # Place pivot in correct position

    # Record the array state after partitioning
    steps_list.append(f"pi = {i+1}") # Indicate pivot index
    steps_list.append(list(arr)) # Append a copy of the array state

    return i + 1

if __name__ == "__main__":
    dataset_filename = input("Enter dataset filename (e.g., dataset_1000000.csv): ")
    start_row = int(input("Enter start row (e.g., 1): "))
    end_row = int(input("Enter end row (e.g., 7): ")) 

    initial_data = read_dataset_slice(dataset_filename, start_row, end_row)
    steps = [initial_data] # Add the initial state
    print(f"Initial data slice: {initial_data}")

    # Create a mutable copy for sorting
    data_to_sort = list(initial_data)
    quick_sort_steps(data_to_sort, 0, len(data_to_sort) - 1, steps)

    output_filename = f"quick_sort_step_{start_row}_{end_row}.txt"
    write_steps_to_file(output_filename, steps)
    print(f"Quick sort steps saved to {output_filename}")
    print(f"Final sorted slice: {data_to_sort}")