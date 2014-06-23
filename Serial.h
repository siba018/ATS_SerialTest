class Serial
{
private:
	bool isConnect;
public:
	Serial(void);
	virtual ~Serial(void);
	void Init(void);
	int Serial::OpenPort(void);
	void blow(void); // ’P‘Åƒxƒ‹‚ð–Â‚ç‚·
	void sendMessage(LPVOID buffer, int toWriteBytes);
};