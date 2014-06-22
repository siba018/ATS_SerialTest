#define SPP_DECELERATION 19.5F // �����萔 (�����x[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define SPP_OFFSET 0.5F // �ԏ�q�I�t�Z�b�g[m]
#define SPP_ALARMTYPE 0 // 0: �J��Ԃ�����, 1; 1�񔭉�

class CSpp
{
private:
	float m_dist; // ��~�\��_����
	bool m_played;
	bool m_halt;
	int m_haltChime;
	int m_passAlarm;

public:
	float *TrainSpeed; // ��ԑ��x[km/h]
	int *DeltaT; // �t���[������
	int *BrakeNotch; // ���݂̃u���[�L�m�b�`

	int HaltChime; // ��ԃ`���C��
	int PassAlarm; // �ʉ߈ē�����
	int ServiceNotch; // ATS�m�F�m�b�`

	CSpp(void);
	virtual ~CSpp(void);
	void RunSpp(void);
	void InitSpp(void);
	void Receive(int);
	void NotchChanged(void);
	void StopChime(void);
};
