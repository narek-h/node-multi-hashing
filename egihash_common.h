#pragma once

#include "portable_endian.h"
#include <string>

inline std::string GetHex(const uint8_t* data, unsigned int size)
{
	std::string psz(size * 2 + 1, '\0');
	for (unsigned int i = 0; i < size; i++)
		sprintf(const_cast<char*>(psz.data()) + i * 2, "%02x", data[size - i - 1]);
	return std::string(const_cast<char*>(psz.data()), const_cast<char*>(psz.data()) + size * 2);
}
#pragma pack(push, 1)
struct CBlockHeaderTruncatedLE
{
	int32_t nVersion;
	char hashPrevBlock[65];
	char hashMerkleRoot[65];
	uint32_t nTime;
	uint32_t nBits;
	uint32_t nHeight;

	CBlockHeaderTruncatedLE(const void* data)
		: nVersion(htole32(*reinterpret_cast<const int32_t*>(reinterpret_cast<const uint8_t*>(data) + 0)))
		  , hashPrevBlock{0}
	, hashMerkleRoot{0}
	, nTime(htole32(*reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(data) + 4 + 32 + 32 )))
		, nBits(htole32(*reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(data) + 4 + 32 + 32 + 4 )))
		, nHeight(htole32(*reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(data) + 4 + 32 + 32 + 4 + 4 )))
		{
			auto inputHashPrevBlock = reinterpret_cast<const uint8_t*>(data) + 4;
			auto prevHash = GetHex(inputHashPrevBlock, 32);
			std::memcpy(hashPrevBlock, prevHash.c_str(), (std::min)(prevHash.size(), sizeof(hashPrevBlock)));

			auto inputMerkleHashPrevBlock = reinterpret_cast<const uint8_t*>(data) + 36;
			auto merkleRoot = GetHex(inputMerkleHashPrevBlock, 32);
			std::memcpy(hashMerkleRoot, merkleRoot.c_str(), (std::min)(merkleRoot.size(), sizeof(hashMerkleRoot)));
		}
};

static_assert(sizeof(int32_t) == 4, "bad");
static_assert(sizeof(uint32_t) == 4, "very bad");
static_assert(sizeof(CBlockHeaderTruncatedLE) == 146, "vori e"); //146

#pragma pack(pop)

struct CBlockHeaderFullLE : public CBlockHeaderTruncatedLE
{
	uint32_t nNonce;
	char hashMix[65];

	CBlockHeaderFullLE(const void* data, uint32_t nonce, const uint8_t* hashMix_)
		: CBlockHeaderTruncatedLE(data)
		  , nNonce(nonce)
		  , hashMix{0}
	{
		auto mixString = GetHex(hashMix_, 32);
		/*cdebug << "NONCE:" << nNonce;
		  cdebug << "GET HEX HASH:" << mixString;*/
		std::memcpy(hashMix, mixString.c_str(), (std::min)(mixString.size(), sizeof(hashMix)));
	}
};
//static_assert(sizeof(CBlockHeaderFullLE) == 215, "CBlockHeaderFullLE has incorrect size");

