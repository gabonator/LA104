class CParser
{
    const uint8_t* mBuffer;
    
public:
    CParser(uint8_t* buffer) : mBuffer(buffer)
    {
    }
    template <typename T> CParser& operator >> (T& arg)
    {
        memcpy(&arg, mBuffer, sizeof(arg));
        mBuffer += sizeof(arg);
        return *this;
    }
};

class CFat32
{
public:
    CSd& mSd;
    uint8_t* mData;
    uint8_t* mData2;
    int mLastFatSector{0};
    int mFirstDataSector;
    int mPartitionBegin;
    
    struct mbr_t
    {
        enum PartitionType {
            Fat12 = 0x01,
            Fat16 = 0x06,
            Fat32 = 0x0b
        };
        
        struct {
            uint8_t State;
            uint8_t BeginHead;
            uint16_t BeginSector;
            uint8_t Type;
            uint8_t EndHead;
            uint16_t EndSector;
            uint32_t FirstSectorOffset;
            uint32_t NumSectors;
        } partition1;
        
        uint16_t signature;
        
        bool Load(uint8_t* buffer)
        {
            CParser(buffer+0x1fe) >> signature;
            if (signature != 0xaa55)
            {
                Platform::log_e("Mbr signature wrong %04x != aa55", signature);
                return false;
            }
            CParser(buffer+0x1be) >> partition1.State
                >> partition1.BeginHead >> partition1.BeginSector
                >> partition1.Type
                >> partition1.EndHead >> partition1.EndSector
                >> partition1.FirstSectorOffset >> partition1.NumSectors;
            if (partition1.Type != Fat32)
            {
                Platform::log_e("Mbr wrong partition type %02x != Fat32(0x0b)", partition1.Type);
                return false;
            }
            return true;
        }
    };
    
    struct bpb_t
    {
        uint8_t JumpBoot[3];
        uint8_t OEMName[8];
        uint16_t BytesPerSector;
        uint8_t SectorPerCluster;
        uint16_t ReservedSectorCount;
        uint8_t NumberOfFats;
        uint16_t RootEntCnt;
        uint16_t ToSec16;
        uint8_t Media;
        uint16_t FTASz16;
        uint16_t SecPerTrk;
        uint16_t NumHeads;
        uint32_t HiddenSector;
        uint32_t ToSec32;
        uint32_t FATSz32;
        uint16_t ExtFlags;
        uint16_t FSVer;
        uint32_t RootClus;
        uint16_t FSInfo;
        uint16_t BkBootSec;
        uint8_t FatCal;

        bool Load(uint8_t* buffer)
        {
            CParser(buffer) >> JumpBoot >> OEMName >> BytesPerSector >> SectorPerCluster
                >> ReservedSectorCount >> NumberOfFats >> RootEntCnt >> ToSec16
                >> Media >> FTASz16 >> SecPerTrk >> NumHeads >> HiddenSector >> ToSec32
                >> FATSz32 >> ExtFlags >> FSVer >> RootClus >> FSInfo >> BkBootSec >> FatCal;
            
            Platform::log_i("bytes per sector   %d", BytesPerSector);
            Platform::log_i("sector per cluster %d", SectorPerCluster);
            Platform::log_i("reserved sec count %d", ReservedSectorCount);
            Platform::log_i("number of fats     %d", NumberOfFats);
            Platform::log_i("Rootentcnt         %d", RootEntCnt);
            Platform::log_i("Root cluster       %u", RootClus);
            Platform::log_i("BPB To Sec 32      %u", ToSec32);
            Platform::log_i("size of each fat   %u", FATSz32);
            
            if (BytesPerSector != 512 || NumberOfFats != 2 || FATSz32 == 0)
                return false;
            
            return true;
        }
    };
public:
    struct direntry_t
    {
        char Name[11];
        uint8_t Attr;
        uint8_t NTRes;
        uint8_t CrtTimeTenth;
        uint16_t CrtTime;
        uint16_t CrtDate;
        uint16_t LstAccDate;
        uint16_t FstClusHI;
        uint16_t WrtTime;
        uint16_t WrtDate;
        uint16_t FstClusLO;
        uint32_t FileSize;
        
        // used during reading
        int readSectors;
        uint32_t readCluster;
        
        bool Load(uint8_t* buffer)
        {
            CParser(buffer) >> Name >> Attr >> NTRes
                >> CrtTimeTenth >> CrtTime >> CrtDate
                >> LstAccDate >> FstClusHI >> WrtTime
                >> WrtDate >> FstClusLO >> FileSize;
            return true;
        }
        bool IsDirectory()
        {
            return (Name[0] != 0xe5) && (Attr & 0x10);
        }
        bool IsFile()
        {
            return (Name[0] != 0xe5) && (Attr & 0x20);
        }
        void GetName(char* str)
        {
            // name
            int validNameChars;
            for (validNameChars=7; validNameChars>=0; validNameChars--)
                if (Name[validNameChars] != ' ' && Name[validNameChars])
                    break;
            for (int i=0; i<=validNameChars; i++)
            {
                char c = Name[i];
                if (c >= 'A' && c <= 'Z')
                    c = c - 'A' + 'a';
                str[i] = c;
            }
            validNameChars++;
            str[validNameChars++] = '.';
            str[validNameChars] = 0;
            
            // extension
            for (int i=0; i<3; i++)
            {
                char c = Name[8+i];
                if (c == 0 || c == ' ')
                    continue;
                if (c >= 'A' && c <= 'Z')
                    c = c - 'A' + 'a';
                str[validNameChars++] = c;
            }
            if (str[validNameChars-1] == '.')
                str[validNameChars-1] = 0;
            else
                str[validNameChars] = 0;
        }
    };

//private:
    bpb_t mBpb;
    
public:
    CFat32(CSd& sd, uint8_t* sector, uint8_t* sector2) : mSd(sd), mData(sector), mData2(sector2)
    {
    }

    bool init()
    {
        mSd.readSector(mData, 0);
        mbr_t mbr;
        if (!mbr.Load(mData))
            return false;
        
        mPartitionBegin = mbr.partition1.FirstSectorOffset;

        mSd.readSector(mData, mPartitionBegin);
        if (!mBpb.Load(mData))
            return false;

        uint32_t rootDirSectors = ((mBpb.RootEntCnt * 32) + (mBpb.BytesPerSector - 1)) / mBpb.BytesPerSector;
        uint32_t FATSz = mBpb.FTASz16 != 0 ? mBpb.FTASz16 : mBpb.FATSz32;
        mFirstDataSector = mBpb.ReservedSectorCount + mBpb.NumberOfFats * FATSz + rootDirSectors;
        return true;
    }

    uint32_t NextCluster(uint32_t clusterNumber)
    {
        uint32_t FatOffset = clusterNumber * 4; // fat32
        uint32_t ThisFatSecNum = mBpb.ReservedSectorCount + (FatOffset / mBpb.BytesPerSector);
        uint32_t ThisFatEntOffset = FatOffset % mBpb.BytesPerSector;
        if ((uint32_t)mLastFatSector != mPartitionBegin + ThisFatSecNum)
        {
          mLastFatSector = mPartitionBegin + ThisFatSecNum;
          mSd.readSector(mData2, mPartitionBegin + ThisFatSecNum);
        }
        
        uint32_t cluster = 0;
        cluster |= mData2[ThisFatEntOffset];
        cluster |= mData2[ThisFatEntOffset + 1] << 8;
        cluster |= mData2[ThisFatEntOffset + 2] << 16;
        cluster |= mData2[ThisFatEntOffset + 3] << 24;
        cluster &= 0x0FFFFFFF;
        return cluster;
    }

    uint32_t NextCluster2(uint32_t clusterNumber, uint32_t& fclus)
    {
        uint32_t FatOffset = clusterNumber * 4; // fat32
        uint32_t ThisFatSecNum = mBpb.ReservedSectorCount + (FatOffset / mBpb.BytesPerSector);
        uint32_t ThisFatEntOffset = FatOffset % mBpb.BytesPerSector;
        if ((uint32_t)mLastFatSector != mPartitionBegin + ThisFatSecNum)
        {
          if (fclus)
            mSd.readBytes2Stop();

          mLastFatSector = mPartitionBegin + ThisFatSecNum;
          mSd.readSector(mData2, mPartitionBegin + ThisFatSecNum);
          fclus = 0;
        }
        
        uint32_t cluster = 0;
        cluster |= mData2[ThisFatEntOffset];
        cluster |= mData2[ThisFatEntOffset + 1] << 8;
        cluster |= mData2[ThisFatEntOffset + 2] << 16;
        cluster |= mData2[ThisFatEntOffset + 3] << 24;
        cluster &= 0x0FFFFFFF;
        return cluster;
    }

    bool FindPath(char* name, direntry_t& dirEntry)
    {
        uint32_t cluster = 2;
        char* token = nullptr;
        while (*name)
        {
            if ((token = strstr(name, "/")) != nullptr)
            {
                token[0] = 0;
                if (!FindFile(cluster, name, dirEntry))
                    return false;
                cluster = (dirEntry.FstClusHI<<16) | dirEntry.FstClusLO;
                name = token+1;
            } else
                return FindFile(cluster, name, dirEntry);
       }
       return false;
    }

    bool FindFile(int cluster, const char* name, direntry_t& dirEntry)
    {
        while (true)
        {
            uint32_t sector = (cluster-2) * mBpb.SectorPerCluster + mFirstDataSector;
            for (int j = 0; j < mBpb.SectorPerCluster; j++)
            {
                mSd.readSector(mData, mPartitionBegin + sector + j);
                for (int i = 0; i < 16; i++)
                {
                    char temp[16];
                    dirEntry.Load(mData + i*32);
                    dirEntry.GetName(temp);
                    if (strcmp(temp, name) == 0)
                        return true;
                }
            }
            
            cluster = NextCluster(cluster);
            if (cluster == 0x0FFFFFFF)
                break;
            if (cluster == 0)
            {
                Platform::log_e("Wrong cluster");
                return false;
            }
        }
        return false;
    }

    int ReadFileSector(direntry_t& entry)
    {
        if ((unsigned)entry.readSectors * mBpb.BytesPerSector >= entry.FileSize)
            return -1;
        
        if ((entry.readSectors % mBpb.SectorPerCluster) == 0)
        {
            if (entry.readCluster == (uint32_t)-1)
                entry.readCluster = entry.FstClusLO | (entry.FstClusHI<<16);
            else
                entry.readCluster = NextCluster(entry.readCluster);
        }
        entry.readSectors++;
        return entry.readCluster;
    }

    bool ReadFile(direntry_t& entry)
    {
        if ((unsigned)entry.readSectors * mBpb.BytesPerSector >= entry.FileSize)
            return false;
        
        if ((entry.readSectors % mBpb.SectorPerCluster) == 0)
        {
            if (entry.readCluster == (uint32_t)-1)
                entry.readCluster = entry.FstClusLO | (entry.FstClusHI<<16);
            else
                entry.readCluster = NextCluster(entry.readCluster);
        }
        mSd.readSector(mData, mPartitionBegin + (entry.readCluster - 2)*mBpb.SectorPerCluster + mFirstDataSector + (entry.readSectors % mBpb.SectorPerCluster));
        entry.readSectors++;
        return true;
    }

    void dump()
    {
        uint32_t sector = mFirstDataSector;
        uint32_t cluster = 2;
        while (true)
        {
            for (int j = 0; j < mBpb.SectorPerCluster; j++)
            {
                mSd.readSector(mData, mPartitionBegin + sector + j);
                for (int i = 0; i < 16; i++)
                {
                    direntry_t dirEntry;
                    dirEntry.Load(mData + i*32);

                    char fileName[16];
                    dirEntry.GetName(fileName);
                    
                    if (dirEntry.IsDirectory())
                        Platform::log_i("<%s>, %x/%x  ", fileName,
                            dirEntry.FstClusLO, dirEntry.FstClusHI);
                    else if (dirEntry.IsFile())
                        Platform::log_i("%s, %d, %x/%x  ", fileName, dirEntry.FileSize,
                            dirEntry.FstClusLO, dirEntry.FstClusHI);
                }
            }
            
            cluster = NextCluster(cluster);
            if (cluster == 0x0FFFFFFF)
                break;
            if (cluster == 0)
            {
                Platform::log_e("Wrong cluster");
                return;
            }
            sector = (cluster - 2) * mBpb.SectorPerCluster + mFirstDataSector;
        }
    }

};
