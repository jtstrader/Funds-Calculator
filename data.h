#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<iomanip>
#include<limits>
#include<sstream>

using namespace std;

// initial definitions
struct UserInfo;
struct Transaction;
class Data;

/* Data is written out to a binary file. UserInfo is the header of the file
 * and keeps track of relevant information of the user, such as current balance,
 * number of accounts and the transactions for each. */
typedef char String[80];
struct UserInfo {
    String USER_NAME;
    float CHECKING_BALANCE, SAVINGS_BALANCE, TOTAL_BALANCE;
    int NUM_CHECKING_TRANS, NUM_SAVINGS_TRANS;

    friend ostream& operator << (ostream& stream, UserInfo& uinf);
};

/* Transaction data is maintained through a list of transaction records. These
 * will be able to be printed to the user through a public method of the data class.
 * Transactions are stored in the binary file and are gathered through a for loop,
 * with the index being handled by NUM_CHECKING_TRANS and NUM_SAVINGS_TRANS. */
enum TransactionType{ADD, SUBTRACT, T_BREAK};
enum AccountType{CHECKING, SAVINGS, A_BREAK};
struct Transaction {
    long id; // track ID of transaction for changes to the list
    TransactionType t_type;
    AccountType a_type;
    float change; // change in funds (addition/subtraction)
    String reason; // reasoning for transaction or general information

    friend ostream& operator << (ostream& stream, Transaction& tinf);
};

class Data {
    public:
        // initializing
        Data(char* fileName, string USER_NAME);

        // file handling
        void open(char* fileName);
        void close();

        // transactions, listings, deletions
        void createNewTransaction(TransactionType t_type, AccountType a_type, float change, string reason);
        void createTransactionListFiles();
        void listCheckingTransactions();
        void listSavingsTransactions();
        bool deleteTransaction(long id, AccountType a_type);

        // printers and getters
        void WriteUserInfo();
        void WriteTransactionInfo();
        float getCheckingBalance();
        float getSavingsBalance();

    private:
        // variables
        UserInfo uInfo;
        map<long, long> checkingRecords; // maps id to byte offset
        map<long, long> savingsRecords; // maps id to byte offset
        vector<long> avail; // places that new records can be placed
        fstream dataFile;
        char* dataFileName;
        
        // functions
        bool checkDecimal(string input); // check if number of decimals in input is valid
        void updateTotal(); // update total balance
        
};