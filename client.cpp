#include<iostream>
#include "data.h"

using namespace std;

char menu(Data& userData);
bool checkArgs(char* dataFile);
void getName(string& NEW_NAME);
void printHelp();

int main(int argc, char** argv) {
    // no flags for this program
    string USER_NAME;
    if(argc!=2) {
        cout<<"FundsCalculator: Invalid syntax. Use FundsCalculator --help for more information."<<endl;
        return 1;
    }
    else if(strcmp(argv[1], "--help") == 0) {
        // help option
        printHelp();
        return 0;
    }
    else if(!checkArgs(argv[1])) {
        cout<<"FundsCalculator: File "<<argv[1]<<" not found."<<endl;
        cout<<"Would you like to create a new file with this name? [Y/N]: ";
        string input; char letter;
        getline(cin, input); letter = toupper(input[0]);
        if(letter == 'Y') {
            fstream newFile(argv[1], ios::out | ios::binary); // create new file
            newFile.close();
        }
        getName(USER_NAME);
    }

    // passed all error checks, open file and begin transaction tracking
    Data* userData = new Data(argv[1], USER_NAME);
    char option;
    do {
        option = menu(*userData);

    } while(option!='e');

    
    return 0;
}

char menu(Data& userData) {
    cout<<"---CURRENT ACCOUNT INFO---"<<endl;
    userData.Write();
    cout<<"--------------------------"<<endl<<endl;

    cout<<"|======= OPTIONS ========|"<<endl;
    cout<<"| a. Deposit Funds       |"<<endl;
    cout<<"| b. Withdraw Funds      |"<<endl;
    cout<<"| c. Remove Transaction  |"<<endl; 
    cout<<"| d. Update Transaction  |"<<endl;
    cout<<"| e. EXIT                |"<<endl;
    cout<<"|========================|"<<endl<<endl;
    cout<<"| Please select an option: ";
    
    string input; getline(cin, input);
    char letter = tolower(input[0]);
    return letter;
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
            string check; getline(cin, check);
            if(toupper(check[0])=='Y')
                ex = true; // break out of loop
        }
    } while(!ex);
}

void printHelp() {
    cout<<"usage: FundsCalculator [DATA_FILE]"<<endl;
    return;
}