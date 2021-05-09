#include "data.h"

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
    }
    else {
        dataFile.read((char*)&uInfo, sizeof(UserInfo)); // read in header information
        
        // storage type: CHECKING, then SAVINGS
        for(int i=0; i<uInfo.NUM_CHECKING_TRANS; i++) {
            Transaction t; dataFile.read((char*)&t, sizeof(Transaction));
            checkingRecords[t.id] = t; // save to map of records
        }
        for(int i=0; i<uInfo.NUM_SAVINGS_TRANS; i++) {
            Transaction t; dataFile.read((char*)&t, sizeof(Transaction));
            savingsRecords[t.id] = t; // save to map of records
        }
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
    long backId = 0;
    if(a_type == CHECKING && checkingRecords.size()>0) {
        map<long, Transaction>::iterator it = checkingRecords.end();
        it--; backId = it->first;
    }
    if(a_type == SAVINGS && savingsRecords.size()>0) {
        map<long, Transaction>::iterator it = savingsRecords.end();
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
    switch(a_type) {
        case CHECKING:
            uInfo.CHECKING_BALANCE+=change;
            uInfo.NUM_CHECKING_TRANS++;
            break;

        case SAVINGS:
            uInfo.SAVINGS_BALANCE+=change;
            uInfo.NUM_SAVINGS_TRANS++;
            break;
    }
    
    
    
    
    
    dataFile.seekg(0, ios::end); // go to end of file, add new record


    
}

/*ostream& operator << (ostream& stream, UserInfo& uinf) {
    return (
        stream<<"|=== "<<uinf.USER_NAME<<endl
              <<"| Checking: $"<<fixed<<setprecision(2)<<uinf.CHECKING_BALANCE<<endl
              <<"|  Savings: $"<<fixed<<setprecision(2)<<uinf.SAVINGS_BALANCE<<endl
              <<"|    Total: $"<<fixed<<setprecision(2)<<uinf.TOTAL_BALANCE<<endl
              <<"|=== "<<endl
              
    );
}*/

void Data::Write() {
    cout<<"|=== "<<uInfo.USER_NAME<<endl
        <<"| Checking: $"<<fixed<<setprecision(2)<<uInfo.CHECKING_BALANCE<<endl
        <<"|  Savings: $"<<fixed<<setprecision(2)<<uInfo.SAVINGS_BALANCE<<endl
        <<"|    Total: $"<<fixed<<setprecision(2)<<uInfo.TOTAL_BALANCE<<endl
        <<"|=== "<<endl;
}

////////////////////////////////////////


////////////////////////////////////////
// PRIVATE FUNCTIONS                  //
////////////////////////////////////////

bool Data::checkDecimal(string input) {

    // Under the assumption that client file takes care of
    // any errors involving invalid input. string input is
    // for the sake of avoiding trailing zeroes obtained when
    // using to_string()

    size_t found = input.find('.');
    
    if(found!=string::npos && (input.length()-found>3))
        return false; // input.length()-found > 3 implies more than 2 decimals
    return true;
}