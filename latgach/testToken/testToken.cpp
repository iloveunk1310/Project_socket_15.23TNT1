#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <gdiplus.h>
#include <sys/stat.h>
#include <cstdio>
#include <tchar.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;
std::string get_current_time_and_date() {
    std::time_t now = std::time(nullptr);
    char buffer[100];
    struct tm timeInfo;
    if (localtime_s(&timeInfo, &now) == 0) {
        if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo)) {
            return std::string(buffer);
        }
    }
    return "";
}

pair<string, string> tokenize(string s, string del)
{
    vector<string> alo;
    int start, end = -1 * del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        alo.push_back(s.substr(start, end - start));
    } while (end != -1);
    return make_pair(alo[0], alo[1]);
}
void ListApp() {

    const char* command = "powershell -Command \"Get-ItemProperty HKLM:\\Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\* | Select-Object DisplayName, DisplayVersion, Publisher, InstallDate | Format-Table –AutoSize > D:\\ListApp.txt\"";
    int result = system(command);
}
void startService(const wstring& serviceName) {
    // Open a handle to the service control manager
    SC_HANDLE hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        cerr << "OpenSCManager failed with error: " << GetLastError() << endl;
        return;
    }

    // Open a handle to the service
    SC_HANDLE hService = OpenService(hSCManager, serviceName.c_str(), SERVICE_START);
    if (!hService) {
        cerr << "OpenService failed with error: " << GetLastError() << endl;
        CloseServiceHandle(hSCManager);
        return;
    }
    // Start the service
    if (!StartService(hService, 0, nullptr)) {
        cerr << "StartService failed with error: " << GetLastError() << endl;
    }
    else {
        wcout << L"Service " << serviceName << L" started successfully." << endl;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
void stopService(const wstring& serviceName) {
    // Open a handle to the service control manager
    SC_HANDLE hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        cerr << "OpenSCManager failed with error: " << GetLastError() << endl;
        return;
    }

    // Open a handle to the service
    SC_HANDLE hService = OpenService(hSCManager, serviceName.c_str(), SERVICE_STOP);
    if (!hService) {
        cerr << "OpenService failed with error: " << GetLastError() << endl;
        CloseServiceHandle(hSCManager);
        return;
    }

    // Stop the service
    SERVICE_STATUS status;
    if (!ControlService(hService, SERVICE_CONTROL_STOP, &status)) {
        cerr << "ControlService failed with error: " << GetLastError() << endl;
    }
    else {
        wcout << L"Service " << serviceName << L" stopped successfully." << endl;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
void add_to_start(const std::string& path, const std::string& s) {
    // Open the file in read mode to load its content
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return;
    }

    // Read the entire content of the file
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string fileContent = buffer.str();

    inputFile.close();

    // Open the file in write mode to overwrite its content
    std::ofstream outputFile(path);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return;
    }

    // Write the new line followed by the original content
    outputFile << s << std::endl;
    outputFile << fileContent;

    outputFile.close();
}
int main() {
    string str = "msiserver";
    std::wstring str2(str.length(), L' ');
    std::copy(str.begin(), str.end(), str2.begin());
    startService(str2);
    return 0;
}