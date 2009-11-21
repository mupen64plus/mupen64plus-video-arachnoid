#ifndef CYCLIC_REDUNDANCY_CHECK_CALCULATOR_2_H_
#define CYCLIC_REDUNDANCY_CHECK_CALCULATOR_2_H_

//*****************************************************************************
//! Cyclic Redundancy Check Calculator 
//! CRC is a type of hash function which is used to produce a small, 
//! fixed-size checksum of a larger block of data.
//!
//! Often used in networks for reducing errors caused by noise.
//!
//! http://en.wikipedia.org/wiki/CRC32
//! http://en.wikipedia.org/wiki/Hash_function
//! http://en.wikipedia.org/wiki/Hash_table
//! http://www.gamedev.net/reference/articles/article1941.asp
//! http://www.codeproject.com/cpp/crc32_large.asp
//*****************************************************************************
class CRCCalculator2
{
public:

    //Constructor
    CRCCalculator2();

    //Functions for calculating crc values
    unsigned int calcCRC(unsigned int crc, void *buffer, unsigned int count);
    unsigned int calcPaletteCRC(unsigned int crc, void *buffer, unsigned int count);

private:

    //Help function used to build hash table
    unsigned int _reflect(unsigned int ref, char ch);

private:   

    static unsigned int m_crcTable[256];   //!< Hash table that associates keys with values
};

#endif