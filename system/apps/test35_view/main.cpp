#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

uint8_t mFileSystemBuffer[BIOS::FAT::SectorSize];

bool isdigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isalnum(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

using namespace BIOS;

class CTextReader : public CBufferedReader
{
    int mLineOffsetsPlacement[200];
    CArray<int> mLineOffsets;
    int mLineOffset{0};
    
    int mCurrentLine{0};
    
    static constexpr int mCharsPerLine{BIOS::LCD::Width/8-2+1};
    char mBuffer[mCharsPerLine+8];
    
public:
    CTextReader()
    {
        mLineOffsets.Init(mLineOffsetsPlacement, COUNT(mLineOffsetsPlacement));
        mLineOffsets.SetSize(COUNT(mLineOffsetsPlacement));
        for (int i=0; i<mLineOffsets.GetSize(); i++)
            mLineOffsets[i] = -1;
        mLineOffsets[0] = 0;
    }
    
    bool Open(const char* filename)
    {
        return CBufferedReader::Open((char*)filename);
    }
    
    int GetOffset(int line)
    {
        int index = line-mLineOffset;
        if (index >= 0 && index < mLineOffsets.GetSize())
            return mLineOffsets[index];
        
        return -1;
    }
    
    char* GetLine(int line)
    {
        if (mCurrentLine == line)
        {
            if (CBufferedReader::GetFileOffset() >= CBufferedReader::GetFileSize())
                return nullptr;
            
            bool wrap = false;
            int offset = CBufferedReader::GetFileOffset();
            CBufferedReader::ReadLine(mBuffer, mCharsPerLine, &wrap);
            if (!wrap)
                strcat(mBuffer, "\n");
            else
            {
                // TODO: duplicity
                // find last space
                for (int i=strlen(mBuffer)-1; i>mCharsPerLine/2; i--)
                    if (mBuffer[i] == ' ')
                    {
                        mBuffer[i] = 0;
                        CBufferedReader::Seek(offset+i+1);
                        break;
                    }
            }

            mCurrentLine++;
            int index = mCurrentLine-mLineOffset;
            if (index >= 0 && index < mLineOffsets.GetSize())
                mLineOffsets[index] = CBufferedReader::GetFileOffset();
            else
            {
                // change offset
            }
            return mBuffer;
        } else
        {
            int index = line-mLineOffset;
            if (index >= 0 && index < mLineOffsets.GetSize())
            {
                int offset = mLineOffsets[index];
                if (offset != -1)
                {
                    CBufferedReader::Seek(offset); // TODO: optimize
                    bool wrap = false;
                    CBufferedReader::ReadLine(mBuffer, mCharsPerLine, &wrap);
                    if (!wrap)
                    {
                        strcat(mBuffer, "\n");
                    }
                    else
                    {
                        // find last space
                        for (int i=strlen(mBuffer)-1; i>mCharsPerLine/2; i--)
                            if (mBuffer[i] == ' ')
                            {
                                mBuffer[i] = 0;
                                CBufferedReader::Seek(offset+i+1);
                                break;
                            }
                    }

                    mCurrentLine = line + 1;
                    mLineOffsets[mCurrentLine-mLineOffset] = CBufferedReader::GetFileOffset();
                    return mBuffer;
                }
            } else
            {
                // change offset & rescan file
            }
        }
        return nullptr;
    }
};

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
#ifndef EMSCRIPTEN
int _main(void)
{
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    BIOS::LCD::Print(4, 0, RGB565(ffffff), RGBTRANS, "File viewer");

    BIOS::FAT::SetSharedBuffer(mFileSystemBuffer);

#ifdef __APPLE__
    OS::SetArgument((char*)"/viewtxt.elf /Users/gabrielvalky/Documents/git/LA104/system/apps/test35_view/readme.txt");
    //const char* path = "/Users/gabrielvalky/Documents/git/LA104/system/apps/test35_view/test.txt";
#else
    //OS::SetArgument("/viewtxt.elf test.txt")
    //const char* path = "test.txt";
#endif
    
    char* fileName = OS::GetArgument();
    fileName = strstr(fileName, " ");
    if (!fileName)
    {
        BIOS::DBG::Print("$\nNo file specified.\n");
        BIOS::SYS::DelayMs(1000);
        return 1;
    }
    
    fileName++; // skip space
    
    CTextReader reader;
    if (!reader.Open(fileName))
    {
        BIOS::DBG::Print("$\nUnable to open file: '");
        BIOS::DBG::Print(fileName);
        BIOS::DBG::Print("'\n'");
        BIOS::SYS::DelayMs(1000);
        return 1;
    }
        
    char showName[26];
    
    if (strlen(fileName) < COUNT(showName)-1)
        strcpy(showName, fileName);
    else
    {
        strcpy(showName, "...");
        strcat(showName, fileName + strlen(fileName)-(COUNT(showName)-3-1));
    }
    BIOS::LCD::Print(BIOS::LCD::Width-4 - strlen(showName)*8, 0, RGB565(b0b0b0), RGBTRANS, showName);

    int base = 0;
    constexpr int lines = (BIOS::LCD::Height-16)/14;
    KEY::EKey key = KEY::EKey::None;

    auto CheckKey = [&]() -> bool{
        if (key == BIOS::KEY::None)
            key = BIOS::KEY::GetKey();
        return key != BIOS::KEY::None;
    };
    
    auto RedrawColored = [&](){
        bool inString = false;
        for (int i=0; i<(BIOS::LCD::Height-16)/14; i++)
        {
            char* buf = reader.GetLine(base + i);

            if (CheckKey())
                continue;

            int bx = 4;
            int by = 16+i*14;

            if (!buf)
            {
                LCD::Bar(bx, by, LCD::Width, by+14, RGB565(0000b0));
                continue;
            }
            
            int l = strlen(buf);
            char ch[2] = {0, 0};
            bool inDirective = buf[0] == '#';
            bool inNumber = false;
            bool inSpecial = false;
            bool inScope = false;
            bool inComment = false;
            bool finishString = false;
            int inKeyword = 0;
            for (int i=0; i<l; i++)
            {
                int c = buf[i];
                ch[0] = c;
                if (c == '\n' || c == '\r')
                {
                    inString = false;
                    continue;
                }
                
                if (c == '"' || c == '\'')
                {
                    if (inString)
                        finishString = true;
                    else
                        inString = true;
                }
                
                if (strstr("[]!=:();/*+-<>", ch) != nullptr)
                    inSpecial = true;
                if (strstr("{}", ch) != nullptr)
                    inScope = true;
                if (strncmp(buf+i, "//", 2) == 0)
                    inComment = true;
                
                bool wasSpacer = i==0 || !isalnum(buf[i-1]);
                if (wasSpacer)
                {
                    if (strncmp(buf+i, "if", 2) == 0 && !isalnum(buf[i+2]))
                        inKeyword = 2;
                    if (strncmp(buf+i, "int", 3) == 0 && !isalnum(buf[i+3]))
                        inKeyword = 3;
                    if (strncmp(buf+i, "void", 4) == 0 && !isalnum(buf[i+4]))
                        inKeyword = 4;
                    if (strncmp(buf+i, "char", 4) == 0 && !isalnum(buf[i+4]))
                        inKeyword = 4;
                    if (strncmp(buf+i, "class", 5) == 0 && !isalnum(buf[i+5]))
                        inKeyword = 5;
                    if (strncmp(buf+i, "while", 5) == 0 && !isalnum(buf[i+5]))
                        inKeyword = 5;
                    if (strncmp(buf+i, "using", 5) == 0 && !isalnum(buf[i+5]))
                        inKeyword = 5;
                    if (strncmp(buf+i, "return ", 6) == 0  && !isalnum(buf[i+6]))
                        inKeyword = 6;
                }
                if (inNumber && !isdigit(c))
                    inNumber = false;

                int color = RGB565(00ffff);
                if (inDirective || inNumber)
                    color = RGB565(00ff00);
                if (inKeyword || inSpecial)
                    color = RGB565(ffffff);
                if (inString || inScope)
                    color = RGB565(ffff00);
                if (inComment)
                    color = RGB565(b0b0b0);
                
                bx += LCD::Print(bx, by, color, RGB565(0000b0), ch);

                if (finishString)
                    inString = false;

                if (!isalnum(buf[i]) && isdigit(buf[i+1]))
                    inNumber = true;
                if (inKeyword)
                    inKeyword--;
                inSpecial = false;
            }
            LCD::Bar(bx, by, LCD::Width-8, by+14, RGB565(0000b0));
        }
    };

    auto Redraw = [&](){
        for (int i=0; i<(BIOS::LCD::Height-16)/14; i++)
        {
            char* buf = reader.GetLine(base + i);
            if (!buf)
                continue;

            if (CheckKey())
                continue;

            int bx = 8;
            int by = 16+i*14;
            bx += LCD::Print(bx, by, RGB565(00ffff), RGB565(0000b0), buf);
            LCD::Bar(bx, by, LCD::Width-8, by+14, RGB565(0000b0));
        }
    };

    auto DrawProgress = [&](){
        int ofs1 = reader.GetOffset(base);
        int ofs2 = reader.GetOffset(base + lines);
        int len = reader.GetFileSize();
        if (ofs2 == -1)
            ofs2 = len;
        if (ofs2 > len) // WTF???
            ofs2 = len;
        if (ofs1 != -1 && ofs2 != -1)
        {
            CRect rcProgress(LCD::Width - 6, 16, LCD::Width-2, LCD::Height);
            int top = rcProgress.top + rcProgress.Height()*ofs1/len;
            int bottom = rcProgress.top + rcProgress.Height()*ofs2/len;
            
            CRect rcTop(rcProgress);
            rcTop.bottom = top;
            CRect rcMid(rcProgress);
            rcMid.top = top;
            rcMid.bottom = bottom;
            CRect rcBottom(rcProgress);
            rcBottom.top = bottom;
            
            if (rcTop.Height() > 0)
                LCD::Bar(rcTop, RGB565(0000b0));
            if (rcMid.Height() > 0)
                LCD::Bar(rcMid, RGB565(b0b0b0));
            if (rcBottom.Height() > 0)
                LCD::Bar(rcBottom, RGB565(0000b0));
        }
    };
    (void)Redraw;
    (void)RedrawColored;
    
    RedrawColored();
    DrawProgress();
    bool interrupted = false;
    
    while (true)
    {
        if (key == KEY::EKey::None)
            key = KEY::GetKey();
        
        if (key == KEY::EKey::Escape)
            break;
        
        bool redraw = false;
        if (key == KEY::Down && reader.GetOffset(base+lines) != -1)
        {
            base++;
            redraw = true;
        } else
        if (key == KEY::Up && base > 0)
        {
            base--;
            redraw = true;
        }
        
        key = KEY::EKey::None;

        if (redraw || interrupted)
        {
            redraw = false;
            RedrawColored();
            interrupted = key != KEY::EKey::None;
            DrawProgress();
        }
    }

    BIOS::FAT::SetSharedBuffer(nullptr);
    
    return 0;
}
#else

    CTextReader reader;
    bool interrupted = false;
    int base = 0;
    constexpr int lines = (BIOS::LCD::Height-16)/14;
    KEY::EKey key = KEY::EKey::None;

    bool CheckKey() 
    {
        if (key == BIOS::KEY::None)
            key = BIOS::KEY::GetKey();
        return key != BIOS::KEY::None;
    };
    
    void RedrawColored()
    {
        bool inString = false;
        for (int i=0; i<(BIOS::LCD::Height-16)/14; i++)
        {
            char* buf = reader.GetLine(base + i);

            if (CheckKey())
                continue;

            int bx = 4;
            int by = 16+i*14;

            if (!buf)
            {
                LCD::Bar(bx, by, LCD::Width, by+14, RGB565(0000b0));
                continue;
            }
            
            int l = strlen(buf);
            char ch[2] = {0, 0};
            bool inDirective = buf[0] == '#';
            bool inNumber = false;
            bool inSpecial = false;
            bool inScope = false;
            bool inComment = false;
            bool finishString = false;
            int inKeyword = 0;
            for (int i=0; i<l; i++)
            {
                int c = buf[i];
                ch[0] = c;
                if (c == '\n' || c == '\r')
                {
                    inString = false;
                    continue;
                }
                
                if (c == '"' || c == '\'')
                {
                    if (inString)
                        finishString = true;
                    else
                        inString = true;
                }
                
                if (strstr("[]!=:();/*+-<>", ch) != nullptr)
                    inSpecial = true;
                if (strstr("{}", ch) != nullptr)
                    inScope = true;
                if (strncmp(buf+i, "//", 2) == 0)
                    inComment = true;
                
                bool wasSpacer = i==0 || !isalnum(buf[i-1]);
                if (wasSpacer)
                {
                    if (strncmp(buf+i, "if", 2) == 0 && !isalnum(buf[i+2]))
                        inKeyword = 2;
                    if (strncmp(buf+i, "int", 3) == 0 && !isalnum(buf[i+3]))
                        inKeyword = 3;
                    if (strncmp(buf+i, "void", 4) == 0 && !isalnum(buf[i+4]))
                        inKeyword = 4;
                    if (strncmp(buf+i, "char", 4) == 0 && !isalnum(buf[i+4]))
                        inKeyword = 4;
                    if (strncmp(buf+i, "class", 5) == 0 && !isalnum(buf[i+5]))
                        inKeyword = 5;
                    if (strncmp(buf+i, "while", 5) == 0 && !isalnum(buf[i+5]))
                        inKeyword = 5;
                    if (strncmp(buf+i, "using", 5) == 0 && !isalnum(buf[i+5]))
                        inKeyword = 5;
                    if (strncmp(buf+i, "return ", 6) == 0  && !isalnum(buf[i+6]))
                        inKeyword = 6;
                }
                if (inNumber && !isdigit(c))
                    inNumber = false;

                int color = RGB565(00ffff);
                if (inDirective || inNumber)
                    color = RGB565(00ff00);
                if (inKeyword || inSpecial)
                    color = RGB565(ffffff);
                if (inString || inScope)
                    color = RGB565(ffff00);
                if (inComment)
                    color = RGB565(b0b0b0);
                
                bx += LCD::Print(bx, by, color, RGB565(0000b0), ch);

                if (finishString)
                    inString = false;

                if (!isalnum(buf[i]) && isdigit(buf[i+1]))
                    inNumber = true;
                if (inKeyword)
                    inKeyword--;
                inSpecial = false;
            }
            LCD::Bar(bx, by, LCD::Width-8, by+14, RGB565(0000b0));
        }
    };

    void Redraw()
    {
        for (int i=0; i<(BIOS::LCD::Height-16)/14; i++)
        {
            char* buf = reader.GetLine(base + i);
            if (!buf)
                continue;

            if (CheckKey())
                continue;

            int bx = 8;
            int by = 16+i*14;
            bx += LCD::Print(bx, by, RGB565(00ffff), RGB565(0000b0), buf);
            LCD::Bar(bx, by, LCD::Width-8, by+14, RGB565(0000b0));
        }
    };

    void DrawProgress()
    {
        int ofs1 = reader.GetOffset(base);
        int ofs2 = reader.GetOffset(base + lines);
        int len = reader.GetFileSize();
        if (ofs2 == -1)
            ofs2 = len;
        if (ofs2 > len) // WTF???
            ofs2 = len;
        if (ofs1 != -1 && ofs2 != -1)
        {
            CRect rcProgress(LCD::Width - 6, 16, LCD::Width-2, LCD::Height);
            int top = rcProgress.top + rcProgress.Height()*ofs1/len;
            int bottom = rcProgress.top + rcProgress.Height()*ofs2/len;
            
            CRect rcTop(rcProgress);
            rcTop.bottom = top;
            CRect rcMid(rcProgress);
            rcMid.top = top;
            rcMid.bottom = bottom;
            CRect rcBottom(rcProgress);
            rcBottom.top = bottom;
            
            if (rcTop.Height() > 0)
                LCD::Bar(rcTop, RGB565(0000b0));
            if (rcMid.Height() > 0)
                LCD::Bar(rcMid, RGB565(b0b0b0));
            if (rcBottom.Height() > 0)
                LCD::Bar(rcBottom, RGB565(0000b0));
        }
    };

void mainInit()
{
    BIOS::FAT::Init();
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    BIOS::LCD::Print(4, 0, RGB565(ffffff), RGBTRANS, "File viewer");

#ifdef __APPLE__
    OS::SetArgument((char*)"/viewtxt.elf /Users/gabrielvalky/Documents/git/LA104/system/apps/test35_view/readme.txt");
    //const char* path = "/Users/gabrielvalky/Documents/git/LA104/system/apps/test35_view/test.txt";
#else
    //OS::SetArgument("/viewtxt.elf test.txt")
    //const char* path = "test.txt";
#endif
    
    char* fileName = OS::GetArgument();
    fileName = strstr(fileName, " ");
    if (!fileName)
    {
        BIOS::DBG::Print("$\nNo file specified.\n");
        BIOS::SYS::DelayMs(1000);
        return;
    }
    
    fileName++; // skip space
    
    if (!reader.Open(fileName))
    {
        BIOS::DBG::Print("$\nUnable to open file: '");
        BIOS::DBG::Print(fileName);
        BIOS::DBG::Print("'\n'");
        BIOS::SYS::DelayMs(1000);
        return;
    }    
    
    char showName[26];
    
    if (strlen(fileName) < COUNT(showName)-1)
        strcpy(showName, fileName);
    else
    {
        strcpy(showName, "...");
        strcat(showName, fileName + strlen(fileName)-(COUNT(showName)-3-1));
    }
    BIOS::LCD::Print(BIOS::LCD::Width-4 - strlen(showName)*8, 0, RGB565(b0b0b0), RGBTRANS, showName);
    
    RedrawColored();
    DrawProgress();
}

bool mainLoop()
{    
        if (key == KEY::EKey::None)
            key = KEY::GetKey();
        
        if (key == KEY::EKey::Escape)
            return false;
        
        bool redraw = false;
        if (key == KEY::Down && reader.GetOffset(base+lines) != -1)
        {
            base++;
            redraw = true;
        } else
        if (key == KEY::Up && base > 0)
        {
            base--;
            redraw = true;
        }
        
        key = KEY::EKey::None;

        if (redraw || interrupted)
        {
            redraw = false;
            RedrawColored();
            interrupted = key != KEY::EKey::None;
            DrawProgress();
        }

    return true;
}


void mainFinish()
{
}
#endif


void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
#ifdef __APPLE__
    kill(getpid(), SIGSTOP);
#endif
    while (1);
}

