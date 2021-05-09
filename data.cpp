#include "data.h"

////////////////////////////////////////
// PUBLIC FUNCTIONS                   //
////////////////////////////////////////
Data::Data() {

}

Data::~Data() {

}

void Data::open(char* fileName, string USER_NAME, bool newFile) {
    dataFile.open(fileName, ios::in | ios::out | ios::binary);
    if(newFile) {
        // create and then write new header information
        strcpy(uInfo.USER_NAME, USER_NAME.c_str());
        uInfo.CHECKINGS_BALANCE = 0;
        uInfo.NUM_CHECKINGS_TRANS = 0;
        uInfo.SAVINGS_BALANCE = 0;
        uInfo.NUM_SAVINGS_TRANS = 0;
        uInfo.TOTAL_BALANCE = 0;

        dataFile.write((char*)&uInfo, sizeof(UserInfo)); // write out header information
    }
    else
        dataFile.read((char*)&uInfo, sizeof(UserInfo)); // read in header information
}

void Data::close() {

}

// create new transaction and provide updates to the uInfo object
// add transaction to the binary file.
void Data::createNewTransaction(TransactionType t_type, AccountType a_type, float change) {
    // get last transaction record
    map<long, Transaction>::iterator it = transactionRecords.end();
    it--; long backId = it->second.id;
    Transaction newTransaction = {
        backId+1,
        t_type,
        a_type,
        change
    };
    if(t_type == SUBTRACT) 
        change *= -1;
    switch(a_type) {
        case CHECKINGS:
            uInfo.CHECKINGS_BALANCE+=change;
            uInfo.NUM_CHECKINGS_TRANS++;
            break;

        case SAVINGS:
            uInfo.SAVINGS_BALANCE+=change;
            uInfo.NUM_SAVINGS_TRANS++;
            break;
    }
    
    
    
    
    
    dataFile.seekg(0, ios::end); // go to end of file, add new record


    
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