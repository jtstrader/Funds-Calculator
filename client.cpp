#include<iostream>
#include "data.h"

using namespace std;

bool checkArgs(char* dataFile);
void getName(string& NEW_NAME);
void printHelp();

int main(int argc, char** argv) {
    // no flags for this program
    string USER_NAME;
    if(argc!=2) {
        cout<<"bank: Invalid syntax. Use bank --help for more information."<<endl;
        return 1;
    }
    else if(strcmp(argv[1], "--help") == 0) {
        // help option
        printHelp();
        return 0;
    }
    else if(!checkArgs(argv[1])) {
        cout<<"bank: File "<<argv[1]<<" not found."<<endl;
        cout<<"Would you like to create a new file with this name? [Y/N]: ";
        string input; char letter;
        cin>>input; letter = toupper(input[0]);
        if(letter == 'Y') {
            fstream newFile(argv[1], ios::out | ios::binary); // create new file
            newFile.close();
        }
        getName(USER_NAME);
    }

    // passed all error checks, open file and begin transaction tracking
    
    return 0;
}

bool checkArgs(char* dataFile) {
    // confirm that the file can be opened
    fstream test_open(dataFile, ios::in | ios::binary); bool open = false;
    if(test_open.is_open())
        open = true;
    test_open.close();

    return open;
}

void getName(string& NEW_NAME) {
    bool ex = false;
    do {
        cout<<"Please input the name of your account: ";
        getline(cin, NEW_NAME);
        if(NEW_NAME.length()>80) {
            // String = char[80]
            cout<<"Please input a name under 80 characters."<<endl;
        }
        else if(NEW_NAME.length()==0) {
            cout<<"Please input a valid name."<<endl;
        }
        else {
            // valid name obtained, confirm with user.
            cout<<"Your name is: "<<NEW_NAME<<endl;
            cout<<"Keep this name? [Y/N]: ";
            string check; cin>>check;
            if(toupper(check[0])=='Y')
                ex = true; // break out of loop
        }
    } while(!ex);
}

void printHelp() {
    cout<<"usage: bank [DATA_FILE]"<<endl;
    return;
}