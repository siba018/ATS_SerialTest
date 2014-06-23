#include "stdafx.h"
#include "atsplugin.h"
#include "Serial.h"


Serial::Serial()
{
}

Serial::~Serial()
{
}

LPCSTR portName;
HANDLE myHComPort;
// 初期化する オープンでもある。
void Serial::Init(){
	isConnect = false;
}
int Serial::OpenPort()
{
	if (isConnect == false){
		portName = "\\\\.\\COM13";
		myHComPort = CreateFile(
			(LPCTSTR)portName,
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL);
		if (myHComPort == INVALID_HANDLE_VALUE) {
			return -1;
		}
		else{
			isConnect = true;
		}

		// ポートのボーレート、パリティ等を設定 
		DCB config;
		config.BaudRate = 9600;
		// Parity、StopBits、DataBitsも同様に設定 
		SetCommState(myHComPort, &config);
	}
//	m_approachLampOld = 0;
//	init();
}
void Serial::sendMessage(LPVOID buffer, int toWriteBytes){
	//LPVOIDってポインタらしい（よくしらない）
	DWORD writeBytes;
	DWORD index = 0;
	// 指定されたデータを全て書き込む為にループを廻す 
	while (toWriteBytes > 0){
		WriteFile(myHComPort,
			((BYTE*)buffer) + index, toWriteBytes, &writeBytes, NULL);
		index += writeBytes;
		toWriteBytes -= writeBytes;
	}
}
