import csv

def read_dataset(filename):
    data = []
    try:
        with open(filename, 'r', newline='', encoding='utf-8') as f:
            reader = csv.reader(f)
            for row in reader:
                if row and len(row) >= 2: # Ensure row has at least two elements
                    try:
                        # Convert the first element to an integer, keep the second as string
                        data.append((int(row[0]), row[1]))
                    except ValueError:
                        print(f"Warning: Skipping malformed row: {row}")
                else:
                    print(f"Warning: Skipping empty or incomplete row: {row}")
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
    return data

def write_steps_to_file(filename, steps_list):
    try:
        with open(filename, 'w', encoding='utf-8') as f:
            for step in steps_list:
                f.write(str(step) + '\n')
    except Exception as e:
        print(f"An error occurred while writing to the file: {e}")

def binary_search_steps(arr, target): #perform the binary search function
    steps = []
    low = 0
    high = len(arr) - 1

    found = False
    while low <= high:
        mid = low + (high - low) // 2 

        # Ensure mid is a valid index
        if mid < 0 or mid >= len(arr):
            steps.append("Error: Mid index out of bounds.")
            break

        current_value_int = arr[mid][0] 
        current_value_str = arr[mid][1] 

        # Record the step using 1-based indexing for the row number
        # (mid + 1) converts the 0-based array index to a 1-based row number.
        steps.append(f"{mid + 1}: {current_value_int}/{current_value_str}")

        if current_value_int == target:
            found = True
            break  # Target found
        elif current_value_int < target:
            low = mid + 1  # Search in the right half
        else: # current_value_int > target
            high = mid - 1 # Search in the left half
    
    if not found:
        steps.append("-1") # If target was not found after the loop

    return steps

if __name__ == "__main__":
    dataset_filename = input("Enter sorted dataset filename (e.g., merge_sort_1000000.csv): ")
    target_str = input("Enter the target integer to search for: ")
    
    try:
        target = int(target_str)
    except ValueError:
        print("Invalid target. Please enter an integer.")
        exit() # Exit if target is not a valid integer

    data = read_dataset(dataset_filename)
    if not data:
        print("No data was loaded. Please check the file path and content.")
        exit()

    print(f"Dataset loaded with {len(data)} elements.")
    print(f"Performing binary search for target: {target}")

    # Get the search steps
    search_steps = binary_search_steps(data, target)

    # Construct the output filename
    output_filename = f"binary_search_step_{target}.txt"
    
    # Write the steps to the output file
    write_steps_to_file(output_filename, search_steps)

    print(f"Binary search steps saved to {output_filename}")
    
    # Check if target was found based on the last step
    if search_steps and search_steps[-1] != "-1":
        print(f"Target {target} found.")
    else:
        print(f"Target {target} not found.")
