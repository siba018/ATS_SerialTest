#include "stdafx.h"
#include "atsplugin.h"
#include "Spp.h"

CSpp::CSpp()
{
}

CSpp::~CSpp()
{
}

void CSpp::RunSpp()
{
	float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]
    float def = speed / 3600 * *DeltaT; // 1�t���[���œ���������(��Βl)[m]
	m_dist -= def; // �c�苗�������Z����

	// �x���炷
	if(speed * speed / SPP_DECELERATION >= m_dist - 50 && m_dist > 0 && !m_played) // ���x�ƍ� (�p�^�[��)
	{
		m_played = true;
		if(m_halt) // ��Ԃ̏ꍇ
		{
#if(SPP_ALARMTYPE) // 1�񔭉��^
			m_haltChime = ATS_SOUND_PLAY;
#else              // �J��Ԃ������^
			if(*BrakeNotch >= ServiceNotch)
			{
				m_haltChime = -1200; // �u���[�L�����߂��ꍇ�͉��ʂ����������� (-12dB)
			}
			else
			{
				m_haltChime = 0;
			}
#endif
		}
		else // �ʉ߂̏ꍇ
		{
			m_passAlarm = ATS_SOUND_PLAY;
		}
	}

	// �T�E���h���X�V
	HaltChime = m_haltChime;
	PassAlarm = m_passAlarm;

	// CONTINUE�ɂ��Ă���
	if(m_haltChime == ATS_SOUND_PLAY){m_haltChime = ATS_SOUND_CONTINUE;}
	m_passAlarm = ATS_SOUND_CONTINUE;
}

void CSpp::NotchChanged()
{
	// �u���[�L�����߂��ꍇ�͉��ʂ����������� (-12dB)
	if(m_haltChime == 0 && *BrakeNotch >= ServiceNotch){m_haltChime = -1200;}
}

// �`���C��������
void CSpp::StopChime()
{
	m_haltChime = ATS_SOUND_STOP;
}

// �ԏ�q�̏����󂯎��
void CSpp::Receive(int data)
{
	m_halt = data >= 0;
	m_dist = abs(data) - SPP_OFFSET;
	m_played = false;
	m_haltChime = ATS_SOUND_STOP;
}

// ����������
void CSpp::InitSpp()
{
	m_dist = 0;
	m_haltChime = ATS_SOUND_STOP;
}