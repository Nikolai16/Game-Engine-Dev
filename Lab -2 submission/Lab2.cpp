//Nikolas Niemczak

using namespace std;
#include <iostream>
#include <windows.h>
#include <Winbase.h>
#include <stdio.h>
#include <direct.h>  
#include <stdlib.h>  
#include <ctype.h> 

bool IsOnlyInstance(LPCTSTR gameTitle) {
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);
	if (GetLastError() != ERROR_SUCCESS) {

			cout << "Multiple instances open" << endl;
			return false;
	}
	cout << "Only one instance open" << endl;
	return true;
}

bool CheckStorage(const DWORDLONG diskSpaceNeeded) {

	int const drive = _getdrive();
	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);
	unsigned __int64 const neededClusters = diskSpaceNeeded /
		(diskfree.sectors_per_cluster*diskfree.bytes_per_sector);
	if (diskfree.avail_clusters < neededClusters) {
		
		cout << ("CheckStorage Failure : Not enough physical storage for " + diskSpaceNeeded);
		return false;
	}

	cout << ("CheckStorage Sucess : You have enough physical storage.") << endl;
	return true;
}

void CheckMemory() {
	MEMORYSTATUSEX memoryStatus;
	GlobalMemoryStatusEx(&memoryStatus);

		cout << "You have " << memoryStatus.ullAvailPhys << " avaliable bytes for phyisical memory." << endl;

		cout << "You have " << memoryStatus.ullAvailVirtual << " avaliable bytes for virtual memory." << endl;
}

DWORD ReadCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;

	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) {

		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)
			&dwMHz, &BufSize);
	}
	return dwMHz;
}

void ReadCPUSpecs()
{
	DWORD specsType = REG_SZ;
	HKEY hKey;
	LPCTSTR registryName = "ProcessorNameString";
	TCHAR specsData[50];
	DWORD BufferSize = sizeof(specsData);
	
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) {
		RegQueryValueEx(hKey, registryName, NULL, &specsType, reinterpret_cast<LPBYTE>(&specsData), &BufferSize);
		cout << specsData << endl;
	}
}



void main()
{

	IsOnlyInstance("game");

	//Paramiter is in bytes
	CheckStorage(300000000);


	CheckMemory();


	cout << "Cpu speed is " << ReadCPUSpeed() << " MHz" << endl;
	ReadCPUSpecs();

	system("pause");
}
