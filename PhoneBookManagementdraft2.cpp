#include <iostream>
#include <fstream>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <set>
#include <iterator>

using namespace std;

void menu();
void AddNum();
void Show_Records();
void SearchRecord();
void Record_Deletion();
void Update_Record();
void clearScreen();

class PhoneBook
{

private:
    int srno;
    char name[28];
    char mobile_number[17];
    char email[40];
    char group[30];

public:
    int getSrNo() const
    {
        return srno;
    }

    const char *getName() const
    {
        return name;
    }

    void storeData()
    {
        cin.ignore(); // This is to ignore the newline character left in the buffer
        cout << "Enter Record Name: ";
        cin.getline(name, 28);
        /*The getline function in C++ is used to read a line of text from an input stream. It is a part of string header The getline()
        function extracts characters from the input stream and appends it to the string object until the delimiting character
        is encountered It is often used to read a complete line of input, including spaces, until a newline character is encountered  first
        parameter is where to store and other is how many character to be read*/
        cout << "Enter Mobile Number: ";
        cin.getline(mobile_number, 17);
        cout << "Enter E-mail I.D: ";
        cin.getline(email, 40);
        cout << "Enter Record Group: ";
        cin.getline(group, 30);
        cout << endl;
    }

    void showData() const
    {
        /*In C++, the const qualifier at the end of a member function declaration indicates that the function does not
        modify any member variables of the class. This is particularly useful for functions that are intended to provide
        read-only access to the class's data, as it allows the function to be called on both const and non-const instances of the class */

        cout << "\n.......................PhoneBook Records..............................\n";
        cout << "Sr.No.: " << srno << endl;
        cout << "Name: " << name << endl;
        cout << "Mobile No.: " << mobile_number << endl;
        cout << "Email: " << email << endl;
        cout << "Group: " << group << endl;
    }

    void setSrNo(int newSrNo)
    {
        srno = newSrNo;
    }

    // operator overloading learnt on 29 May
    bool operator<(const PhoneBook &other) const
    {

        /*C strcmp() is a built-in library function that is used for string comparison. This function takes two strings
         (array of characters) as arguments, compares these two strings lexicographically,
         and then returns 0,1, or -1 as the result. It is defined inside <string.h> header file with its prototype as follows:*/
        return strcmp(name, other.name) < 0;
    }
};

void readAllRecords(set<PhoneBook> &records)
{
    ifstream fin("PhoneBook.dat", ios::in | ios::binary);
    PhoneBook temp;
    while (fin.read((char *)&temp, sizeof(temp)))
    {
        records.insert(temp);
    }
    fin.close();
}

void writeAllRecords(const set<PhoneBook> &records)
{
    ofstream fout("PhoneBook.dat", ios::out | ios::binary | ios::trunc);
    for (const auto &record : records)
    {
        fout.write((char *)&record, sizeof(record));
    }
    fout.close();
}

void AddNum()
{
    set<PhoneBook> records;
    readAllRecords(records);

    PhoneBook newRecord;
    newRecord.storeData();

    // Insert the new record into the set
    records.insert(newRecord);

    // Reassign serial numbers
    int srNo = 1;
    for (auto &record : records)
    {
        const_cast<PhoneBook &>(record).setSrNo(srNo++);
    }

    writeAllRecords(records);

    cout << "\n....Record Saved to File....\n";
}

void Show_Records()
{
    set<PhoneBook> records;
    readAllRecords(records);
    for (const auto &record : records)
    {
        record.showData();
    }
    cout << "\nReading of Data File completed....\n";
}

void SearchByName(const set<PhoneBook> &records, const string &name)
{
    bool found = false;
    for (const auto &record : records)
    {
        if (strcmp(record.getName(), name.c_str()) == 0)
        {
            record.showData();
            found = true;
            return;
        }
    }
    if (!found)
    {
        cout << "\nThe Record with Name " << name << " is not in the file...\n";
    }
}

void SearchBySerialNumber(const set<PhoneBook> &records, int srno)
{
    bool found = false;
    for (const auto &record : records)
    {
        if (record.getSrNo() == srno)
        {
            record.showData();
            found = true;
            return;
        }
    }
    if (!found)
    {
        cout << "\nThe Record of Serial Number " << srno << " is not in the file...\n";
    }
}

void SearchRecord()
{
    set<PhoneBook> records;
    readAllRecords(records);

    cout << "Start typing to search by name (press ESC to exit):" << endl;
    string input;
    char ch;

    while (true)
    {
        ch = _getch(); // Capture a single character

        if (ch == 27) // ESC key to exit
        {
            break;
        }
        else if (ch == '\b') // Backspace key
        {
            if (!input.empty())
            {
                input.pop_back();
            }
        }
        else
        {
            input.push_back(ch);
        }

        clearScreen(); // Clear the console screen
        cout << "Search: " << input << endl;

        if (input.empty())
        {
            cout << "No search query entered." << endl;
        }
        else
        {
            bool found = false;
            for (const auto &record : records)
            {
                if (strstr(record.getName(), input.c_str()) != nullptr)
                {
                    record.showData();
                    found = true;
                }
            }
            if (!found)
            {
                cout << "No matching records found." << endl;
            }
        }
    }

    cout << "\nReading of Data File Completed.....\n";
}

void Record_Deletion()
{
    set<PhoneBook> records;
    readAllRecords(records);

    int n, flag = 0;
    cout << "Enter Serial Number of Record To Delete: ";
    cin >> n;

    for (auto it = records.begin(); it != records.end();)
    {
        if (it->getSrNo() == n)
        {
            cout << "\nThe Following record is deleted....\n";
            it->showData();
            it = records.erase(it);
            flag++;
        }
        else
        {
            ++it;
        }
    }

    if (flag == 0)
    {
        cout << "\nThe Record of Serial Number " << n << " is not in the file...\n";
    }
    else
    {
        // Reassign serial numbers
        int srNo = 1;
        for (auto &record : records)
        {
            const_cast<PhoneBook &>(record).setSrNo(srNo++);
        }

        writeAllRecords(records);
        cout << "\n....Record Deleted....\n";
    }
    cout << "\nReading of Data File Completed.....\n";
}

void Update_Record()
{
    set<PhoneBook> records;
    readAllRecords(records);

    int n, flag = 0;
    cout << "Enter Serial Number of Record To Modify: ";
    cin >> n;

    for (auto &record : records)
    {
        if (record.getSrNo() == n)
        {
            cout << "\nThe Following record will be updated....\n";
            record.showData();
            flag++;
            cout << "\nRe-Enter the New Details....\n";
            const_cast<PhoneBook &>(record).storeData();
            break;
        }
    }

    if (flag == 0)
    {
        cout << "\nThe Record of Serial Number " << n << " is not in the file....\n";
    }
    else
    {
        writeAllRecords(records);
        cout << "\n...Data Modified Successfully\n";
    }

    cout << "\nReading of Data File Completed.... \n";
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu()
{
    int ch;
    do
    {
        clearScreen();
        cout << "........................................................................................\n";
        cout << " +--------------------------------PhoneBook Management---------------------------------+\n";
        cout << "........................................................................................\n";
        cout << ":::::::::::::::::::::::::::::::::::Program Menu::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "0. Exit\n";
        cout << "1. Save New Phone Record\n";
        cout << "2. Display All Saved Record\n";
        cout << "3. Search Specific Records\n";
        cout << "4. Delete Specific Record\n";
        cout << "5. Modify Existing Record\n";
        cout << "Enter your choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            AddNum();
            break;
        case 2:
            Show_Records();
            break;
        case 3:
            SearchRecord();
            break;
        case 4:
            Record_Deletion();
            break;
        case 5:
            Update_Record();
            break;
        }

        getch();
    } while (ch != 0);
}

int main()
{
    menu();
    return 0;
}
