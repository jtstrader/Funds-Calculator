# Funds Calculator
Program that tracks previous and potential future transactions, storing all information in a file to keep records on a multitude of users.

# Starting Up
The program takes in an input file to read in data from. A file MUST be declared here. If the file does not exist, the program will request if you wish to make a file with the name provided. For example:

    $ FundsCalculator inputFile.dat
    $ FundsCalculator: File "inputFile.dat" does not exist. Would you like to create it? [Y/N]
    $ Y
    $ FundsCalculator: File created. Please input the name of your account: 
    ...

Naming your account is important but can be changed in the main menu. From there, you can deposit/withdraw from your checking and savings account, list your previous transactions, update them with new information, or delete them. All the data will be saved once the program finishes, meaning that forcing the program to close mid-transaction might cause a loss of data, so be careful!

# Saving Data
The program uses a map in order to read data from the file. Due to the nature of the transactions, it is safer to store the data in the map of id's with the transactions respective byte offsets in the file. This allows the map to easily delete records by removing the data from the map but saves storage in RAM by avoiding keeping hundreds, if not thousands of records in RAM at once. Optimizations to minimize file seeks have been put in place to try and avoid continuously seeking back and forth, as well as optimizations to minimize file size. All deleted records' offsets are recorded in a vector that is used to create new records. When a record is deleted, the program replaces the transaction record with the enumerable type "T_BREAK", letting the computer know where to store the next free record. 
