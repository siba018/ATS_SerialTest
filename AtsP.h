#define ATSP_DECELERATION 19.5F // �����萔 (�����x[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define ATSP_OFFSET 0.5F // �ԏ�q�I�t�Z�b�g[m]
#define ATSP_VO 25 // ��~�\��_����M���@�܂ł̋���[m]

class CAtsp
{
private:
	float m_DistSig; // ��~�\��_�܂ł̋���[m]
	float m_DistLimSpd; // ���x�����ʒu�܂ł̋���[m]
	int m_target; // �ڕW���x[km/h]
	int m_limSpd; // �������x[km/h]
	int m_maxSpd; // �ō����x[km/h]
	int m_approachLampOld; // �O��̃p�^�[���ڋߕ\����
	int m_bell; // �P�Ńx���̏��

	void applyBrake(int); // �u���[�L����
	void startAtsp(void); // ATS-P����
	void blowBell(bool); // �P�Ńx����炷
	void applyEmgBrake(void); // ���u���[�L����
	void init(void); // �ϐ��̏�����

public:
	float *TrainSpeed; // ��ԑ��x[km/h]
	int *DeltaT; // �t���[������[ms/frame]
	int *BrakeNotch; // �u���[�L�m�b�`
	int *Reverser; // ���o�[�T�[�ʒu
    int EmergencyNotch; // �ԗ��̔��m�b�`
	int ServiceNotch; // �ԗ���ATS�m�F�m�b�`
	int Bell; // �P�Ńx��
	int ApproachLamp; // �p�^�[���ڋߕ\����
	int BrakeApplyLamp; // �u���[�L����\����
	int AtspLamp; // ATS-P�\����
	bool ServiceBrake; // ��p�u���[�L
	bool EmergencyBrake; // ���u���[�L
    
	CAtsp(void);
	virtual ~CAtsp(void);
	void RunP(void);
	void InitP(void);
	void PassedSig(float);
	void PassedStopEmg(float);
	void PassedStopSvc(float);
	void PassedSpeedLim(int, int);
	void PassedSpeedMax(int);
	void Reset(void);
	void Exit(void);
	void CheckSound(void);
	
};
