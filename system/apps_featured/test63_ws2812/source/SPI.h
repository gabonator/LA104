class CSPI
{
public:
	enum {SPI_CLOCK_DIV32=0x0020};

public:
	void begin();
	void end();
	void setClockDivider(int clockDivider);
	void dmaSendAsync(uint8_t* data, int len);
};

extern CSPI SPI;
