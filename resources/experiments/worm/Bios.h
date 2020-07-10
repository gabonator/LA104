#ifndef __BIOS_H__
#define __BIOS_H__

#include "Classes.h"
#include "Types.h"

class DLLAPI BIOS {
public:
	class DLLAPI DBG {
	public:
		static void Print (const char* format, ...);
		// move to utils?
		static int sprintf(char* buf, const char * format, ...);
	};

	class DLLAPI SYS {
	public:
		enum {
			EApp1 = 0,
			EApp2,
			EApp3,
			EApp4,
			ESys,
			EDfu
		};

		static void DelayMs(unsigned short ms);
		static ui32 GetTick();
		static void Init();
		static void Beep( int ms );
		static int GetBattery();
		static void SetBacklight(int nLevel); // 0..100
		static void SetVolume(int nLevel); // 0..100
		static int Execute( int nCode );
		static void* IdentifyApplication( int nCode );
		static void Set( int nKey, int nValue );
		static int Get( int nKey, int nSub = 0 );
		static int GetTemperature();
		static int GetCoreVoltage();
		static void Standby( bool bEnterSleep );

		static ui32 GetProcAddress( const char* strFunction );
		static bool IsColdBoot();
		static char* GetSharedBuffer();
		static int GetSharedLength();
	};

	class DLLAPI LCD {
	public:
		enum {
			// Screen resolution
			LcdWidth = 400,
			LcdHeight = 240,

			// BufferBegin mode
			BfXY = 1,
			BfYX = 2
		};

	public:
		static void Init();
		static int Printf (int x, int y, unsigned short clrf, unsigned short clrb, const char* format, ...);
		static int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str);
		static int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str);
		static int Print (const CPoint& cp, unsigned short clrf, char *str);
		static void Line(int x1, int y1, int x2, int y2, unsigned short clr);
		static ui16 GetPixel(int x, int y);
		static void PutPixel(int x, int y, unsigned short clr);
		static void PutPixel(const CPoint& cp, unsigned short clr);
		static void Clear(unsigned short clr);
		static void RoundRect(int x1, int y1, int x2, int y2, unsigned short clr);
		static void RoundRect(const CRect& rc, unsigned short clr);
		static void Bar(int x1, int y1, int x2, int y2, unsigned short clr);
		static void Bar(const CRect& rc, unsigned short clr);
		static void Rectangle(const CRect& rc, unsigned short clr);
		static void Pattern(int x1, int y1, int x2, int y2, const ui16 *pat, int l);	
		static int Draw(int x, int y, unsigned short clrf, unsigned short clrb, const char *p);
		
		static void BufferBegin(const CRect& rc, ui8 nMode);
		static void BufferPush(ui16 clr);
		static void BufferEnd();
		static void Buffer(int x, int y, unsigned short* pBuffer, int n);
		
		static void GetImage(const CRect& rcRect, ui16* pBuffer );
		static void PutImage(const CRect& rcRect, ui16* pBuffer );
		static void Shadow(int x1, int y1, int x2, int y2, unsigned int nColor);
	
		static const void* GetCharRom();
	};

	class DLLAPI KEY {
	public:
		enum {
			// Key definitions
			KeyLeft = 1,
			KeyRight = 2,
			KeyUp = 4,
			KeyDown = 8,
			KeyEnter = 16,
			KeyEscape = 32,
			KeyFunction = 64,
			KeyFunction2 = 128,
			KeyS1 = 256,
			KeyS2 = 512
		};

	public:
		static ui16 GetKeys();
	};

	class DLLAPI ADC {
	public:
		typedef unsigned long TSample;
		union SSample {
			struct
			{
				ui8 CH1 : 8;
				ui8 CH2 : 8;
				ui8 CH3 : 1;
				ui8 CH4 : 1;
				ui8 dummy0 : 6;
				ui8 dummy1 : 8;
			};
			ui8 CH[4];
			ui32 nValue;
		};
	
		enum EState {
			Start = 0,
			Empty = 1,
			Full = 2
		};
		
		enum {
			Length = 4096,
			BufferLength = Length*4
		};

	public:
		static void Init();
		static unsigned char Ready();
		static EState GetState();

	private:
		static unsigned long Get();

	public:
		static int GetPointer();
		static void Restart();
		static void Enable(bool bEnable);
		static bool Enabled();
		static void Configure(ui8 nACouple, ui8 nARange, ui16 nAOffset, ui8 nBCouple, ui8 nBRange, ui16 nBOffset, float fTimePerDiv);
		static void ConfigureTrigger(ui16 nTThreshold, ui16 nVThreshold, ui8 nSource, ui8 nType);
//		static void ConfigureBuffer(int nLength);
//		static void GetBufferRange(int& nBegin, int& nEnd);

		static void Copy(int nCount);
		static unsigned long GetCount();
		static TSample& GetAt(int i);
	};

	class DLLAPI GEN {
	public:
		enum
			{
				DcMin = 0,
				DcMax = 0xfff
			};

	public:
		static void ConfigureSq(ui16 psc, ui16 arr, ui16 ccr);
		static void ConfigureWave(ui16* pData, ui16 cnt);
		static void ConfigureWaveRate(ui16 arr);
		static void ConfigureDc(ui16 nData);
	};

	class DLLAPI DSK {
	public:
		enum {
			IoRead = 1,
			IoWrite = 2,
			IoClosed = 3
		};
		static PVOID GetSharedBuffer();
		static BOOL Open(FILEINFO* pFileInfo, const char* strName, ui8 nIoMode);
		static BOOL Read(FILEINFO* pFileInfo, ui8* pSectorData);
		static BOOL Write(FILEINFO* pFileInfo, ui8* pSectorData);
		static BOOL Close(FILEINFO* pFileInfo, int nSize = -1);
	};

	class DLLAPI SERIAL
	{
	public:
		static void Init();
		static void Configure(int nBaudrate);
		static void Send(const char* strBuf);
		static int Getch();
		static void Putch(char ch);
	};

	class DLLAPI VER
	{
	public:
		static const char* GetHardwareVersion();
		static const char* GetSystemVersion();
		static const char* GetFpgaVersion();
		static const char* GetDfuVersion();
		static ui32 GetDisplayType();
		static ui32 GetSerialNumber();
		static void DrawLogo(int x, int y);
	};

	class DLLAPI MOUSE
	{
	public:
		enum {
			Click = 1,
			DbClick = 2
		};
	public:
		static bool IsSupported();
		static int GetX();
		static int GetY();
		static bool GetDown();
	};

	class DLLAPI GPIO
	{
	public:
		enum {
			// nPort
			PortA = 0,
			PortB = 1,
			PortC = 2,
			PortD = 3,

			// nReg
			RegCrl = 0,
			RegCrh = 1,
			RegIdr = 2,
			RegOdr = 3,
			RegBsrr = 4,
			RegBrr = 5,
			RegLckr = 6,
			
			// nState
			StateInput = 0,           // ..00
			StateOutput10Mhz = 1,     // ..01
			StateOutput2Mhz = 2,      // ..10
			StateOutput50Mhz = 3,     // ..11

			StateInputAnalog = 0<<2,
			StateInputFloating = 1<<2,
			StateInputPull = 2<<2,

			StateOutputPushPull = 0<<2,
			StateOutputOpenDrain = 1<<2,
			StateOutputFunctionPushPull = 2<<2,
			StateOutputFunctionOpenDrain = 3<<2,
			
			// macros for easy use
			StateSimpleInput = StateInput | StateInputFloating,
			StateSimpleOutput = StateOutput2Mhz | StateOutputPushPull
		};

	public:
		static ui32* GetRegister(int nPort, int nReg);
		static void SetState(int nPort, int nPin, int nState);
		static void SetPin(int nPort, int nPin, bool bOn);
		static bool GetPin(int nPort, int nPin);
	};

#ifdef _VERSION2	
	class DLLAPI MEMORY
	{
	public:
		static bool PageWrite(int nPage, const ui8* pBuffer);
		static bool PageRead(int nPage, ui8* pBuffer);
		static bool PageErase(int nPage);

		static void LinearStart();
		static bool LinearFinish();
		static bool LinearProgram( ui32 nAddress, unsigned char* pData, int nLength );
	};

	class DLLAPI FAT
	{
	public:
		enum EResult 
		{
			EOk,
			EDiskError,
			EIntError,
			ENoFile,
			ENoPath,
			EDiskFull
		};

		enum EAttribute 
		{
			EReadOnly = 1,
			EHidden = 2,
			ESystem = 4,
			EDirectory = 0x10,
			EArchive = 0x20
		};

		struct TFindFile
		{
			ui32 nFileLength;		
			ui16 nDate;
			ui16 nTime;
			ui8 nAtrib;
			char strName[13];
		};

		static EResult Init();
		static EResult Open(const char* strName, ui8 nIoMode);
		static EResult Read(ui8* pSectorData);
		static EResult Write(ui8* pSectorData);
		static EResult Seek(ui32 lOffset);
		static EResult Close(int nSize = -1);
		static ui32 GetFileSize();
	
		static EResult OpenDir(char* strPath);
		static EResult FindNext(TFindFile* pFile);
	};
#endif
};
#endif