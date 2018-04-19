/*
 * common.h
 *
 *  Created on: Dec 13, 2017
 *      Author: ranjeet
 */

#ifndef ENERGIMINER_COMMON_H_
#define ENERGIMINER_COMMON_H_

#include "portable_endian.h"

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <mutex>
#include <array>

enum class MinerExecutionMode : unsigned
{
    kCPU   = 0x1,
    kCL    = 0x2,
    kMixed = 0x3,
    kCUDA  = 0x4
};

enum class EnumMinerEngine : unsigned
{
    kCPU    = 0x0,
    kCL     = 0x1,
    kTest   = 0x2
};

uint16_t inline ReadLE16(const unsigned char* ptr)
{
    return le16toh(*((uint16_t*)ptr));
}

uint32_t inline ReadLE32(const unsigned char* ptr)
{
    return le32toh(*((uint32_t*)ptr));
}

uint64_t inline ReadLE64(const unsigned char* ptr)
{
    return le64toh(*((uint64_t*)ptr));
}

void inline WriteLE16(unsigned char* ptr, uint16_t x)
{
    *((uint16_t*)ptr) = htole16(x);
}

void inline WriteLE32(unsigned char* ptr, uint32_t x)
{
    *((uint32_t*)ptr) = htole32(x);
}

void inline WriteLE64(unsigned char* ptr, uint64_t x)
{
    *((uint64_t*)ptr) = htole64(x);
}

uint32_t inline ReadBE32(const unsigned char* ptr)
{
    return be32toh(*((uint32_t*)ptr));
}

uint64_t inline ReadBE64(const unsigned char* ptr)
{
    return be64toh(*((uint64_t*)ptr));
}

void inline WriteBE32(unsigned char* ptr, uint32_t x)
{
    *((uint32_t*)ptr) = htobe32(x);
}

void inline WriteBE64(unsigned char* ptr, uint64_t x)
{
    *((uint64_t*)ptr) = htobe64(x);
}

namespace energi {

inline std::string GetHex(const uint8_t* data, unsigned int size)
{
    std::string psz(size * 2 + 1, '\0');
    for (unsigned int i = 0; i < size; i++)
        sprintf(const_cast<char*>(psz.data()) + i * 2, "%02x", data[size - i - 1]);
    return std::string(const_cast<char*>(psz.data()), const_cast<char*>(psz.data()) + size * 2);
}


#if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#define WANT_BUILTIN_BSWAP
#else
#define bswap_32(x) ((((x) << 24) & 0xff000000u) | (((x) << 8) & 0x00ff0000u) \
        | (((x) >> 8) & 0x0000ff00u) | (((x) >> 24) & 0x000000ffu))
#endif

static inline uint32_t swab32(uint32_t v)
{
#ifdef WANT_BUILTIN_BSWAP
    return __builtin_bswap32(v);
#else
    return bswap_32(v);
#endif
}

#ifdef HAVE_SYS_ENDIAN_H
#include <sys/endian.h>
#endif

}


#endif /* ENERGIMINER_COMMON_H_ */
