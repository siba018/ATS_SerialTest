class Serial
{
private:
	bool isConnect;
public:
	Serial(void);
	virtual ~Serial(void);
	void Init(void);
	int Serial::OpenPort(void);
	void blow(void); // 単打ベルを鳴らす
	void sendMessage(LPVOID buffer, int toWriteBytes);
};