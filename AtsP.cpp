#include "stdafx.h"
#include "atsplugin.h"
#include "Atsp.h"

CAtsp::CAtsp()
{
}

CAtsp::~CAtsp()
{
}

// ����������
void CAtsp::InitP()
{
	blowBell(false);
	m_approachLampOld = 0;
	init();
}

// ATS-SN�ɐؑ�
void CAtsp::Exit()
{
	if(AtspLamp){blowBell(false);}
	init();
}

// �n��q�ʉ�
void CAtsp::PassedSig(float distance)
{
	startAtsp();
	m_DistSig = distance - ATSP_VO;
}

// ����푬�x�����n��q�ʉ�
void CAtsp::PassedSpeedLim(int speed, int distance)
{
	startAtsp();
	m_limSpd = speed;
	m_DistLimSpd = distance - ATSP_OFFSET;
}

// �ō����x��ݒ�
void CAtsp::PassedSpeedMax(int speed)
{
	startAtsp();
	m_maxSpd = speed;
}

// ������~(���)�n��q�ʉ�
void CAtsp::PassedStopEmg(float distance)
{
	startAtsp();
	m_DistSig = distance - ATSP_VO;

	if(distance < 50 && *TrainSpeed != 0){applyEmgBrake();}
}

// ������~(��p)�n��q�ʉ�
void CAtsp::PassedStopSvc(float distance)
{
	startAtsp();
	m_DistSig = distance - ATSP_VO;

	if(distance < 50 && *TrainSpeed != 0){applyBrake(0);}
}

// ���A
void CAtsp::Reset()
{
	if(EmergencyBrake && *BrakeNotch == EmergencyNotch){EmergencyBrake = false;} // ���u���[�L����̏ꍇ
	if(ServiceBrake && *BrakeNotch >= ServiceNotch){ServiceBrake = false;} // ��p�u���[�L����̏ꍇ

	if(!EmergencyBrake && !ServiceBrake)
	{
		BrakeApplyLamp = 0; // �u���[�L����\����
		ApproachLamp = 0; // �p�^�[���ڋߕ\����
	}
}

void CAtsp::RunP()
{
	float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]
    float def = speed / 3600 * *DeltaT; // 1�t���[���œ���������(��Βl)[m]
	m_DistSig -= def; // �c�苗�������Z����
	m_DistLimSpd -= def; // �c�苗�������Z����
	if(m_DistLimSpd < 0){m_DistLimSpd = 2000000;} // ����_���z������p�^�[������������

	// ��ތ��m
	if(*TrainSpeed * *Reverser < -6){applyEmgBrake();}

	// �p�^�[�����v�Z����
	float patternSig = speed * speed / ATSP_DECELERATION; // ��~�M��
	float pSeepdLim = (speed * speed - m_limSpd * m_limSpd) / ATSP_DECELERATION; // ���x����

	// �ɉ�����
	if(ServiceBrake)
	{
		// �ڕW���x�ȉ��ɂȂ邩�ڋ߃p�^�[�����������(�����A�b�v����)��ɉ�����
		if(speed < m_target || (patternSig < m_DistSig - 50 && m_target <= 0))
		{
			ServiceBrake = false;
			BrakeApplyLamp = 0;
			ApproachLamp = 0; // �p�^�[������
		}
	}

	// �u���[�L����
	if(!ServiceBrake && !EmergencyBrake)
	{
		// �p�^�[���ڋ�
		if(patternSig >= m_DistSig - 50 && m_DistSig > -50 - ATSP_VO || 
		   pSeepdLim >= m_DistLimSpd - 50 && m_DistLimSpd > 0 && speed >= m_limSpd ||
		   speed >= m_maxSpd - 5)
		{
			ApproachLamp = 1; // �p�^�[���ڋ�
		}
		else
		{
			ApproachLamp = 0; // �p�^�[������
		}
		
		// �u���[�L����
		if(patternSig >= m_DistSig && speed >= 16 && m_DistSig > -50 - ATSP_VO) // ��~�M���ɑ΂���u���[�L�p�^�[��
		{
			applyBrake(0);
		}
		if(pSeepdLim >= m_DistLimSpd && m_DistLimSpd > 0) // ���x�����ɑ΂���u���[�L�p�^�[��
		{
			applyBrake(m_limSpd - 5);
		}
		if(speed >= m_maxSpd + 1) // �ō����x�ɑ΂���u���[�L�p�^�[��
		{
			applyBrake(m_maxSpd - 5);
		}
	}

	if(ApproachLamp != m_approachLampOld)
	{
		blowBell((bool)m_approachLampOld); // �P�Ńx����炷
		m_approachLampOld = ApproachLamp;
	}

	Bell = m_bell; // �T�E���h���X�V
	m_bell = ATS_SOUND_CONTINUE; // CONTINUE�ɂ��Ă���
}

// ����������
void CAtsp::init()
{
	m_DistSig = -50 - ATSP_VO; // ��~�\��_����
	m_DistLimSpd = 0; // ���x����
	m_maxSpd = 370; // �ō����x[km/h]
	ApproachLamp = 0; // �p�^�[���ڋߕ\����
	BrakeApplyLamp = 0; // �u���[�L����\����
	ServiceBrake = false; // ��p�u���[�L
	EmergencyBrake = false; // ���u���[�L
	AtspLamp = 0; // ATS-P�\����
}

// ATS-P����
void CAtsp::startAtsp()
{
	if(AtspLamp == 0)
	{
		AtspLamp = 1; // ATS-P�\�����_��
		blowBell(false); // �P�Ńx����炷
	}
}

// �u���[�L����
void CAtsp::applyBrake(int targetSpeed)
{
	ServiceBrake = true;
	BrakeApplyLamp = 1; // �u���[�L����\�����_��
	m_target = targetSpeed; // �ڕW���x
}

// �P�Ńx����炷
void CAtsp::blowBell(bool off)
{
	if(*TrainSpeed != 0 || off){m_bell = ATS_SOUND_PLAY;} // �w�W�����v���ɂ͖炳�Ȃ�
}

// ���u���[�L����
void CAtsp::applyEmgBrake()
{
	EmergencyBrake = true; // ���u���[�L
	BrakeApplyLamp = 1; // �u���[�L����\�����_��
	ApproachLamp = 1; // �p�^�[���ڋߕ\�����_��
}
void CAtsp::CheckSound(void){
	m_bell = ATS_SOUND_PLAY;
}