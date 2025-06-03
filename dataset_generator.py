import random
import string

def generate_random_string(length): 
    # Generate a random string of specified length using letters (both cases)
    letters = string.ascii_letters
    return ''.join(random.choice(letters) for _ in range(length))

def generate_dataset(settings, output_file):
    num_to_generate = int(input("Enter the number of records to generate: "))
    with open(output_file, 'w') as f:
        for _ in range(num_to_generate):
            # Generate random number between 1 and 1 million
            number = random.randint(settings["start_number"], settings["end_number"])
            
            # Randomly choose between 4 or 5 characters
            str_length = random.randint(settings["string_length_min"], settings["string_length_max"])
            random_string = generate_random_string(str_length)
            
            # Write to file in the format: number,string
            f.write(f"{number},{random_string}\n")

def setting_dataset_type():
    start_number = int(input("Enter the start number: "))
    end_number = int(input("Enter the end number: "))
    string_length_min = int(input("Enter the minimum length of the string: "))
    string_length_max = int(input("Enter the maximum length of the string: "))
    return {"start_number":start_number,"end_number":end_number,"string_length_min":string_length_min,"string_length_max":string_length_max}

default = {"start_number":1,"end_number":1000000000,"string_length_min":4,"string_length_max":5}

if __name__ == "__main__":
    generate_type = input("Enter the type of dataset to generate: "
                            "\n1. default (assignment)"
                            "\n2. settings"
                            "\nInput:")
    if generate_type == "1":
        pass
    elif generate_type == "2":
        default = setting_dataset_type()
    generate_dataset(default,"dataset_1000000.csv")

    print("Dataset generated successfully in dataset_1000000.csv")
