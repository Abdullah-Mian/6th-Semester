#include <iostream>
#include <fstream>
using namespace std;

// Function to list .txt files in current directory
void listTextFiles()
{
    cout << "\nAvailable .txt files in current directory:\n";
    system("dir /b");
    cout << endl;
}

// Function to display menu
void showMenu()
{
    cout << "\n=== File Encryption Tool ===\n";
    cout << "1. List available files in current directory\n";
    cout << "2. Encrypt/Decrypt a file\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

// Function to encrypt/decrypt a character
char encryptChar(char c, char key)
{
    return c ^ key;
}

// Function to encrypt file
void encryptFile(const char inputFile[])
{
    // Read file content
    ifstream inFile(inputFile);
    if (!inFile)
    {
        cout << "Error: Cannot open input file!\n";
        return;
    }

    // Read entire file content into memory
    string content;
    char c;
    while (inFile.get(c))
    {
        content += c;
    }
    inFile.close();

    // Reopen file in write mode
    ofstream outFile(inputFile);
    if (!outFile)
    {
        cout << "Error: Cannot write to file!\n";
        return;
    }

    // Write encrypted content back to file
    char key = 'K';
    for (char c : content)
    {
        outFile.put(encryptChar(c, key));
    }

    cout << inputFile << " has been encrypted/decrypted in-place!" << endl;
    outFile.close();
}

int main()
{
    int choice;
    char filename[100];

    while (true)
    {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            listTextFiles();
            break;

        case 2:
            listTextFiles();
            cout << "Enter filename to encrypt (including extension): ";
            cin.getline(filename, 100);
            encryptFile(filename);
            break;

        case 3:
            cout << "Goodbye!\n";
            return 0;

        default:
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}