#pragma once

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "transaction.h"
#include "utilstrencodings.h"
#include "serialize.h"
#include "uint256.h"


namespace energi {

struct BlockHeader
{
    int32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nHeight;
    uint256 hashMix;
    uint64_t nNonce;

    BlockHeader()
    {
        SetNull();
    }

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nHeight);
        READWRITE(hashMix);
        READWRITE(nNonce);
    }

    void SetNull()
    {
        nVersion = 0;
        hashPrevBlock.SetNull();
        hashMerkleRoot.SetNull();
        nTime = 0;
        nHeight = 0;
        hashMix.SetNull();
        nNonce = 0;
    }
};

struct Block : public BlockHeader
{
    std::vector<CTransaction> vtx;

    CTxOut txoutBackbone; // Energibackbone payment
    CTxOut txoutMasternode; // masternode payment
    std::vector<CTxOut> voutSuperblock; //superblock payment

    Block()
    {
        SetNull();
    }
    Block(const BlockHeader& header)
    {
        SetNull();
        *((BlockHeader*)this) = header;
    }

    ADD_SERIALIZE_METHODS

    template<typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(*(BlockHeader*)this);
        READWRITE(vtx);
    }

    void SetNull()
    {
        BlockHeader::SetNull();
        vtx.clear();
        txoutBackbone = CTxOut();
        txoutMasternode = CTxOut();
        voutSuperblock.clear();
    }
};

#pragma pack(push, 1)

struct CBlockHeaderTruncatedLE
{
    int32_t nVersion;
    char hashPrevBlock[65];
    char hashMerkleRoot[65];
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nHeight;

    CBlockHeaderTruncatedLE(const BlockHeader& header)
        : nVersion(htole32(header.nVersion))
        , hashPrevBlock{0}
        , hashMerkleRoot{0}
        , nTime(htole32(header.nTime))
        , nBits(htole32(header.nBits))
        , nHeight(htole32(header.nHeight))
    {
        auto prevHash = header.hashPrevBlock.ToString();
        memcpy(hashPrevBlock, prevHash.c_str(), (std::min)(prevHash.size(), sizeof(hashPrevBlock)));

        auto merkleRoot = header.hashMerkleRoot.ToString();
        memcpy(hashMerkleRoot, merkleRoot.c_str(), (std::min)(merkleRoot.size(), sizeof(hashMerkleRoot)));
    }
};

static_assert(sizeof(CBlockHeaderTruncatedLE) == 146, "CBlockHeaderTruncatedLE has incorrect size");
#pragma pack(pop)

} // namespace energi
