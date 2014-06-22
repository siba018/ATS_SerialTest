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
// ���������� �I�[�v���ł�����B
int Serial::Init()
{
	portName = "\\\\.\\COM13";
	myHComPort = CreateFile(
		(LPCTSTR)portName,
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (myHComPort == INVALID_HANDLE_VALUE) {
		return -1;
	}

	// �|�[�g�̃{�[���[�g�A�p���e�B����ݒ� 
	DCB config;
	config.BaudRate = 9600;
	// Parity�AStopBits�ADataBits�����l�ɐݒ� 
	SetCommState(myHComPort,&config);

//	m_approachLampOld = 0;
//	init();
}
void Serial::sendMessage(LPVOID buffer, int toWriteBytes){
	DWORD writeBytes;
	DWORD index = 0;
	// �w�肳�ꂽ�f�[�^��S�ď������ވׂɃ��[�v���� 
	while (toWriteBytes > 0){
		WriteFile(myHComPort,
			((BYTE*)buffer) + index, toWriteBytes, &writeBytes, NULL);
		index += writeBytes;
		toWriteBytes -= writeBytes;
	}
}
