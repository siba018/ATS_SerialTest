class Serial
{
private:

public:
	Serial(void);
	virtual ~Serial(void);
	int Init(void);
	void blow(void); // �P�Ńx����炷
	void sendMessage(LPVOID buffer, int toWriteBytes);
};