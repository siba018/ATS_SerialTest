class Serial
{
private:

public:
	Serial(void);
	virtual ~Serial(void);
	int Init(void);
	void blow(void); // 単打ベルを鳴らす
	void sendMessage(LPVOID buffer, int toWriteBytes);
};