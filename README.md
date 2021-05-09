# Funds Calculator
Program that tracks previous and potential future transactions, storing all information in a file to keep records on a multitude of users.

# Starting Up
The program takes in an input file to read in data from. A file MUST be declared here. If the file does not exist, the program will request if you wish to make a file with the name provided. For example:

    $ FundsCalculator inputFile.dat
    $ FundsCalculator: File "inputFile.dat" does not exist. Would you like to create it? [Y/N]
    $ Y
    $ FundsCalculator; File created. Please input the name of your account: 
    ...

Naming your account is important but can be changed in the main menu. From there, you can deposit/withdraw from your checking and savings account, list your previous transactions, update them with new information, or delete them. All the data will be saved once the program finishes, meaning that forcing the program to close mid-transaction might cause a loss of data, so be careful!
