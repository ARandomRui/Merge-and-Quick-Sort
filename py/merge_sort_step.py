import csv

def read_dataset_slice(filename, start_row, end_row):
    """Reads a slice of the dataset, returning a list of (int, str) tuples."""
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader):
            if start_row <= i + 1 <= end_row: # +1 because rows are 1-indexed in problem description
                if row: # Ensure row is not empty
                    data.append((int(row[0]), row[1]))
            elif i + 1 > end_row:
                break
    return data

def write_steps_to_file(filename, steps_list):
    """Writes the sorting steps to a file."""
    with open(filename, 'w') as f:
        for step in steps_list:
            f.write(str(step) + '\n')

def merge_sort_steps_recursive(arr, steps_list, original_arr_ref, start_idx_in_original):
    """
    Performs merge sort on arr and records steps.
    Crucially, it updates a reference to the 'original_arr_ref' (the full slice being sorted)
    and appends a copy of that full slice after each merge.
    """
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left_half = arr[:mid]
    right_half = arr[mid:]

    left_half = merge_sort_steps_recursive(left_half, steps_list, original_arr_ref, start_idx_in_original)
    right_half = merge_sort_steps_recursive(right_half, steps_list, original_arr_ref, start_idx_in_original + mid)

    merged_arr = merge(left_half, right_half)

   
    current_start = start_idx_in_original
    current_end = start_idx_in_original + len(merged_arr)

   
    for i in range(len(merged_arr)):
        original_arr_ref[current_start + i] = merged_arr[i]
        
    
    steps_list.append(list(original_arr_ref)) 

    return merged_arr

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
    start_row = int(input("Enter start row (e.g., 1): "))  
    end_row = int(input("Enter end row (e.g., 7): ")) 

    initial_data_slice = read_dataset_slice(dataset_filename, start_row, end_row)
   
    array_in_progress = list(initial_data_slice)
    
    steps = [list(initial_data_slice)]
    print(f"Initial data slice: {initial_data_slice}")
   
    sorted_data_result = merge_sort_steps_recursive(list(initial_data_slice), steps, array_in_progress, 0)
    
    output_filename = f"merge_sort_step_{start_row}_{end_row}.txt" 
    write_steps_to_file(output_filename, steps)
    print(f"Merge sort steps saved to {output_filename}")
    print(f"Final sorted slice: {sorted_data_result}")