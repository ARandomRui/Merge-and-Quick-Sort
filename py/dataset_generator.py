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
            print(f"DEBUG: File '{filename}' opened successfully for writing.") # Added debug
            for item in data:
                f.write(item + '\n')
        print(f"Dataset saved to {filename}")
    except Exception as e: # Catch any potential errors during file writing
        print(f"ERROR: Failed to write file {filename}. Reason: {e}") # Added error print

if __name__ == "__main__":
    # Example usage for demo
    # The lecturer may specify a different size, e.g., 1000000 
    dataset_size = int(input("Enter dataset size (e.g., 1000000): "))
    output_filename = f"dataset_{dataset_size}.csv"
    generate_dataset(dataset_size, output_filename)