// Ats.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "atsplugin.h"
#include "Atssn.h"
#include "Atsp.h"
#include "Serial.h"
#include "Spp.h"
#include "Ats.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_atssn.Time = &g_time;
			g_atssn.Notch = &g_brakeNotch;

			g_atsp.TrainSpeed = &g_speed;
			g_atsp.DeltaT = &g_deltaT;
			g_atsp.BrakeNotch = &g_brakeNotch;
			g_atsp.Reverser = &g_reverser;

			g_spp.TrainSpeed = &g_speed;
			g_spp.DeltaT = &g_deltaT;
			g_spp.BrakeNotch = &g_brakeNotch;

			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



ATS_API void WINAPI Load()
{
	/*
	g_atssn.CheckPanel();
	// ����M�o�b�t�@������
	char SndBuf[32];
	char RcvBuf[32];
	memset(SndBuf, 0, sizeof(SndBuf));
	memset(RcvBuf, 0, sizeof(RcvBuf));

	// COM�|�[�g�̃I�[�v��
	HANDLE hRs232c = CreateFile(
		"\\\\.\\COM13", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hRs232c == INVALID_HANDLE_VALUE) {
	//	printf("RS232C CreateFile Error\n");
	//	return -1;
	}

	// COM�|�[�g�̃Z�b�g�A�b�v
	if (SetupComm(hRs232c, sizeof(RcvBuf), sizeof(SndBuf)) == FALSE){
	//	printf("RS232C SetupComm Error\n");
	//	return -1;
	}

	// COM�|�[�g�̐ݒ�擾
	DCB dcb;
	memset(&dcb, NULL, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hRs232c, &dcb);

	// COM�|�[�g�̐ݒ�ύX
	dcb.BaudRate = 9600;
	dcb.Parity = 0;    // 0,1,2,3,4 = no,odd,even,mark,space 
	dcb.StopBits = 0;    // 0,1,2     = 1bits,1.5bits,2bits
	dcb.ByteSize = 8;    // 4,5,6,7,8 = 4bits,5bits,6bits,7bits,8bits
	if (SetCommState(hRs232c, &dcb) == FALSE){
	//	printf("RS232C SetCommState Error\n");
	//	return -1;
	}

	// ���M�d���쐬
	//sprintf(SndBuf, "abc������");
	
	// COM�|�[�g�Ƀf�[�^���M
	DWORD dwSize;
	OVERLAPPED ovWrite;
	memset(&ovWrite, 0, sizeof(ovWrite));
	//if (WriteFile(hRs232c, SndBuf, strlen(SndBuf), &dwSize, &ovWrite) == FALSE){
	if (WriteFile(hRs232c, "a", strlen("a"), &dwSize, &ovWrite) == FALSE){
	//	printf("RS232C WriteFile Error\n");
	//	return -1;
	}
	else{
		// ���M����
	//	printf("RS232C WriteFile Success\n");
	}
	// COM�|�[�g�̃N���[�Y
	CloseHandle(hRs232c);
	g_atsp.CheckSound();
	*/
}


ATS_API void WINAPI Dispose(){

}
ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec)
{
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
	g_atssn.EmgNotch = g_emgBrake;
	g_atssn.CancelNotch = vehicleSpec.AtsNotch;
	g_atsp.EmergencyNotch = g_emgBrake;
	g_atsp.ServiceNotch = vehicleSpec.AtsNotch;
	g_spp.ServiceNotch = vehicleSpec.AtsNotch;
}

ATS_API void WINAPI Initialize(int brake)
{
	g_atssn.InitSn();
	g_atsp.InitP();
	g_spp.InitSpp();
	g_serial.Init();
	g_speed = 0;
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int *panel, int *sound)
{
	
	g_deltaT = vehicleState.Time - g_time;
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;

	g_atssn.RunSn(); // ATS-SN
	g_atsp.RunP(); // ATS-P
    g_spp.RunSpp(); // ��ԉw�ʉߖh�~���u

	// �n���h���o��
	if(g_atssn.AtsBrake || g_atsp.EmergencyBrake)
	{
		g_output.Brake = g_emgBrake;
	}
	else if(g_atsp.ServiceBrake && g_brakeNotch != g_emgBrake)
	{
		g_output.Brake = g_svcBrake;
	}
	else
	{
		g_output.Brake = g_brakeNotch;
	}
	if(g_pilotlamp)
	{
		g_output.Reverser = g_reverser;
	}
	else
	{
		g_output.Reverser = 0;
	}
	g_output.Power = g_powerNotch;
	g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	// �p�l���o��
	panel[0] = g_atssn.WhiteLamp;
	panel[1] = g_atssn.RedLamp;
	panel[2] = 1; // P�d��
	panel[3] = g_atsp.ApproachLamp;
	panel[4] = 0; // �u���[�L�J��
	panel[5] = g_atsp.BrakeApplyLamp;
	panel[6] = g_atsp.AtspLamp;
	panel[7] = 0; // �̏�

	// �T�E���h�o��
	sound[0] = g_atssn.Bell;
	sound[1] = g_atssn.Chime;
	sound[2] = g_atsp.Bell;
	sound[6] = g_spp.HaltChime;
	sound[7] = g_spp.PassAlarm;

    return g_output;
}

ATS_API void WINAPI SetPower(int notch)
{
	g_powerNotch = notch;
}

ATS_API void WINAPI SetBrake(int notch)
{
	g_brakeNotch = notch;
	g_atssn.CheckAts();
	g_spp.NotchChanged();
}

ATS_API void WINAPI SetReverser(int pos)
{
	g_reverser = pos;
}

ATS_API void WINAPI KeyDown(int atsKeyCode)
{
	switch(atsKeyCode)
	{
	case ATS_KEY_S: // ATS �m�F
		g_atssn.DownButtom();
		break;
	case ATS_KEY_A1: // �x�񎝑�
		g_atssn.CancelChime();
		break;
	case ATS_KEY_B1: // ���A
		g_atssn.Reset();
		g_atsp.Reset();
		break;
	case ATS_KEY_B2: //end
		//COM�|�[�g�I�[�v��
		if (-1 == g_serial.OpenPort()){
			g_atsp.CheckSound(); //�_�����������
		}
		break;
	case ATS_KEY_C1: //pageup
		//send
		g_serial.sendMessage("a", 1);
		break;
	}
}

ATS_API void WINAPI KeyUp(int hornType)
{
	if(hornType == ATS_KEY_S){g_atssn.UpButton();}
}

ATS_API void WINAPI HornBlow(int atsHornBlowIndex)
{
}

ATS_API void WINAPI DoorOpen()
{
	g_pilotlamp = false;
	g_spp.StopChime(); // ��ʂ̃`���C�����~�߂�
}

ATS_API void WINAPI DoorClose()
{
	g_pilotlamp = true;
}

ATS_API void WINAPI SetSignal(int signal)
{
}

ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beaconData){

	switch(beaconData.Type)
	{
	case ATS_BEACON_S: // S�����O
		g_atsp.Exit();
		if(g_speed != 0){g_atssn.PassedLong(beaconData.Signal);} // �w�W�����v�����O����
		break;
	case ATS_BEACON_SN: // SN����
		g_atsp.Exit();
		g_atssn.PassedShort(beaconData.Signal);
		break;
	case ATS_BEACON_P: // ��~�M��
		g_atssn.TurnOff();
		g_atsp.PassedSig(beaconData.Distance);
		break;
	case ATS_BEACON_EMG: // ����(���)
		g_atssn.TurnOff();
		g_atsp.PassedStopEmg(beaconData.Distance);
		break;
	case ATS_BEACON_SVC: // ����(��p)
		g_atssn.TurnOff();
		g_atsp.PassedStopSvc(beaconData.Distance);
		break;
	case ATS_BEACON_SPDLIM: // ����푬�x����
		g_atssn.TurnOff();
		g_atsp.PassedSpeedLim(beaconData.Optional % 1000, beaconData.Optional / 1000);
		break;
	case ATS_BEACON_SPDMAX: // �ō����x
		g_atssn.TurnOff();
		g_atsp.PassedSpeedMax(beaconData.Optional);
		break;
	case ATS_BEACON_SPP: // ��ԉw�ʉߖh�~���u
        g_spp.Receive(beaconData.Optional);
	}

}
