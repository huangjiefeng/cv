#pragma once
#pragma warning(disable: 4290)

class BitStream
{
public:
    BitStream(const char *d, unsigned int len);
    ~BitStream(void);

    enum SetPosMode {kAbsolutePos, kRelativePos};

    //bit位置操作
    unsigned int SetPos(unsigned int pos, SetPosMode mode = kAbsolutePos);
    unsigned int GetPos() const { return pos_; }
    bool Eos() const { return pos_ >= length_; }
    void Reset();

    //数据读取操作
    char NextByte() const;
    char ReadByte();
    unsigned int NextBytesToUInt32(unsigned int n) const throw(unsigned int);
    unsigned int ReadBytesToUInt32(unsigned int n) throw(unsigned int);
    bool ReadNextBytes(void *buf, unsigned int n);

    bool NextBit() const;
    bool ReadBit();
    /*unsigned int NextBitsToUInt32(unsigned int n) const throw(unsigned int);
    unsigned int ReadBitsToUInt32(unsigned int n) throw(unsigned int);*/

    unsigned int ReadGolombCode() throw(unsigned int);

    //特定位置的指针获取
    const char * MoveToNextNaluStart(); //移动当前位置到下一个NALU起始，若找不到则返回EndPtr
    const char * MoveToNaluEnd(); //移动当前位置到NALU结尾的后一个字符
    const char * BeginPtr() const; //返回数据流的起始字节指针
    const char * EndPtr() const; //返回数据流的最后一个字节的下一个字节指针
    const char * CurPtr() const; //返回数据流的当前字节指针，即bit位所在字节

private:
    inline unsigned int NextBytePos() const;
    BitStream &operator=(const BitStream &) {};

private:
    const char *data_;
    const unsigned int length_; //in bits count
    unsigned int pos_; //下一个要读的bit位置
};

