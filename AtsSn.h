class CAtssn
{
private:
	int m_atsAlarm; // �x��ߒ�
	int m_tmrAtspassAlarm; // �`���C���ؑփ^�C�}
	int m_tmrAts; // 5�b�^�C�}
	int m_blnAtsLamp; // �ԐF��
	bool m_atsButton; // ATS�{�^��

	void alarmSound(void);
	void cancelAts(void);

public:
	int *Time; // ���ݎ���[s]
	int *Notch; // �u���[�L�m�b�`
	int CancelNotch; // �ԗ���ATS�m�F�m�b�`
	int EmgNotch; // �ԗ��̔��m�b�`
	bool AtsBrake; // ATS�u���[�L
	int Bell; // ATS�x��
	int Chime; // �����`���C��
	int RedLamp; // �ԐF��
	int WhiteLamp; // ���F��

	CAtssn(void);
	virtual ~CAtssn(void);
	void RunSn(void);
	void CheckAts(void);
	void InitSn(void);
	void DownButtom(void);
	void CancelChime(void);    
	void UpButton(void);
	void PassedLong(int);
	void PassedShort(int);
	void Reset(void);
	void TurnOff(void);
	void CheckPanel(void);
};
