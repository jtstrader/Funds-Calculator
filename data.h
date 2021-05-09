#include<map>
#include<string>
#include<fstream>
using namespace std;

/* Data is written out to a binary file. UserInfo is the header of the file
 * and keeps track of relevant information of the user, such as current balance,
 * number of accounts and the transactions for each. */
typedef char String[80];
struct UserInfo {
    String USER_NAME;
    float CHECKING_BALANCE, SAVINGS_BALANCE, TOTAL_BALANCE;
    int NUM_CHECKING_TRANS, NUM_SAVINGS_TRANS;
};

/* Transaction data is maintained through a list of transaction records. These
 * will be able to be printed to the user through a public method of the data class.
 * Transactions are stored in the binary file and are gathered through a for loop,
 * with the index being handled by NUM_CHECKING_TRANS and NUM_SAVINGS_TRANS. */
enum TransactionType{ADD, SUBTRACT};
enum AccountType{CHECKING, SAVINGS};
struct Transaction {
    long id; // track ID of transaction for changes to the list
    TransactionType t_type;
    AccountType a_type;
    float change; // change in funds (addition/subtraction)
};

class Data {
    public:
        // initializing
        Data(char* fileName, string USER_NAME);
        ~Data();

        // file handling
        void open(char* fileName);
        void close();

        // transaction addition, subtraction, etc.
        void createNewTransaction(TransactionType t_type, AccountType a_type, float change);


        // printer/user interaction methods
        void printUserInfo();
        void printTransactions();

    private:
        // variables
        UserInfo uInfo;
        map<long, Transaction> checkingRecords;
        map<long, Transaction> savingsRecords;
        fstream dataFile;
        char* dataFileName;
        
        // functions
        bool checkDecimal(string input); // check if number of decimals in input is valid
};