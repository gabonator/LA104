//#include "terminal.cpp"
#include "../../system/os_host/source/bios/Bios.h"
#include "../../system/os_host/source/gui/Gui.cpp"
//#include "../../system/os_host/source/gui/Gui.h"
#include "../../system/os_host/source/framework/Serialize.h"
#include "reader.h"

using namespace BIOS;

#include "../../system/apps_shell/test28_shell/source/bitmap.h"
#include "../../system/apps_shell/test28_shell/source/json.h"
//#include "../../system/apps/test23_screenshot/screenshot.h"
#include "save.h"

char basepath[128] = ""; //"/Users/gabrielvalky/Documents/git/LA104/system/build/image_la104/";
char rootpath[128];

int mScroll = 0;
int mCursor = 0;
bool HasFocus() { return true; }
CRect m_rcClient(0, 14, BIOS::LCD::Width, BIOS::LCD::Height);

class CDirInfo
{
    char mFileName[16*2];
    char mShortName[32*2];
    char mIconName[16*2];
    char mExecName[64*2];
    static char* mRoot;
    int mOrder;
    bool isFolder{false};
    
public:
    CDirInfo()
    {
    }
    
    CDirInfo(char* path, char* name)
    {
        char temp[256];
        char* fullPath = temp;
        char* buffer = temp;

        strcpy(mShortName, "");
        strcpy(mIconName, "");
        strcpy(mExecName, "");
        mOrder = 0;

        mRoot = path;
        strcpy(mFileName, name);
        
        bool hasIndex = false;
        CBufferedReader reader;
        {
            if (strstr(name, ".lnk") == nullptr)
            {
                strcpy(fullPath, path);
                strcat(fullPath, "/");
                strcat(fullPath, name);
                strcat(fullPath, "/index.lnk");
                hasIndex = reader.Open(fullPath);
                isFolder = hasIndex;
            } else
            {
                strcpy(fullPath, path);
                strcat(fullPath, "/");
                strcat(fullPath, name);
                hasIndex = reader.Open(fullPath);
                isFolder = false;
            }
        }
        if (hasIndex)
        {
            reader >> buffer;
            CJson json(buffer);
            if (json.Verify())
            {
                json["icon"].ToString(mIconName, 16);
                if (strlen(mIconName) == 0)
                {
                    strcpy(mIconName, name);
                    strcat(mIconName, ".bmp");
                }
                json["execute"].ToString(mExecName, 64);
                CJson jsonDesc = json["description"];
                if (jsonDesc)
                {
                    CJson jsonDescShort = jsonDesc["long"];
                    if (jsonDescShort)
                        jsonDescShort.ToString(mShortName, 32);
                    else
                        jsonDesc.ToString(mShortName, 32);
                } else
                {
                    strcpy(mShortName, name);
                }
                mOrder = json["order"].GetNumber();
            } else
            {
                _ASSERT(0);
                strcpy(mShortName, name);
            }
        } else
        {
            _ASSERT(0);
            strcpy(mShortName, name);
        }

        if (strlen(mIconName) == 0)
        {
            CJson json(buffer);
            if (json.Verify())
            {
                json["icon"].ToString(mIconName, 16);
            }
            
            strcpy(mIconName, name);
            strcat(mIconName, ".bmp");
        }
    }
    
    const char* GetRoot() const
    {
        return mRoot;
    }
    const char* GetShortName() const
    {
        return mShortName;
    }
    const char* GetFileName() const
    {
        return mFileName;
    }
    const char* GetIconName() const
    {
        return mIconName;
    }
    const char* GetExecutable() const
    {
        return mExecName;
    }
    int GetOrder() const
    {
        return mOrder;
    }
    bool IsFolder() const
    {
        return isFolder;
    }
};

char* CDirInfo::mRoot = {0};


    CDirInfo mItemsStorage[64];
    CArray<CDirInfo> mItems;
    char mCurrentDir[128];
    void SortItems();
    typedef char TFolderName[32];
    TFolderName mFolderStackStorage[8];
    CArray<TFolderName> mFolderStack;

    void LoadItems()
    {
        mItems.RemoveAll();
        
        //strcpy(mCurrentDir, "");
        strcpy(mCurrentDir, rootpath);

        for (int i=0; i<mFolderStack.GetSize(); i++)
        {
            strcat(mCurrentDir, "/");
            strcat(mCurrentDir, mFolderStack[i]);
        }
//printf("scan: '%s' %d\n", mCurrentDir, mFolderStack.GetSize());

        FAT::EResult eOpen = FAT::OpenDir(mCurrentDir);
        if (eOpen == FAT::EResult::EOk)
        {
            FAT::TFindFile file;
            while (FAT::FindNext(&file) == FAT::EResult::EOk)
            {
//printf("Found: '%s'\n", file.strName);
                if (file.nAtrib & FAT::EAttribute::EHidden)
                    continue;
                
                if (file.strName[0] == '.' || !(file.nAtrib & FAT::EAttribute::EDirectory))
                    continue;
                
                mItems.Add(CDirInfo(mCurrentDir, file.strName));
            }
        }

        eOpen = FAT::OpenDir(mCurrentDir);
        if (eOpen == FAT::EResult::EOk)
        {
            FAT::TFindFile file;
            while (FAT::FindNext(&file) == FAT::EResult::EOk)
            {
                if (file.nAtrib & FAT::EAttribute::EHidden)
                    continue;
                if (file.strName[0] == '.' || (file.nAtrib & FAT::EAttribute::EDirectory))
                    continue;
                if (strstr(file.strName, ".lnk") == nullptr || strstr(file.strName, "INDEX") != nullptr)
                    continue;

                mItems.Add(CDirInfo(mCurrentDir, file.strName));
            }
        }
        
        SortItems();
    }
    
    void SortItems()
    {
        mItems.Sort([](CDirInfo& a, CDirInfo& b) -> int {
            int order = a.GetOrder() - b.GetOrder();
            if (order != 0)
                return order;
            return strcmp(b.GetShortName(), a.GetShortName());
        });
    }
void DrawIcon(int bx, int by, CDirInfo& info, bool on, int row);

    void DrawIcons()
    {
        GUI::Background(m_rcClient, RGB565(b0b0b0), RGB565(808080));
        int index = mScroll;
        for (int y=0; y<2; y++)
            for (int x=0; x<3; x++)
            {
                if (index < mItems.GetSize())
                {
                    CDirInfo& item = mItems[index];
                    DrawIcon(15+98*x, 30+104*y, item, false, y);
                    DrawIcon(15+98*x, 30+104*y, item, true, y);
                }
                index++;
            }
    }

void FixPath(char* fixedPath)
{
    char *parent = nullptr;
    while ((parent = strstr(fixedPath, "//")) != nullptr)
    {
        int chars = strlen(parent)+1;
        for (int i=0; i<chars; i++)
            parent[i] = parent[i+1];
    }
    while ((parent = strstr(fixedPath, "/../")) != nullptr)
    {
       char* end = parent+3;
       parent--;
       while (*parent != '/' && parent > fixedPath)
           parent--;

       // remove parent..end
       int charsToRemove = end-parent;
       int chars = strlen(end)+1;
       for (int i=0; i<chars; i++)
           parent[i] = parent[i+charsToRemove];
    }
}


void DrawIcon(int bx, int by, CDirInfo& info, bool on, int row)
//void DrawIcon(int bx, int by, bool on, char* imgSrc)
{
  CRect rcIcon(bx-4, by-4, bx+64+4, by+64+4);
        char imgSrc[128];
        if (strstr(info.GetFileName(), ".lnk"))
        {
            strcpy(imgSrc, info.GetRoot());
            strcat(imgSrc, "/");
            strcat(imgSrc, info.GetIconName());
        } else
        {
            strcpy(imgSrc, info.GetRoot());
            strcat(imgSrc, "/");
            strcat(imgSrc, info.GetFileName());
            strcat(imgSrc, "/");
            strcat(imgSrc, info.GetIconName());
        }

    FixPath(imgSrc);

  if (on)
  {
      int shift[8] = {5, 3, 2, 1, 1, 0, 0, 0};
      for (int y=rcIcon.top; y <rcIcon.bottom; y++)
      {
          CRect sub(rcIcon);
          sub.top = y;
          sub.bottom = y+1;
          int dis = min(y-rcIcon.top, rcIcon.bottom-1-y);
          if (dis<8)
          {
              sub.left += shift[dis];
              sub.right -= shift[dis];
          }
          GUI::Background(sub, RGB565RGB(39, 101, 217), RGB565RGB(39, 101, 217));
      }
  } else
  {
      GUI::Background(rcIcon, RGB565(b0b0b0), RGB565(808080));
  }
  if (!DrawImage(imgSrc, bx, by))
  {
    printf("Missing: %s\n", imgSrc + strlen(basepath));
    return;
//      _ASSERT(0);
  }
  printf("Origin: %s\n", imgSrc + strlen(basepath));

  char* suffix = imgSrc + strlen(imgSrc) - 4; // .BMP
  _ASSERT(suffix[0] == '.');
  strcpy(suffix, row == 0 ? (on ? ".tm1" : ".tm0") : (on ? ".tm3" : ".tm2"));
  SaveImage(imgSrc, rcIcon);
  printf("Target: %s\n", imgSrc + strlen(basepath));
}

void WriteImage();

void ProcessFolder(char* path)
{
    strcpy(rootpath, basepath);
    strcat(rootpath, path);
    if (rootpath[0])//gabo
      strcat(rootpath, "/");
    
    printf("Folder: %s\n", path);
    //strcpy(rootpath, path);

    mFolderStack.Init(mFolderStackStorage, COUNT(mFolderStackStorage));
    mItems.Init(mItemsStorage, COUNT(mItemsStorage));
    LoadItems();
//    for (int i=0; i<mItems.GetSize(); i++)
//        printf("  %s\n", mItems[i].GetIconName());

    for (mScroll=0; mScroll<mItems.GetSize(); mScroll+=6)
    {
        DrawIcons();
        char shot[128];
        sprintf(shot, "screenshots/%s%d.bmp", path, mScroll/6);
        for (int i=12; i<strlen(shot); i++)
            if (shot[i] == '/')
                shot[i] = '_';
        
        SaveScreenshot16(shot);        
    }

    
    typedef char TFolderName[32];
    TFolderName mSubfoldersData[32];
    CArray<TFolderName> mSubfolders;
    mSubfolders.Init(mSubfoldersData, COUNT(mSubfoldersData));

    FAT::EResult eOpen = FAT::OpenDir(mCurrentDir);
    _ASSERT(eOpen == FAT::EResult::EOk)
    FAT::TFindFile file;
    while (FAT::FindNext(&file) == FAT::EResult::EOk)
    {
        if (file.nAtrib & FAT::EAttribute::EHidden)
            continue;
        
        if (file.strName[0] == '.' || !(file.nAtrib & FAT::EAttribute::EDirectory))
            continue;
        
        mSubfolders.SetSize(mSubfolders.GetSize()+1);
        strcpy(mSubfolders[mSubfolders.GetSize()-1], file.strName);
    }
    
    
    for (int i=0; i<mSubfolders.GetSize(); i++)
    {
        char tempPath[128];
        strcpy(tempPath, path);
        strcat(tempPath, mSubfolders[i]);
        strcat(tempPath, "/");
        ProcessFolder(tempPath);
    }
}

int main(int argc, char *argv[])
{
  // Needs absolute path
  strcpy(basepath, argv[1]);
  FixPath(basepath);
  ProcessFolder((char*)"");
  return 0;
}
