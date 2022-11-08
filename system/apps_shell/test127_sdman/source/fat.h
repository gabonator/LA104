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

class CFat
{
public:
    CSd& mSd;
    uint8_t* mData;

    enum PartitionType {
        None = 0x00,
        Fat12 = 0x01,
        Fat16 = 0x06,  // fat16 b
        Fat32 = 0x0b,  // fat32 chs
        Fat32Lba = 0x0c,
        Linux = 0x83
    };
    
    struct mbr_t
    {                        
        bool Load(uint8_t* buffer)
        {
            uint16_t signature;

            CParser(buffer+0x1fe) >> signature;
            if (signature != 0xaa55)
            {
                Platform::log_e("Bad MBR sign: %04x", signature);
                return false;
            }
/*
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
*/
            return true;
        }
    };

    struct partition_t {
        uint8_t State;
        uint8_t BeginHead;
        uint16_t BeginSector;
        uint8_t Type;
        uint8_t EndHead;
        uint16_t EndSector;
        uint32_t FirstSectorOffset;
        uint32_t NumSectors;

        bool Load(uint8_t* buffer)
        {
            CParser(buffer) >> State
                >> BeginHead >> BeginSector
                >> Type
                >> EndHead >> EndSector
                >> FirstSectorOffset >> NumSectors;
            return true;
        }

        const char* GetType()
        {
            switch (Type)
            {
                case PartitionType::None:
                    return "none";
                case PartitionType::Fat12:
                    return "fat12";
                case PartitionType::Fat16:
                    return "fat16";
                case PartitionType::Fat32:
                    return "fat32";
                case PartitionType::Fat32Lba:
                    return "fat32";
                case PartitionType::Linux:
                    return "linux";
                default:
                    return "?";
            }
        }
        void FormatSize(char* str)
        {
            uint64_t size = NumSectors * 512;
            if (size >= 1000*1024*1024)
            {
                int sizeGib10 = size/1000/1000/100;
                sprintf(str, "%d.%d GB", sizeGib10/10, sizeGib10%10);
            } else if (size > 1024*1024)
            {
                int sizeMB10 = size*10/1024/1024;
                sprintf(str, "%d.%d MB", sizeMB10/10, sizeMB10%10);
            } else
            {
                int sizeKB10 = size/10*1024;
                sprintf(str, "%d.%d MB", sizeKB10/10, sizeKB10%10);
            }
        }
    };

public:
    CFat(CSd& sd, uint8_t* sector) : mSd(sd), mData(sector)
    {
    }
    bool CheckMbr()
    {
        mSd.readSector(mData, 0);
        mbr_t mbr;
        return mbr.Load(mData);
    }
    void GetPartition(int n, partition_t& partition)
    {
        partition.Load(mData + 0x1be + n*16);
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
        uint16_t FATSz16;
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
                >> Media >> FATSz16 >> SecPerTrk >> NumHeads >> HiddenSector >> ToSec32
                >> FATSz32 >> ExtFlags >> FSVer >> RootClus >> FSInfo >> BkBootSec >> FatCal;
            
            Platform::log_i("Bytes per sector   %d", BytesPerSector);
            Platform::log_i("Sector per cluster %d", SectorPerCluster);
            Platform::log_i("Reserved sec count %d", ReservedSectorCount);
            Platform::log_i("Number of fats     %d", NumberOfFats);
            Platform::log_i("Rootentcnt         %d", RootEntCnt);
            if (ToSec16)
              Platform::log_i("BPB To Sec 16  %u", ToSec16);
            else {
              Platform::log_i("Root cluster       %u", RootClus);
              Platform::log_i("BPB To Sec 32  %u", ToSec32);
            }
            Platform::log_i("Size of each fat   %u", FATSz16 ? FATSz16 : FATSz32);
            
            if (BytesPerSector != 512 || NumberOfFats != 2 || (FATSz32 == 0 && FATSz16 == 0))
                return false;
            
            return true;
        }
    };

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
            return (Name[0] != (char)0xe5) && (Attr & 0x10);
        }
        bool IsFile()
        {
            return (Name[0] != (char)0xe5) && (Attr & 0x20) && !IsVolumeLabel();
        }
        bool IsVolumeLabel()
        {
            return Attr == 0x28;
        }
        bool IsLfn()
        {
            return !(Name[0] & 0x80) && (Attr == 0x0f) && (NTRes == 0);
        }
        bool IsLfnLast()
        {
            return IsLfn() && (Name[0] & 0x40);
        }
        void GetName(char* str)
        {
            if (IsLfn())
            {
                str[0] = Name[1];
                str[1] = Name[3];
                str[2] = Name[5];
                str[3] = Name[7];
                str[4] = Name[9];
                str[5] = CrtTime & 0xff;
                str[6] = CrtDate & 0xff;
                str[7] = LstAccDate & 0xff;
                str[8] = FstClusHI & 0xff;
                str[9] = WrtTime & 0xff; //
                str[10] = WrtDate & 0xff; //
                str[11] = FileSize & 0xff;
                str[12] = (FileSize >> 16) & 0xff;
                str[13] = 0;
                return;
            }

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

class CFat32
{
public:
    CSd& mSd;
    uint8_t* mData;
    int mFirstDataSector;
    int mPartitionBegin;
    int mPartitionLength;
    bool mFat32;
    bpb_t mBpb;
    int mCluster{2};

    CFat32(CSd& sd, uint8_t* sector) : mSd(sd), mData(sector)
    {
    }

    void SetRange(int begin, int length)
    {
        mPartitionBegin = begin;
        mPartitionLength = length;
    }

    bool Check()
    {
        mSd.readSector(mData, mPartitionBegin);
        if (!mBpb.Load(mData))
            return false;
        //TODO
        uint32_t rootDirSectors = ((mBpb.RootEntCnt * 32) + (mBpb.BytesPerSector - 1)) / mBpb.BytesPerSector;
        mFat32 = mBpb.FATSz16 == 0;
        uint32_t FATSz = mFat32 ? mBpb.FATSz32 : mBpb.FATSz16;
        mFirstDataSector = mBpb.ReservedSectorCount + mBpb.NumberOfFats * FATSz + rootDirSectors;
        return true;
    }
    
    uint32_t NextCluster(uint32_t clusterNumber)
    {
        uint32_t FatOffset = mFat32 ? clusterNumber * 4 : clusterNumber * 2;
        uint32_t ThisFatSecNum = mBpb.ReservedSectorCount + (FatOffset / mBpb.BytesPerSector);
        uint32_t ThisFatEntOffset = FatOffset % mBpb.BytesPerSector;

        mSd.readSector(mData, mPartitionBegin + ThisFatSecNum);
        
        uint32_t cluster = 0;
        cluster |= mData[ThisFatEntOffset];
        cluster |= mData[ThisFatEntOffset + 1] << 8;
        cluster |= mData[ThisFatEntOffset + 2] << 16;
        cluster |= mData[ThisFatEntOffset + 3] << 24;
        cluster &= 0x0FFFFFFF;
        return cluster;
    }

    void ChangeDir(uint32_t sector)
    {
////        if (sector == -1)
////        {
//            uint32_t rootDirSectors = ((mBpb.RootEntCnt * 32) + (mBpb.BytesPerSector - 1)) / mBpb.BytesPerSector;
//            uint32_t FATSz = mFat32 ? mBpb.FATSz32 : mBpb.FATSz16;
//            mFirstDataSector = mBpb.ReservedSectorCount + mBpb.NumberOfFats * FATSz + rootDirSectors;
////        }
//        mFirstDataSector += sector * mBpb.SectorPerCluster;
        mCluster = sector;
    }
    //(1606+63)*512 = 0xd0a00
    //0xd1600
    void ListFiles(bool (*process)(direntry_t&))
    {
        uint32_t cluster = mCluster;
        while (true)
        {
            uint32_t sector = (cluster-2) * mBpb.SectorPerCluster + mFirstDataSector;
            for (int j = 0; j < mBpb.SectorPerCluster; j++)
            {
                mSd.readSector(mData, mPartitionBegin + sector + j);
                for (int i = 0; i < 16; i++)
                {
                    direntry_t dirEntry;
                    dirEntry.Load(mData + i*32);
                    if (!process(dirEntry))
                        return;
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
        }
    }
    /*
    void Read(int cluster, int offset, int size, uint8_t* buffer)
    {
//        if (size == 157)
//        {
//            int f =9;
//        }
        while (offset > 512 * mBpb.SectorPerCluster)
        {
            cluster = NextCluster(cluster);
            offset -= 512 * mBpb.SectorPerCluster;
        }
        int sector = (cluster-2) * mBpb.SectorPerCluster + mFirstDataSector;

        int offsetBlk = offset / 512;
        offset &= 511;
        while (size > 0)
        {
            mSd.readSector(mData, mPartitionBegin + sector + offsetBlk);

            int toRead = min(511-offset, size);
            memcpy(buffer, mData+offset, toRead);
            offset = 0;
            buffer += toRead;
            size -= toRead;
            if (++offsetBlk == mBpb.SectorPerCluster)
            {
                cluster = NextCluster(cluster);
                sector = (cluster-2) * mBpb.SectorPerCluster + mFirstDataSector;
                offsetBlk = 0;
            }
        }
    }*/
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



};
