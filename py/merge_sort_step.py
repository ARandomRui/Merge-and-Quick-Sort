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

    # Save current full state after merge
    steps.append(list(dataset))

if __name__ == "__main__":
    dataset_filename = input("Enter dataset filename (e.g., dataset_1000000.csv): ")
    start_row = int(input("Enter start row (e.g., 1): "))  
    end_row = int(input("Enter end row (e.g., 7): ")) 

    initial_data_slice = read_dataset_slice(dataset_filename, start_row, end_row)
   
    array_in_progress = list(initial_data_slice)
    
    steps = [list(initial_data_slice)]
    
    array_in_progress = list(initial_data_slice)
    merge_sort(array_in_progress, 0, len(array_in_progress) - 1, steps)
    
    output_filename = f"merge_sort_step_{start_row}_{end_row}.txt" 
    write_steps_to_file(output_filename, steps)
    print(f"Merge sort steps saved to {output_filename}")