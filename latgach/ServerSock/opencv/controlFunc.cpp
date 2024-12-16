#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio> // for remove()
#include <sys/stat.h> // for stat()
using namespace cv;
using namespace std;

void openWebcam() {
    Mat myImage;//Declaring a matrix to load the frames//
    namedWindow("Video Player");//Declaring the video to show the video//
    VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
    if (!cap.isOpened()) { //This section prompt an error message if no video stream is found//
        cout << "No video stream detected" << endl;
        system("pause");
        return;
    }
    while (true) { //Taking an everlasting loop to show the video//
        cap >> myImage;
        if (myImage.empty()) { //Breaking the loop if no video frame is detected//
            break;
        }
        imshow("Video Player", myImage);//Showing the video//
        char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27) { //If 'Esc' is entered break the loop//
            break;
        }
    }
    cap.release();//Releasing the buffer memory//
    return;
}

void PrintServices() {
    SC_HANDLE hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        cerr << "OpenSCManager failed with error: " << GetLastError() << endl;
        return;
    }
    DWORD dwBytesNeeded = 0;
    DWORD dwServicesReturned = 0;
    DWORD dwResumeHandle = 0;
    DWORD dwServiceType = SERVICE_WIN32;
    DWORD dwServiceState = SERVICE_STATE_ALL;

    ENUM_SERVICE_STATUS_PROCESS* pServices = nullptr;

    if (!EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        dwServiceType,
        dwServiceState,
        nullptr,
        0,
        &dwBytesNeeded,
        &dwServicesReturned,
        &dwResumeHandle,
        nullptr
    ) && GetLastError() != ERROR_MORE_DATA) {
        cerr << "EnumServicesStatusEx failed with error: " << GetLastError() << endl;
        CloseServiceHandle(hSCManager);
        return;
    }

    pServices = reinterpret_cast<ENUM_SERVICE_STATUS_PROCESS*>(new BYTE[dwBytesNeeded]);

    // Call EnumServicesStatusEx again with allocated buffer
    if (!EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        dwServiceType,
        dwServiceState,
        reinterpret_cast<LPBYTE>(pServices),
        dwBytesNeeded,
        &dwBytesNeeded,
        &dwServicesReturned,
        &dwResumeHandle,
        nullptr
    )) {
        cerr << "EnumServicesStatusEx failed with error: " << GetLastError() << endl;
        delete[] pServices;
        CloseServiceHandle(hSCManager);
        return;
    }

    // Print the services
    wofstream fp("list_of_services.txt");
    if (!fp.is_open()) {
        cerr << "Failed to open file for writing." << endl;
        CloseServiceHandle(hSCManager);
        return;
    }
    for (DWORD i = 0; i < dwServicesReturned; ++i) {
        fp << L"Service Name: " << pServices[i].lpServiceName << endl;
        fp << L"Display Name: " << pServices[i].lpDisplayName << endl;
        fp << L"Current State: ";
        switch (pServices[i].ServiceStatusProcess.dwCurrentState) {
        case SERVICE_STOPPED:
            fp << L"Stopped";
            break;
        case SERVICE_START_PENDING:
            fp << L"Start Pending";
            break;
        case SERVICE_STOP_PENDING:
            fp << L"Stop Pending";
            break;
        case SERVICE_RUNNING:
            fp << L"Running";
            break;
        case SERVICE_CONTINUE_PENDING:
            fp << L"Continue Pending";
            break;
        case SERVICE_PAUSE_PENDING:
            fp << L"Pause Pending";
            break;
        case SERVICE_PAUSED:
            fp << L"Paused";
            break;
        default:
            fp << L"Unknown";
        }
        fp << endl << endl;
    }
    fp.close();
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

void deleteFile(const std::string& filePath) {
    struct stat buffer;

    // Check if the file exists
    if (stat(filePath.c_str(), &buffer) == 0) {
        // File exists, attempt to delete it
        if (remove(filePath.c_str()) == 0) {
            cout << "File deleted successfully." << endl;
        }
        else {
            cout << "Cannot delete the file." << endl;
        }
    }
    else {
        cout << "File doesn't exist." << endl;
    }
}

int main() {
    /*wstring svc = L"Spooler";
    stopService(svc);*/
    string path = "C:/Users/ASUS/Desktop/test.txt";
    deleteFile(path);
    return 0;
}

