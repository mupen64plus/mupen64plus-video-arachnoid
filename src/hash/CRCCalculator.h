#ifndef CYCLIC_REDUNDANCY_CHECK_CALCULATOR_H_
#define CYCLIC_REDUNDANCY_CHECK_CALCULATOR_H_

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
class CRCCalculator
{
public:

    //Constructor
    CRCCalculator();

    //Functions for calculating crc values
    unsigned long calcCRC(unsigned long crc, void *buffer, unsigned long count);
    unsigned long calcPaletteCRC(unsigned long crc, void *buffer, unsigned long count);

private:

    //Help function used to build hash table
    unsigned long _reflect(unsigned long ref, char ch);

private:   

    static unsigned long m_crcTable[256];   //!< Hash table that associates keys with values
};

#endif