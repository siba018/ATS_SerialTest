#include "stdafx.h"
#include "atsplugin.h"
#include "Atssn.h"

CAtssn::CAtssn(void)
{
}

CAtssn::~CAtssn(void)
{
}

void CAtssn::RunSn(void)
{
	// ATS�쓮
	if(m_atsAlarm == 2)
	{
		if(*Time >= m_tmrAtspassAlarm && m_tmrAtspassAlarm){cancelAts();} // �N�����`���C�������ؑ�

		if(*Time >= m_tmrAts && !AtsBrake)
		{
			AtsBrake = true;
			// ���b�Z�[�W�uATS��舵���s�ǂł��v
		}

		// �ԐF���_��
		if(AtsBrake)
		{
			int n = (*Time % 750) / 375;
			RedLamp = n;
			m_blnAtsLamp = n;
		}
	}
}

// ATS�m�F����
void CAtssn::CheckAts(void)
{
	if(*Notch >= CancelNotch && m_atsButton){cancelAts();} // ATS�m�F
}

// ����������
void CAtssn::InitSn(void)
{
	AtsBrake = false;
	m_atsAlarm = 0;
	Bell = ATS_SOUND_STOP;
	Chime = ATS_SOUND_STOP;
	WhiteLamp = 1;
	RedLamp = 0;
}

// ATS�m�F�{�^��������
void CAtssn::DownButtom(void)
{
	m_atsButton = true;
	CheckAts();
}

// �`���C��������
void CAtssn::CancelChime(void)
{
	if(m_atsAlarm == 1)
	{
        m_atsAlarm = 0;
		Chime = ATS_SOUND_STOP;
        // ���b�Z�[�W�u�����`���C�����~�߂Ă͂����܂���v
	}
}

// ATS�m�F�{�^���𗣂�
void CAtssn::UpButton(void)
{
	m_atsButton = false;
}

// �����O�n��q�ʉ�
void CAtssn::PassedLong(int signal)
{
	if(signal == 0)
	{
		if(m_atsAlarm != 2)
		{
			m_tmrAts = *Time + 5000;
			m_tmrAtspassAlarm = 0;
			alarmSound();
			// ���b�Z�[�W�uATS�m�F���������Ă��������v
		}
	}
	else if(WhiteLamp == 0)
	{
		m_tmrAts = *Time + 5000;
		m_tmrAtspassAlarm = *Time + 375;
		alarmSound();
	}
}

// �����n��q�ʉ�
void CAtssn::PassedShort(int signal)
{
	if(signal == 0)
	{
		if(!AtsBrake)
		{
			AtsBrake = true; // �u���[�L����
			// ���b�Z�[�W�u�M�������Ȃ����v
			if(m_atsAlarm != 2){alarmSound();}
		}
	}
	else if(WhiteLamp == 0)
	{
		m_tmrAts = *Time + 5000;
		m_tmrAtspassAlarm = *Time + 375;
		alarmSound();
	}
}

// ���A
void CAtssn::Reset(void)
{
	if(*Notch == EmgNotch && AtsBrake)
	{
		AtsBrake = false; // �u���[�L�ɉ�
		m_atsAlarm = 0; // �`���C���ߒ�
		Bell = ATS_SOUND_STOP; // �x����~
		WhiteLamp = 1; // ���F���_��
		RedLamp = 0; // �ԐF���œ�
	}
}

// �J��
void CAtssn::TurnOff()
{
	AtsBrake = false; // �u���[�L�ɉ�
	m_atsAlarm = 0; // �`���C���ߒ�
	Bell = ATS_SOUND_STOP; // �x����~
	Chime = ATS_SOUND_STOP; // �`���C����~
	WhiteLamp = 0; // ���F���œ�
	RedLamp = 0; // �ԐF���œ�
}

// ATS�m�F
void CAtssn::cancelAts()
{
	if(m_atsAlarm == 2 && !AtsBrake)
	{
		m_atsAlarm = 1; // �`���C���ߒ�
		Bell = ATS_SOUND_STOP; // �x����~
		Chime = ATS_SOUND_PLAYLOOPING; // �`���C����
		WhiteLamp = 1; // ���F���_��
		RedLamp = 0; // �ԐF���œ�
	}
}

// ATS�x���炷
void CAtssn::alarmSound(void)
{
	m_atsAlarm = 2; // �x���ߒ�
	m_blnAtsLamp = true; // �ԐF���_��
	Chime = ATS_SOUND_STOP; // �`���C����~
	Bell = ATS_SOUND_PLAYLOOPING; // �x����
	WhiteLamp = 0; // ���F���œ�
	RedLamp = 1; // �ԐF���_��
}
void CAtssn::CheckPanel(void){
	Bell = ATS_SOUND_PLAYLOOPING; // �x����
}