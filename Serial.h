class Serial
{
private:
	bool isConnect;
public:
	Serial(void);
	virtual ~Serial(void);
	void Init(void);
	int Serial::OpenPort(void);
	void blow(void); // �P�Ńx����炷
	void sendMessage(LPVOID buffer, int toWriteBytes);
};