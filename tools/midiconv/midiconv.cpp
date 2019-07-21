//
//  main.cpp
//  midiconv
//
//  Created by Gabriel Valky on 11/9/18.
//  Copyright © 2018 Gabriel Valky. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <iomanip>

using namespace std;
template <typename T>
void operator >> (ifstream &ifs, T& obj)
{
    ifs.read((char*)&obj, sizeof(T));
}

template <typename T>
void operator >> (ifstream &ifs, vector<T>& obj)
{
    ifs.read((char*)obj.data(), obj.size());
}

void operator << (ofstream &ofs, vector<uint8_t>& obj)
{
    ofs.write((char*)&ofs, obj.size());
}

template <typename T>
void fixEndian(T& v);

template <>
void fixEndian<uint16_t>(uint16_t& v)
{
    v = (v << 8) | (v >> 8);
}

template <>
void fixEndian<uint32_t>(uint32_t& v)
{
    v = (v<<24) | ((v<<8) & 0x00ff0000) | ((v>>8) & 0x0000ff00) | (v>>24);
}

void dumpEvent(vector<uint8_t>::iterator& it)
{
    int length = *it++;
    for (int i=0; i<length; i++)
        cout << " " << (int)*it++;
    
    cout << "\n";
}

int getDeltaTime(vector<uint8_t>::iterator& i)
{
    int time = 0;
    int quantity;
    while ((quantity = *i++) & 0x80)
    {
        time |= quantity & 0x7f;
        time <<= 7;
    }
    time |= quantity;
    return time;
}

void putDeltaTime(vector<uint8_t>& vct, int x)
{
    assert(x >= 0);
    vector<uint8_t> quantity;
    //quantity.push_back(0);
    quantity.push_back(x & 0x7f);
    
    while (x>0)
    {
        x >>= 7;
        quantity.push_back((x & 0x7f) | 0x80);
    }
    
    std::reverse(quantity.begin(), quantity.end());
    vct.insert(vct.end(), quantity.begin(), quantity.end());
    //vct.push_back(0);
}


#if 1
//constexpr char* input = "/Users/gabrielvalky/Downloads/LaIslaBonita.mid";
//constexpr char* output = "/Users/gabrielvalky/Downloads/laisla.txt";
//constexpr char* outputmidi = "/Users/gabrielvalky/Downloads/laisla.mid";
#define input argv[1]
#define outputmidi argv[2]
#else
constexpr char* input = "/Users/gabrielvalky/Downloads/laisla.mid";
constexpr char* output = "/Users/gabrielvalky/Downloads/laisla3.txt";
constexpr char* outputmidi = "/Users/gabrielvalky/Downloads/laisla3.mid";
#endif

int main(int argc, const char * argv[])
{
    ifstream ifs(input, ios::binary);
    
    struct MThd
    {
        char chunk[4];
        uint32_t length;
        uint16_t format;
        uint16_t tracks;
        uint16_t division;
        
        bool isValid()
        {
            return memcmp(chunk, "MThd", 4) == 0 && length == 6 && (format == 1 || format == 0);
        }
        void fixEndian()
        {
            ::fixEndian(length);
            ::fixEndian(format);
            ::fixEndian(tracks);
            ::fixEndian(division);
        }
    } __attribute__((packed));
    
    struct MTrk
    {
        char chunk[4];
        uint32_t length;
        void fixEndian()
        {
            ::fixEndian(length);
        }
        bool isValid()
        {
            return memcmp(chunk, "MTrk", 4) == 0 && length < 100000;
        }
    } __attribute__((packed));
    
    MThd header;
    ifs >> header;
    header.fixEndian();
    assert(header.isValid());
    
    vector<uint8_t> metaTempo;
    vector<uint8_t> metaTimeSignature;
    map<int, vector<vector<uint8_t> >, std::less<int> > midiEvents;
    
    for (int i=0; i<header.tracks; i++)
    {
        MTrk track;
        ifs >> track;
        track.fixEndian();
        assert(track.isValid());
        
        vector<uint8_t> trackData;
        trackData.resize(track.length);
        ifs >> trackData;
        
        cout << "TRACK " << dec << i << " ------\n";
        int tick = 0;
        int lastCmd = 0;
        for (auto i = trackData.begin(); i != trackData.end(); )
        {
            int time = getDeltaTime(i);
            tick += time;
            if (time > 0)
                cout << dec << "wait " << time << "(0x" << hex << time << ")\n";
            assert(i < trackData.end());
            uint8_t event = *i++;
            if (event & 0x80)
            {
                lastCmd = event;
            } else
            {
                event = lastCmd;
                i--;
            }
            switch (event)
            {
                case 0xff: // Meta event
                {
                    int meta = *i++;
                    cout << "Meta event " << hex << (int)event << " " << (int)meta << ": ";
                    if (meta == 0x51) // tempo
                    {
                        if (*i++ != 3)
                            assert(0);
                        
                        uint8_t a = *i++;
                        uint8_t b = *i++;
                        uint8_t c = *i++;
                        metaTempo = {0xff, 0x51, 3, a, b, c};
                        
                        int tempo = a;
                        tempo >>= 8;
                        tempo |= b;
                        tempo >>= 8;
                        tempo |= c;
                        cout << "tempo: " << dec << tempo << "us per quarter note\n";
                        // 24ths of a microsecond per MIDI clock
                        //tempo / 24
                    } else
                    if (meta == 0x58) // time signature
                    {
                        uint8_t len = *i++;
                        uint8_t nn = *i++;
                        uint8_t dd = *i++;
                        uint8_t cc = *i++;
                        uint8_t bb = *i++;
                        assert(len == 4);
                        metaTimeSignature = {0xff, 0x58, len, nn, dd, cc, bb};
                        cout << "timesignature: " << dec << (int) nn << "/" << (int) dd << " clocks: " << (int)cc << " 32s: " << (int)bb << "\n";
                    } else
                    {
                        dumpEvent(i);
                    }
                    break;
                }
                case 0xf0: // Sysex
                case 0xf7: // Sysex escape
                {
                    cout << "Sysex" << hex << (int)event << ": ";
                    dumpEvent(i);
                    break;
                }
                case 0x80 ... 0x8f: // release
                {
                    uint8_t key = *i++;
                    uint8_t velocity = *i++;
                    cout << "Release: " << hex << (int)event << " key " << (int)key << " velocity " << (int)velocity << "\n";
                    midiEvents[tick].push_back({event, key, velocity});
                    break;
                }
                case 0x90 ... 0x9f: // press
                {
                    uint8_t key = *i++;
                    uint8_t velocity = *i++;
                    cout << "Press: " << hex << (int)event << " key " << (int)key << " velocity " << (int)velocity << "\n";
                    midiEvents[tick].push_back({event, key, velocity});
                    break;
                }
                case 0xa0 ... 0xaf: // aftertouch
                {
                    uint8_t key = *i++;
                    uint8_t velocity = *i++;
                    cout << "Aftertouch: " << hex << (int)event << " key " << (int)key << " velocity " << (int)velocity << "\n";
                    break;
                }
                case 0xb0 ... 0xbf: // controller
                {
                    uint8_t cc = *i++;
                    uint8_t value = *i++;
                    cout << "CC: " << hex << (int)event << " ctrlnum " << (int)cc << " value " << (int)value << "\n";
                    midiEvents[tick].push_back({event, cc, value});
                    break;
                }
                case 0xc0 ... 0xcf: // patch change
                {
                    uint8_t instrument = *i++;
                    cout << "Instrument: " << hex << (int)event << " value: " << dec << (int)instrument << ", " <<  "\n";
                    midiEvents[tick].push_back({event, instrument});
                    break;
                }
                case 0xe0 ... 0xef: // pitchbend
                {
                    uint8_t lsb = *i++;
                    uint8_t msb = *i++;
                    cout << "Pitch: " << hex << (int)event << " " << (int)msb << " " << (int)lsb << "\n";
                    midiEvents[tick].push_back({event, lsb, msb});
                    break;
                }
                    
                default:
                {
                    cout << "Unhandled " << hex << (int)event << " at " << ((int)ifs.tellg() - (int)trackData.size() + std::distance(trackData.begin(), i)) << "! \n";
                }
            }
        }
    }
    
    int lastTick = 0;
/*
    ofstream ofs(output);
    
    ofs << "baud 31250\n";
    for (const auto& eventSet : midiEvents)
    {
        if (lastTick != eventSet.first)
        {
            int wait = eventSet.first - lastTick;
            //if (wait > 1)
            {
                lastTick = eventSet.first;
                ofs << "wait " << setw(0) << dec << wait << "\n";
            }
        }
        for (const auto& event : eventSet.second)
        {
            for (const auto& data : event)
                ofs << setfill('0') << setw(2) << hex << (int)data << " ";
            ofs << "\n";
        }
    }
    ofs << "\n\n\n";
    ofs.close();
*/    
    ofstream ofmidi(outputmidi, ios::binary);
    lastTick = 0;
    vector<uint8_t> midiBuffer;
    
    if (!metaTempo.empty())
    {
        putDeltaTime(midiBuffer, 0);
        midiBuffer.insert(midiBuffer.end(), metaTempo.begin(), metaTempo.end());
    }
    
    if (!metaTimeSignature.empty())
    {
        putDeltaTime(midiBuffer, 0);
        midiBuffer.insert(midiBuffer.end(), metaTimeSignature.begin(), metaTimeSignature.end());
    }

    for (const auto& eventSet : midiEvents)
    {
        for (const auto& event : eventSet.second)
        {
            int wait = eventSet.first - lastTick;
            lastTick = eventSet.first;
            putDeltaTime(midiBuffer, wait);

            midiBuffer.insert(midiBuffer.end(), event.begin(), event.end());
        }
    }

    /*
    putDeltaTime(midiBuffer, 0);
    midiBuffer.push_back(0xff); // end of track
    midiBuffer.push_back(0x2f);
*/

    MThd wrHead;
    memcpy(wrHead.chunk, "MThd", 4);
    wrHead.length = 6;
    wrHead.format = 0;
    wrHead.tracks = 1;
    wrHead.division = 120;
    
    wrHead.fixEndian();
    ofmidi.write((char*)&wrHead, sizeof(wrHead));
    wrHead.fixEndian();

    
    MTrk wrTrack;
    memcpy(wrTrack.chunk, "MTrk", 4);
    wrTrack.length = (int)midiBuffer.size();
    
    wrTrack.fixEndian();
    ofmidi.write((char*)&wrTrack, sizeof(wrTrack));
    wrTrack.fixEndian();

    ofmidi.write((char*)midiBuffer.data(), midiBuffer.size());
    ofmidi.close();
    return 0;
}
