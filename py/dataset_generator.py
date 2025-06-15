import random
import time
import os

def generate_dataset(size, filename):
    """
    Generates a dataset with 'size' unique random (integer, string) pairs.
    Integers are 32-bit, positive, and up to 1 billion.
    Strings are random lowercase letters.
    """
    print(f"Generating dataset of size {size}...")

    # Record the start time for dataset generation
    start_time = time.perf_counter()

    unique_integers = set()
    while len(unique_integers) < size:
        # Generate 32-bit positive integer up to 1 billion
        unique_integers.add(random.randint(1, 1_000_000_000))

    data = []
    for num in unique_integers:
        # Generate a random string (e.g., 5-10 lowercase letters)
        rand_string = ''.join(random.choice('abcdefghijklmnopqrstuvwxyz') for _ in range(random.randint(5, 10)))
        data.append(f"{num},{rand_string}")

    try:
        with open(filename, 'w') as f:
            print(f"DEBUG: File '{filename}' opened successfully for writing.")
            for item in data:
                f.write(item + '\n')
        print(f"Dataset saved to {filename}")
    except Exception as e:
        print(f"ERROR: Failed to write file {filename}. Reason: {e}")

    # Record the end time for dataset generation
    end_time = time.perf_counter()
    
    # Calculate and print the duration
    duration = end_time - start_time
    print(f"Dataset generation time: {duration:.4f} seconds")

if __name__ == "__main__":
    dataset_size = int(input("Enter dataset size (e.g., 1000000): "))
    output_filename = f"dataset_{dataset_size}.csv"
    generate_dataset(dataset_size, output_filename)
