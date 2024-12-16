#pragma once
#ifndef _func_H_
#define _func_H_
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
#pragma comment (lib,"Gdiplus.lib")
#define PORT 9909
#define BUFFER_SIZE 1024
#pragma comment(lib, "Ws2_32.lib")
void ShutDown();
void ListApp();
void InitGDIPlus();
bool SaveBitmapAsPNG(HBITMAP hBitmap, const WCHAR* filePath);
HBITMAP CaptureScreen();
void Screenshot();
void ListService();
void StartApp(const string& app);
void Sleep();
vector<string> tokenize(string s, string del = " ");
void MoveFilez(char sBuff[], int length);
void CopyFileZ(char sBuff[], int length);
void StopApp(const string& app, const string& file);
void deleteFile(const std::string& filePath);
#endif
