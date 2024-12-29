#include<iostream>
#include<limits>
#include<string>
using namespace std;
class ErrorHandling{
    public:
    ErrorHandling(){}
    ~ErrorHandling(){}
    int getValidint(int start_limit, int end_limit) {
    int choice;
    string input;

    while (true) {
        cout << "Enter your choice: ";
        getline(cin, input); // Read the entire line of input

        try {
            size_t pos;
            choice = stoi(input, &pos); // Convert string to integer

            // Check if there are extra characters after the number
            if (pos != input.size()) {
                throw invalid_argument("Invalid input!. Please enter a valid input! ");
            }

            // Check if the number is within the valid range
            if (choice >= start_limit && choice <= end_limit) {
                return choice; // Valid input
            } else {
                if(start_limit == end_limit){
                    cout<<"Please enter only valid response\n";
                }
                else{
                    cout << "Invalid input. Please enter a number between " << start_limit << " and " << end_limit << ".\n";
                }
                
            }
        } catch (const invalid_argument&) {
            cout << "Invalid input. Please enter a valid integer in betwenn "<<start_limit<< " and " <<end_limit<<"\n";
        } catch (const out_of_range&) {
            cout << "Input is out of range. Please enter a valid integer.\n";
        }
    }

}

};