class Serial
{
private:

public:
	Serial(void);
	virtual ~Serial(void);
	int Init(void);
	void blow(void); // ’P‘Åƒxƒ‹‚ð–Â‚ç‚·
	void sendMessage(LPVOID buffer, int toWriteBytes);
};