#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <filesystem>
#pragma comment(lib, "urlmon.lib")
using namespace std;

string findValue(const string& jsonString, const string& fieldName, bool isItNumber) {

    int startPos = jsonString.find(fieldName);

    if (startPos != string::npos) {

        int colonPos = jsonString.find(":", startPos);
        int valueStartPos = jsonString.find_first_not_of(" \t\r\n", colonPos + 1);
        int commaPos = jsonString.find(",", valueStartPos);
        if (commaPos == string::npos) {
            commaPos = jsonString.find("\n", valueStartPos);
        }
        return isItNumber ? jsonString.substr(valueStartPos, commaPos - valueStartPos) : jsonString.substr(valueStartPos + 1, commaPos - valueStartPos -2 );
    }

    return "";
}


int main() {
    string date, currency;
    cin >> date >> currency;

    string URL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?valcode=" + currency + "&date=" + date + "&json";
    string destFile = "file.txt";

    if (S_OK == URLDownloadToFileA(NULL, URL.c_str(), destFile.c_str(), 0, NULL)) {
        cout << "Saved to " << destFile << "\n";
    }
    else {
        cout << "Failed to download data!\n";
        return 1;
    }

    ifstream file(destFile);
    string jsonString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    string rate = findValue(jsonString, "rate",true);
    string exchangedate = findValue(jsonString, "exchangedate",false);
    string cc = findValue(jsonString, "cc", false);
    cout << "Date: " << exchangedate<<endl << "Currency: " << cc << endl<<"Rate: "<<rate<<" grn"<<endl;

    return 0;
}
