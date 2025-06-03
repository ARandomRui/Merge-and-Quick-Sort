#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> read_dataset(int start, int end, const string& filename = "dataset_1000000.csv") {
    vector<string> lines;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return lines;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        count++;
        if (line.empty()) {  // Skip empty lines although it should not happen
            continue;
        }
        if (count >= start) {
            
            lines.push_back(line);
        }
        if (count >= end) {
            break;
        }
    }

    file.close();
    return lines;
}

vector<string> merge_sort(vector<string> dataset, int left, int right) {
    if (left < right){  //left is basically the start, 0 and right is the end, so in 
                        //most of the case right will always be bigger than
                        //left.. unless they're equal due to the mid equation
        mid = (left + right)/2  //so if its X.5 it becomes X 5/2 = 2 
        merge_sort(dataset, left, mid)
        merge_sort(dataset, mid+1, right)
        merge (dataset, left, mid, right)
    }
}

bool compare(string a, string b){


}

vector<string> merge(vector<string> dataset, int left, int mid, int right) {
    vector L(dataset.begin() + 1, dataset.begin() + mid)
    vector R(dataset.begin() + mid + 1, dataset.begin() + right)
    int k = left;
    while (L.size() > 0 && R.size() > 0) {
        if compare(L[0],R[0]){

        }
    }
}

  k = left
  while not L.isEmpty() and not R.isEmpty()
    if L.first() < R.first()
      S[k++] = L.removeFirst()
    else
      S[k++] = R.removeFirst()
  while not L.isEmpty()
    S[k++] = L.removeFirst()
  while not R.isEmpty()
    S[k++] = R.removeFirst()

int main() {

    // Ask user to read from which line till which line
    int start, end;
    cout << "Enter the starting line: ";
    cin >> start;
    cout << "Enter the ending line: ";
    cin >> end;

    // Read the dataset and put it into a vector
    vector<string> dataset = read_dataset(start,end);
    
    // Print the total number of lines read
    cout << "Lines Captured: " << dataset.size() << endl;
    
    // Print starting and ending lines
    cout << "\nDataset starting from line:" << dataset[0] << endl;
    cout << "Dataset ending at line:" << dataset[dataset.size()-1] << endl;

    //Start sorting the dataset
    vector<string> sorted_dataset = merge_sort(dataset, 0, dataset.size()-1);

    return 0;
} 