#include "data.h"

////////////////////////////////////////
// FRIEND FUNCTIONS                   //
////////////////////////////////////////

ostream& operator << (ostream& stream, UserInfo& uinf) {
    // USER INFO OVERLOADING
    return (
        stream<<"|=== "<<uinf.USER_NAME<<endl
              <<"| Checking: $"<<fixed<<setprecision(2)<<uinf.CHECKING_BALANCE<<endl
              <<"|  Savings: $"<<fixed<<setprecision(2)<<uinf.SAVINGS_BALANCE<<endl
              <<"|    Total: $"<<fixed<<setprecision(2)<<uinf.TOTAL_BALANCE<<endl
              <<"|=== "    
    );
}

ostream& operator << (ostream& stream, Transaction& tinf) {
    // TRANSACTION OVERLOADING
    stream<<"|"
          <<(tinf.a_type==CHECKING?" CHECKING":" SAVINGS")<<"|"
          <<(tinf.t_type==ADD?"    Deposit":" Withdrawal") <<"| ";
    
    if(tinf.t_type==SUBTRACT)
        stream<<"-";
    else
        stream<<" ";
    
    stream<<"$"<<fixed<<setprecision(2)<<tinf.change;
    return stream;
}

////////////////////////////////////////
// PUBLIC FUNCTIONS                   //
////////////////////////////////////////
Data::Data(char* fileName, string USER_NAME) {
    dataFileName = fileName; open(dataFileName);
    if(USER_NAME!="") {
        // new user created
        // create and then write new header information
        strcpy(uInfo.USER_NAME, USER_NAME.c_str());
        uInfo.CHECKING_BALANCE = 0;
        uInfo.NUM_CHECKING_TRANS = 0;
        uInfo.SAVINGS_BALANCE = 0;
        uInfo.NUM_SAVINGS_TRANS = 0;
        uInfo.TOTAL_BALANCE = 0;

        dataFile.write((char*)&uInfo, sizeof(UserInfo));
    }
    else {
        dataFile.read((char*)&uInfo, sizeof(UserInfo)); // read in header information
        
        // storage type: CHECKING, then SAVINGS
        Transaction t; const long SIZE = sizeof(Transaction);
        while(dataFile.read((char*)&t, sizeof(Transaction))) {
            if(t.t_type == T_BREAK || t.a_type == A_BREAK) {
                // this is a deleted record.
                // record byte offset in private vector
                avail.push_back(dataFile.tellg());
            }
            else if(t.a_type == CHECKING)
                checkingRecords[t.id] = ((long)dataFile.tellg())-SIZE;
            else
                savingsRecords[t.id] = ((long)dataFile.tellg())-SIZE;
        }
        dataFile.clear(); // clear error flags
    }
}

void Data::open(char* fileName) {
    dataFile.open(fileName, ios::in | ios::out | ios::binary);
}

void Data::close() {
    dataFile.close(); // required to properly save data
}

// create new transaction and provide updates to the uInfo object
// add transaction to the binary file.
void Data::createNewTransaction(TransactionType t_type, AccountType a_type, float change) {
    // get last transaction record
    long backId = -1;
    if(a_type == CHECKING && checkingRecords.size()>0) {
        map<long, long>::iterator it = checkingRecords.end();
        it--; backId = it->first;
    }
    if(a_type == SAVINGS && savingsRecords.size()>0) {
        map<long, long>::iterator it = savingsRecords.end();
        it--; backId = it->first;
    }
    Transaction newTransaction = {
        backId+1,
        t_type,
        a_type,
        change
    };

    if(t_type == SUBTRACT) 
        change *= -1;

    // write to file. first check avail list, if not seek to end
    // perform arithmetic before saving data to maintain changes
    if(avail.size()>0) {
        dataFile.seekp(avail[0]);
        avail.erase(avail.begin()); // remove available slot
    }
    else
        dataFile.seekp(0, ios::end);
    
    switch(a_type) {
        case CHECKING:
            uInfo.CHECKING_BALANCE+=change;
            uInfo.NUM_CHECKING_TRANS++;
            checkingRecords[newTransaction.id] = dataFile.tellp();
            break;

        case SAVINGS:
            uInfo.SAVINGS_BALANCE+=change;
            uInfo.NUM_SAVINGS_TRANS++;
            savingsRecords[newTransaction.id] = dataFile.tellp();
            break;
        // remove compiler warning
        case A_BREAK:
            break;
    }
    dataFile.write((char*)&newTransaction, sizeof(Transaction));
        
    dataFile.seekp(0);
    dataFile.write((char*)&uInfo, sizeof(UserInfo));
}

// create files of their respective transaction lists
void Data::createTransactionListFiles() {
    // 2 main files:
    //   1. dataFileName_CHECKING.txt
    //   2. dataFileName_SAVINGS.txt

    string outputFileName = dataFileName;
    fstream checkingOutFile(outputFileName+"_CHECKING.txt", ios::out | ios::trunc);
    fstream savingsOutFile(outputFileName+"_SAVINGS.txt", ios::out | ios::trunc);

    for(map<long, long>::iterator it = checkingRecords.begin(); it!=checkingRecords.end(); ++it) {
        Transaction t;
        dataFile.seekg(it->second); // go to byte offset
        dataFile.read((char*)&t, sizeof(Transaction));

        // write out to text file
        checkingOutFile<<t<<endl;
    }

    for(map<long, long>::iterator it = savingsRecords.begin(); it!=savingsRecords.end(); ++it) {
        Transaction t;
        dataFile.seekg(it->second); // go to byte offset
        dataFile.read((char*)&t, sizeof(Transaction));

        // write out to text file
        savingsOutFile<<t<<endl;
    }
}

void Data::listCheckingTransactions() {
    for(map<long, long>::iterator it = checkingRecords.begin(); it!=checkingRecords.end(); ++it) {
        Transaction t;
        dataFile.seekg(it->second);
        dataFile.read((char*)&t, sizeof(Transaction));
        cout<<t.id<<". "<<t<<endl;
    }
}

void Data::listSavingsTransactions() {
    for(map<long, long>::iterator it = savingsRecords.begin(); it!=savingsRecords.end(); ++it) {
        Transaction t;
        dataFile.seekg(it->second);
        dataFile.read((char*)&t, sizeof(Transaction));
        cout<<t.id<<". "<<t<<endl;
    }
}

bool Data::deleteTransaction(long id, AccountType a_type) {
    // remove ID from map and rewrite location with blank Transaction where
    // t_type = T_BREAK and a_type = A_BREAK

    map<long, long>::iterator it;
    switch(a_type) {
        case CHECKING:
        {
            it = checkingRecords.find(id);
            if(it==checkingRecords.end())
                return false;
            
            break;
        }
        case SAVINGS:
        {
            it = savingsRecords.find(id);
            if(it==savingsRecords.end())
                return false;
            break;
        }
        case A_BREAK:
            break;
    }
}

void Data::WriteUserInfo() {
    cout<<uInfo<<endl;
}

float Data::getCheckingBalance() {
    return uInfo.CHECKING_BALANCE;
}

float Data::getSavingsBalance() {
    return uInfo.SAVINGS_BALANCE;
}

////////////////////////////////////////


////////////////////////////////////////
// PRIVATE FUNCTIONS                  //
////////////////////////////////////////
