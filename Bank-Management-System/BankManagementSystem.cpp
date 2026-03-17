#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

const string FILE_NAME = "BankAccounts.txt";

class BankAccount
{
private:
    int accNo;
    string name;
    char type;
    int balance;

public:
    BankAccount() = default;

    BankAccount(int accNo, const string &name, char type, int balance)
        : accNo(accNo), name(name), type(toupper(type)), balance(balance) {}

    int getAccNo() const { return accNo; }
    string getName() const { return name; }
    char getType() const { return type; }
    int getBalance() const { return balance; }

    void setName(const string &newName) { name = newName; }
    void setType(char newType) { type = toupper(newType); }
    void setBalance(int newBalance) { balance = newBalance; }

    void deposit(int amt) { balance += amt; }
    bool withdraw(int amt)
    {
        if (amt <= 0 || amt > balance)
            return false;
        balance -= amt;
        return true;
    }

    string toLine() const
    {
        return to_string(accNo) + "|" + name + "|" + type + "|" + to_string(balance);
    }

    static BankAccount fromLine(const string &line)
    {
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, '|'))
        {
            tokens.push_back(token);
        }
        if (tokens.size() == 4)
        {
            return BankAccount(stoi(tokens[0]), tokens[1], tokens[2][0], stoi(tokens[3]));
        }
        return BankAccount();
    }

    void display() const
    {
        cout << "\nAccount No: " << accNo
             << "\nName: " << name
             << "\nType: " << type
             << "\nBalance: " << balance << endl;
    }

    void updateDetails()
    {
        cin.ignore();
        cout << "\nEnter new name: ";
        getline(cin, name);
        cout << "Enter new type (S/C): ";
        cin >> type;
        type = toupper(type);
        cout << "Enter new balance: ";
        cin >> balance;
    }
};

vector<BankAccount> accounts;

void loadAccounts()
{
    accounts.clear();
    ifstream inFile(FILE_NAME);
    string line;
    while (getline(inFile, line))
    {
        accounts.push_back(BankAccount::fromLine(line));
    }
    inFile.close();
}

void saveAccounts()
{
    ofstream outFile(FILE_NAME);
    for (const auto &acc : accounts)
    {
        outFile << acc.toLine() << "\n";
    }
    outFile.close();
}

bool accountExists(int accNo)
{
    for (const auto &acc : accounts)
    {
        if (acc.getAccNo() == accNo)
            return true;
    }
    return false;
}

bool isValidName(const string &name)
{
    for (char ch : name)
    {
        if (!isalpha(ch) && ch != ' ')
            return false;
    }
    return true;
}

void createAccount()
{
    int accNo, balance;
    string name;
    char type;

    cout << "\nEnter Account No: ";
    while (true)
    {
        cin >> accNo;

        if (cin.fail() || accNo <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive account number: ";
        }
        else if (accountExists(accNo))
        {
            cout << "Account number already exists. Enter a different number: ";
        }
        else
        {
            cin.ignore();
            break;
        }
    }

    cout << "Enter Name: ";
    while (true)
    {
        getline(cin, name);

        if (name.empty() || isdigit(name[0]))
        {
            cout << "Invalid name. Please enter a valid name: ";
        }
        else
        {
            break;
        }
    }

    cout << "Enter Type (S for Saving / C for Current): ";
    while (true)
    {
        cin >> type;
        type = toupper(type);
        if (type != 'S' && type != 'C')
        {
            cout << "Invalid type. Please enter S or C: ";
        }
        else
        {
            break;
        }
    }

    cout << "Enter Initial Balance: ";
    while (true)
    {
        cin >> balance;
        if (cin.fail() || balance < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid balance. Please enter a non-negative amount: ";
        }
        else
        {
            break;
        }
    }

    accounts.push_back(BankAccount(accNo, name, type, balance));
    saveAccounts();

    cout << "\nAccount Created Successfully.\n";
}

void displayAccount(int accNo)
{
    for (const auto &acc : accounts)
    {
        if (acc.getAccNo() == accNo)
        {
            acc.display();
            return;
        }
    }
    cout << "\nAccount not found.\n";
}

void depositWithdraw(int accNo, bool deposit)
{
    for (auto &acc : accounts)
    {
        if (acc.getAccNo() == accNo)
        {
            int amt;
            cout << (deposit ? "\nEnter amount to deposit: " : "\nEnter amount to withdraw: ");
            cin >> amt;
            if (deposit)
            {
                acc.deposit(amt);
            }
            else if (!acc.withdraw(amt))
            {
                cout << "\nInvalid or insufficient amount entered.\n";
                return;
            }

            saveAccounts();
            cout << "\nTransaction Successful.\n";
            return;
        }
    }
    cout << "\nAccount not found.\n";
}

void updateAccount(int accNo)
{
    for (auto &acc : accounts)
    {
        if (acc.getAccNo() == accNo)
        {
            acc.updateDetails();
            saveAccounts();
            cout << "\nAccount Updated Successfully.\n";
            return;
        }
    }
    cout << "\nAccount not found.\n";
}

void deleteAccount(int accNo)
{
    auto it = remove_if(accounts.begin(), accounts.end(), [accNo](const BankAccount &acc)
                        { return acc.getAccNo() == accNo; });
    if (it != accounts.end())
    {
        accounts.erase(it, accounts.end());
        saveAccounts();
        cout << "\nAccount Deleted Successfully.\n";
    }
    else
    {
        cout << "\nAccount not found.\n";
    }
}

void displayAllAccounts()
{
    if (accounts.empty())
    {
        cout << "\nNo accounts available.\n";
        return;
    }

    cout << "\n\nACCOUNT HOLDER LIST:\n";
    cout << left << setw(10) << "AccNo" << setw(20) << "Name" << setw(10) << "Type" << setw(10) << "Balance" << endl;
    cout << "----------------------------------------------------------\n";
    for (const auto &acc : accounts)
    {
        cout << left << setw(10) << acc.getAccNo()
             << setw(20) << acc.getName()
             << setw(10) << acc.getType()
             << setw(10) << acc.getBalance() << endl;
    }
}

void transferAmount()
{
    int fromAcc, toAcc, amount;

    cout << "\nEnter Source Account No: ";
    cin >> fromAcc;
    cout << "Enter Destination Account No: ";
    cin >> toAcc;

    if (fromAcc == toAcc)
    {
        cout << "\nSource and destination accounts must be different.\n";
        return;
    }

    auto fromIt = find_if(accounts.begin(), accounts.end(),
                          [fromAcc](const BankAccount &acc)
                          { return acc.getAccNo() == fromAcc; });
    auto toIt = find_if(accounts.begin(), accounts.end(),
                        [toAcc](const BankAccount &acc)
                        { return acc.getAccNo() == toAcc; });

    if (fromIt == accounts.end() || toIt == accounts.end())
    {
        cout << "\nOne or both account numbers not found.\n";
        return;
    }

    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount entered.\n";
        return;
    }

    if (!fromIt->withdraw(amount))
    {
        cout << "\nInsufficient balance in source account.\n";
        return;
    }

    toIt->deposit(amount);
    saveAccounts();

    cout << "\nAmount transferred successfully from Account " << fromAcc
         << " to Account " << toAcc << ".\n";
}

int main()
{
    char choice;
    int accNo;
    loadAccounts();

    do
    {
        system("CLS");
        cout << "\n====== BANK MANAGEMENT SYSTEM ======\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Display Account\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Update Account\n";
        cout << "7. Delete Account\n";
        cout << "8. Transfer Amount\n";
        cout << "9. Exit\n";
        cout << "Enter your choice (1-9): ";
        cin >> choice;

        switch (choice)
        {
        case '1':
            createAccount();
            break;
        case '2':
            cout << "\nEnter Account No: ";
            cin >> accNo;
            depositWithdraw(accNo, true);
            break;
        case '3':
            cout << "\nEnter Account No: ";
            cin >> accNo;
            depositWithdraw(accNo, false);
            break;
        case '4':
            cout << "\nEnter Account No: ";
            cin >> accNo;
            displayAccount(accNo);
            break;
        case '5':
            displayAllAccounts();
            break;
        case '6':
            cout << "\nEnter Account No: ";
            cin >> accNo;
            updateAccount(accNo);
            break;
        case '7':
            cout << "\nEnter Account No: ";
            cin >> accNo;
            deleteAccount(accNo);
            break;
        case '8':
            transferAmount();
            break;
        case '9':
            cout << "\nExiting program.\n";
            break;
        default:
            cout << "\nInvalid choice. Try again.\n";
        }
        cout << endl;
        system("pause");
    } while (choice != '9');

    return 0;
}
