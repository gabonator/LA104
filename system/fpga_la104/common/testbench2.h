#include <string>

class CTestBenchCollect
{
public:
    enum {Label, Error, GroupBegin, GroupEnd, Debug, Verilator};
    int mTick{0};
    std::string bufClk, bufCs, bufSck, bufMiso, bufMosi;
    struct message_t {
      int tick;
      int type;
      std::string text;
    };
    std::vector<message_t> mMessages;
    struct group_t {
      int begin;
      int end;
      std::string color;
    };
    std::vector<group_t> mGroups;
    group_t mCurrentGroup;

public:
    void Collect(int _clk, int _cs, int _sck, int _miso, int _mosi)
    {
      const char mapping[] = "01";
      bufClk += mapping[_clk];
      bufCs += mapping[_cs];
      bufSck += mapping[_sck];
      bufMiso += mapping[_miso];
      bufMosi += mapping[_mosi];
      mTick++;
    }

    void Message(int type, const char* text)
    {
      if (type == GroupBegin)
        mCurrentGroup.begin = mTick;
      if (type == GroupEnd)
      {
        std::vector<std::string> colors = {"#ffb0b0", "#b0ffb0", "#b0b0ff"};
        mCurrentGroup.end = mTick;
        mCurrentGroup.color = colors[mGroups.size() % colors.size()];
        mGroups.push_back(mCurrentGroup);
      }
      mMessages.push_back(message_t{mTick, type, std::string(text)});
    }

    void Dump()
    {
      printf("testbench_verilator = {\n");
      printf("  waveforms: {\n");
      printf("     clk: \"%s\",\n", bufClk.c_str());
      printf("     sck: \"%s\",\n", bufSck.c_str());
      printf("      cs: \"%s\",\n", bufCs.c_str());
      printf("    mosi: \"%s\",\n", bufMosi.c_str());
      printf("    miso: \"%s\",\n", bufMiso.c_str());
      printf("  },\n");
      printf("  labels: {\n");
      for (const auto& label : mMessages)
        printf("    {pos: %d, type: %d, text: \"%s\"},\n", label.tick, label.type, label.text.c_str());
      printf("  },\n");
      printf("  groups: {\n");
      for (const auto& group : mGroups)
        printf("    {begin: %d, end: %d, color: \"%s\"},\n", group.begin, group.end, group.color.c_str());
      printf("  },\n");
      printf("};\n");
/*
testbench_target = {
  fpga: {
    file: "BENCH.FPG",
    timestamp: "23/01/10 17:33",
    crc: 0
  },
  verify: {
    file: null,
    timestamp: "Default wave",
    crc: 0
  },
  waveforms: {
     clk: "110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011000000110011001100110011001100",
     sck: "000000000000000000000000000000000000000000000000000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111110000000011111111000000001111111100000000111111111111111111111111111111111111",
      cs: "111111111111111111111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111111111111111111111",
    mosi: "000000000000000000000000000000000000000000000000111111111111111111111111111111111111111111111111111111111111111100000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111111111111111111111111111111111111000000000000000000000000000000000000000000000000111111111111111111111111111111111111111111111111111111111111111111111111111111110000000000000000000000000000000011111111111111110000000000000000000000000000000011111111111111111111111111111111111111111111111100000000000000000000000000000000111111111111111111111111111111110000000000000000111111111111111100000000000000000000000000000000111111111111111100000000000000000000000000000000000000000000000011111111111111110000000000000000111111111111111100000000000000001111111111111111000000000000000000000000000000001111111111111111000000000000000011111111111111110000000000000000000000000000000011111111111111110000000000000000000000000000000000000000000000000000000000000000000000000000",
    miso: "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz0000000000000000000000000000000000000000000000000000000000000000111111111111111100000000000000001111111111111111zzzzzzzzzzzzzzzz0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000zzzzzzzzzzzzzzzz0000000000000000000000000000000000000000000000000000000000000000000000000000000011111111111111111111111111111111zzzzzzzzzzzzzzzz0000000000000000000000000000000000000000000000000000000000000000000000000000000011111111111111110000000000000000zzzzzzzzzzzzzzzz0000000000000000000000000000000000000000000000000000000000000000111111111111111100000000000000001111111111111111zzzzzzzzzzzzzzzz0000000000000000000000000000000000000000000000000000000000000000111111111111111100000000000000000000000000000000zzzzzzzzzzzzzzzz00000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111111111111111111111zzzzzzzzzzzzzzzzzzzzzzz"
  },
  labels: [
    {pos: 48, type: 2, text: "write 0xf0"},
    {pos: 48, type: 3, text: "read 0x85"},
    {pos: 176, type: 2, text: "write 0xf1"},
    {pos: 176, type: 3, text: "read 0x00"},
    {pos: 304, type: 2, text: "write 0xf2"},
    {pos: 304, type: 3, text: "read 0x83"},
    {pos: 432, type: 2, text: "write 0x73"},
    {pos: 432, type: 3, text: "read 0x02"},
    {pos: 560, type: 2, text: "write 0x48"},
    {pos: 560, type: 3, text: "read 0x85"},
    {pos: 688, type: 2, text: "write 0xa9"},
    {pos: 688, type: 3, text: "read 0x04"},
    {pos: 816, type: 2, text: "write 0xf5"},
    {pos: 816, type: 3, text: "read 0x87"},
    {pos: 948, type: 0, text: "release"}
  ],
  groups: [
    {begin: 48, end: 176, color: "#f8b0b0"},
    {begin: 176, end: 304, color: "#b0fcb0"},
    {begin: 304, end: 432, color: "#b0b0f8"},
    {begin: 432, end: 560, color: "#f8b0b0"},
    {begin: 560, end: 688, color: "#b0fcb0"},
    {begin: 688, end: 816, color: "#b0b0f8"},
    {begin: 816, end: 944, color: "#f8b0b0"}
  ]
};

*/
    }
};

class CTestBenchIo : public CTestBenchCollect
{
public:
  int mTick{0};
  static Vapp* mTop;

public:
    void eval()
    {
        mTop->eval();
        Collect(clk(), cs(), sck(), miso(), mosi());
        mTop->eval();
        Collect(clk(), cs(), sck(), miso(), mosi());
    }
    bool cs()
    {
        return mTop->SSEL;
    }
    void cs(bool l)
    {
        mTop->SSEL = l;
    }
    bool sck()
    {
        return mTop->SCK;
    }
    void sck(bool l)
    {
        mTop->SCK = l;
    }
    bool miso()
    {
        return mTop->MISO;
    }
    void miso(bool l)
    {
        mTop->MISO = l;
    }
    bool mosi()
    {
        return mTop->MOSI;
    }
    void mosi(bool l)
    {
        mTop->MOSI = l;
    }
    bool clk()
    {
        return mTop->clk;
    }
    void clk(bool l)
    {
        mTop->clk = l;
    }
    void misoFloat()
    {
    }
};

class CTestBench : public CTestBenchIo
{
  bool mLsbFirst{false};
public:
  static CTestBench* mInstance;

public:
    void spiBegin(bool lsb)
    {
        mInstance = this; // TODO!
        mLsbFirst = lsb;
        cs(1);
        clk(0);
        sck(0);
        mosi(0);
        misoFloat();
    }

    void spiEnd()
    {        
    }
    
    void tick()
    {
        const int oversampling = 4;

        for (int i=0; i<oversampling; i++)
        {
            clk(!clk());
            eval();
        }
    }
    
    uint8_t spiTick(uint8_t b)
    {
        mosi(b);
        sck(0);
        tick();
        sck(1);
        tick();
        return miso();
    }
    
    uint8_t spiTransfer(uint8_t d)
    {
        uint8_t aux = 0;
        if (mLsbFirst)
        {
          for (int i=0; i<8; i++)
          {
              aux >>= 1;
              aux |= spiTick(d & 1) ? 128 : 0;
              d >>= 1;
          }
        } else {
          for (int i=0; i<8; i++)
          {
              aux <<= 1;
              aux |= spiTick(d & 128);
              d <<= 1;
          }
        }
        return aux;
    }
    void printError(const char* msg)
    {
       Message(Error, msg);
    }
    void printLabel(const char* msg)
    {
       Message(Label, msg);
    }
    void groupBegin(const char* msg)
    {
       Message(GroupBegin, msg);
    }
    void groupEnd(const char* msg = nullptr)
    {
       Message(GroupEnd, msg);
    }
    void printVerilator(const char* msg)
    {
       Message(Verilator, msg);
    }
};

CTestBench* CTestBench::mInstance{nullptr};
Vapp* CTestBenchIo::mTop{nullptr};

bool main_verify(CTestBench& test);

namespace Verilator {
  int PrintSim(const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    char temp[1024];
    vsprintf(temp, format, args);
    assert(CTestBench::mInstance);
    CTestBench::mInstance->printVerilator(temp);
    va_end(args);
    return 0;
  }
}

int main()
{
    CTestBench instance;

    Verilated::debug(0);
    Verilated::randReset(2);
    CTestBench::mTop = new Vapp;
    main_verify(instance);
    CTestBench::mTop->final();
    delete CTestBench::mTop;
    CTestBench::mTop = nullptr;
    instance.Dump();
    return 0;
}

