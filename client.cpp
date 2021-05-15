#include<iostream>
#include "data.h"

using namespace std;

string err_str; // error string for getline() on invalid input

char menu(Data& userData);
bool checkArgs(char* dataFile);
void getName(string& NEW_NAME);
bool getValidDeposit(string& input);
bool getValidWithdraw(string& input, Data& userData, AccountType a_type);
AccountType getAccountType(TransactionType t_type);
bool checkDecimal(string input);
void printHelp();

int main(int argc, char** argv) {
    // no flags for this program
    string USER_NAME = "";
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
        switch(option) {
            case 'a':
            {
                // DEPOSIT FUNDS
                string deposit_amount = "";
                AccountType a_type = getAccountType(ADD);

                if(a_type!=A_BREAK) {
                    while(!getValidDeposit(deposit_amount));
                    if(deposit_amount == "-1")
                        break;
                    userData->createNewTransaction(ADD, a_type, stof(deposit_amount));
                }
                break;
            }
            case 'b':
            {
                // DEPOSIT FUNDS
                string withdraw_amount = "";
                AccountType a_type = getAccountType(SUBTRACT);

                if(a_type!=A_BREAK) {
                    while(!getValidWithdraw(withdraw_amount, *userData, a_type));
                    if(withdraw_amount == "-1")
                        break;
                    userData->createNewTransaction(SUBTRACT, a_type, stof(withdraw_amount));
                }
                break;
            }
            case 'c':
            {
                userData->createTransactionListFiles();
                break;
            }
            case 'd':
            {
                break;
            }
            case 'e':
            {
                break;
            }
            case 'f':
            {
                userData->close();
                break;
            }
            default:
            {
                cout<<"Please enter a valid option."<<endl;
                cout<<"Press enter to continue.";
                getline(cin, err_str);
                break;
            }
        }
    } while(option!='f');

    
    return 0;
}

char menu(Data& userData) {
    cout<<"---CURRENT ACCOUNT INFO---"<<endl;
    userData.WriteUserInfo();
    cout<<"--------------------------"<<endl<<endl;

    cout<<"|======= OPTIONS ========|"<<endl;
    cout<<"| a. Deposit Funds       |"<<endl;
    cout<<"| b. Withdraw Funds      |"<<endl;
    cout<<"| c. List Transactions   |"<<endl;
    cout<<"| d. Update Transaction  |"<<endl; 
    cout<<"| e. Remove Transaction  |"<<endl;
    cout<<"| f. EXIT                |"<<endl;
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

bool getValidDeposit(string& input) {
    // special conditions required:
    //   1. input cannot be negative
    //   2. input must have at maximum 2 decimals 

    cout<<"Enter how much you'd like to deposit. Enter -1 to EXIT: ";
    getline(cin, input);

    if(input=="-1")
        return true;

    // confirm is valid float
    try {
        float testIfValid = stof(input);

        // value is a valid float. cannot be negative. confirm decimals
        if(testIfValid<0) {
            cout<<"You cannot deposit a negative amount."<<endl;
            cout<<"Press enter to continue.";
            getline(cin, err_str);
            input = "-1";
            return false;
        }
        else if(testIfValid==0) {
            cout<<"You cannot deposit $0."<<endl;
            cout<<"Press enter to continue.";
            getline(cin, err_str);
            input = "-1";
            return false;
        }
        return checkDecimal(input);
    }
    catch(invalid_argument e) {
        cerr<<"That is not a valid amount."<<endl;
        cout<<"Press enter to continue.";
        getline(cin, err_str);
        input = "-1";
        return false;
    }
}

bool getValidWithdraw(string& input, Data& userData, AccountType a_type) {
 // special conditions required:
    //   1. input cannot be negative
    //   2. input must have at maximum 2 decimals 

    float balance = (a_type==CHECKING
                    ?userData.getCheckingBalance()
                    :userData.getSavingsBalance());
    cout<<"Current Balance: $"<<balance<<endl;
    cout<<"Enter how much you'd like to withdraw. Enter -1 to EXIT: ";
    getline(cin, input);

    if(input=="-1")
        return true;

    // confirm is valid float
    try {
        float testIfValid = stof(input);

        // value is a valid float. cannot be negative. confirm decimals
        if(testIfValid<0) {
            cout<<"You cannot withdraw a negative amount."<<endl;
            cout<<"Press enter to continue.";
            getline(cin, err_str);
            input = "-1";
            return false;
        }
        else if(testIfValid==0) {
            cout<<"You cannot withdraw $0."<<endl;
            cout<<"Press enter to continue.";
            getline(cin, err_str);
            input = "-1";
            return false;
        }
        if(!checkDecimal(input))
            return false;
        
        // confirm this will not exceed the withdrawal amount
        if((balance-testIfValid)<0) {
            cout<<"You cannot withdraw an amount higher than your balance!"<<endl;
            cout<<"Press enter to continue.";
            getline(cin, err_str);
            input = "-1";
            return false;
        }
        return true;
    }
    catch(invalid_argument e) {
        cerr<<"That is not a valid amount."<<endl;
        cout<<"Press enter to continue.";
        getline(cin, err_str);
        input = "-1";
        return false;
    }
}

AccountType getAccountType(TransactionType t_type) {
    string input = "";
    while(input!="1" && input!="2" && input!="3") {
        cout<<"--"<<(t_type==ADD?"Deposit into: ":"Withdraw from: ")<<endl;
        cout<<"--  1. Checking Account"<<endl;
        cout<<"--  2. Savings Account"<<endl;
        cout<<"--  3. EXIT TRANSACTION"<<endl;
        cout<<"-- Enter an option: ";
        getline(cin, input);

        switch(input[0]) {
            case '1': return CHECKING;
            case '2': return SAVINGS;
            case '3': break;
            default:
                cout<<"Please input a valid response."<<endl;
                cout<<"Press enter to continue.";
                getline(cin, err_str);
                input = "";
                break;
        }
    }
    return A_BREAK;
}

bool checkDecimal(string input) {
    // confirm that maximum number of decimals is 2

    size_t found = input.find('.');
    
    if(found!=string::npos && (input.length()-found>3)) {
        return false; // input.length()-found > 3 implies more than 2 decimals
    }
    return true;
}

void printHelp() {
    cout<<"usage: FundsCalculator [DATA_FILE]"<<endl;
    return;
}