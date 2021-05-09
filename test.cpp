#include<string>
#include<iostream>

using namespace std;



int main() {
    
    cout<<"Input number: ";
    string input; getline(cin, input);
    try {
        float f = stof(input);
        cout<<"F: "<<f<<endl;

        // proper obtainment of f
        size_t found = input.find('.');
        cout<<"found?: "<<found<<endl;
        cout<<"length: "<<input.length()<<endl;

        // in order for this to be a valid number
        // we have to subtract the tow
        size_t diff = input.length()-found;
        
        // DIFF MUST BE <= 3 TO BE A VALID NUMBER
    }
    catch(invalid_argument e) {
        cerr<<"Please input a valid number."<<endl;
    }

    return 0;
}