#include "ZipBuffer.h"

#include <zlib.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <utility>

namespace Lib3MF
{
    namespace helper
    {
        namespace
        {
            constexpr std::uint32_t kLocalFileHeaderSignature = 0x04034b50u;
            constexpr std::uint32_t kCentralDirectorySignature = 0x02014b50u;
            constexpr std::uint32_t kEndOfCentralDirectorySignature =
                0x06054b50u;
            constexpr std::uint32_t kZip64EndOfCentralDirectorySignature =
                0x06064b50u;
            constexpr std::uint32_t
                kZip64EndOfCentralDirectoryLocatorSignature = 0x07064b50u;

            std::uint16_t readUInt16(const std::vector<Lib3MF_uint8>& data,
                                     size_t offset)
            {
                if(offset + 2 > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: unexpected end of buffer while reading uint16");
                }
                return static_cast<std::uint16_t>(
                    data[offset] |
                    (static_cast<std::uint16_t>(data[offset + 1]) << 8U));
            }

            std::uint32_t readUInt32(const std::vector<Lib3MF_uint8>& data,
                                     size_t offset)
            {
                if(offset + 4 > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: unexpected end of buffer while reading uint32");
                }
                return static_cast<std::uint32_t>(
                    data[offset] |
                    (static_cast<std::uint32_t>(data[offset + 1]) << 8U) |
                    (static_cast<std::uint32_t>(data[offset + 2]) << 16U) |
                    (static_cast<std::uint32_t>(data[offset + 3]) << 24U));
            }

            std::uint64_t readUInt64(const std::vector<Lib3MF_uint8>& data,
                                     size_t offset)
            {
                if(offset + 8 > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: unexpected end of buffer while reading uint64");
                }
                return static_cast<std::uint64_t>(data[offset]) |
                       (static_cast<std::uint64_t>(data[offset + 1]) << 8U) |
                       (static_cast<std::uint64_t>(data[offset + 2]) << 16U) |
                       (static_cast<std::uint64_t>(data[offset + 3]) << 24U) |
                       (static_cast<std::uint64_t>(data[offset + 4]) << 32U) |
                       (static_cast<std::uint64_t>(data[offset + 5]) << 40U) |
                       (static_cast<std::uint64_t>(data[offset + 6]) << 48U) |
                       (static_cast<std::uint64_t>(data[offset + 7]) << 56U);
            }

            size_t findEndOfCentralDirectory(
                const std::vector<Lib3MF_uint8>& data)
            {
                constexpr size_t kEOCDMinSize = 22;
                if(data.size() < kEOCDMinSize)
                {
                    throw std::runtime_error(
                        "ZIP: buffer too small for end of central directory");
                }

                for(size_t offset = data.size() - kEOCDMinSize;;)
                {
                    if(readUInt32(data, offset) ==
                       kEndOfCentralDirectorySignature)
                    {
                        return offset;
                    }
                    if(offset == 0)
                    {
                        break;
                    }
                    --offset;
                }

                throw std::runtime_error(
                    "ZIP: end of central directory not found");
            }

            struct ZipEntryInfo
            {
                std::uint16_t generalPurposeFlag = 0;
                std::uint16_t compressionMethod = 0;
                std::uint64_t compressedSize = 0;
                std::uint64_t uncompressedSize = 0;
                std::uint64_t localHeaderOffset = 0;
                std::uint16_t fileNameLength = 0;
                std::uint16_t extraFieldLength = 0;
                std::uint16_t fileCommentLength = 0;
            };

            ZipEntryInfo readCentralDirectoryEntry(
                const std::vector<Lib3MF_uint8>& data, size_t& cursor,
                std::string& fileName)
            {
                if(readUInt32(data, cursor) != kCentralDirectorySignature)
                {
                    throw std::runtime_error(
                        "ZIP: invalid central directory signature");
                }

                ZipEntryInfo info;
                info.generalPurposeFlag = readUInt16(data, cursor + 8);
                info.compressionMethod = readUInt16(data, cursor + 10);
                auto const compressedSize32 = readUInt32(data, cursor + 20);
                auto const uncompressedSize32 = readUInt32(data, cursor + 24);
                info.fileNameLength = readUInt16(data, cursor + 28);
                info.extraFieldLength = readUInt16(data, cursor + 30);
                info.fileCommentLength = readUInt16(data, cursor + 32);
                auto const localHeaderOffset32 = readUInt32(data, cursor + 42);

                bool needsZip64Compressed = (compressedSize32 == 0xFFFFFFFFu);
                bool needsZip64Uncompressed =
                    (uncompressedSize32 == 0xFFFFFFFFu);
                bool needsZip64Offset = (localHeaderOffset32 == 0xFFFFFFFFu);

                info.compressedSize = compressedSize32;
                info.uncompressedSize = uncompressedSize32;
                info.localHeaderOffset = localHeaderOffset32;

                size_t const nameOffset = cursor + 46;
                size_t const nameEnd = nameOffset + info.fileNameLength;
                if(nameEnd > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: truncated filename in central directory entry");
                }

                fileName.assign(
                    reinterpret_cast<const char*>(&data[nameOffset]),
                    info.fileNameLength);

                size_t const extraOffset = nameEnd;
                size_t const extraEnd = extraOffset + info.extraFieldLength;
                if(extraEnd > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: extra field exceeds buffer bounds");
                }

                size_t extraCursor = extraOffset;
                while(extraCursor + 4 <= extraEnd)
                {
                    auto const headerId = readUInt16(data, extraCursor);
                    auto const fieldSize = readUInt16(data, extraCursor + 2);
                    size_t const fieldStart = extraCursor + 4;
                    size_t const fieldEnd = fieldStart + fieldSize;
                    if(fieldEnd > extraEnd)
                    {
                        throw std::runtime_error("ZIP: malformed extra field");
                    }

                    if(headerId == 0x0001)
                    {
                        size_t fieldCursor = fieldStart;
                        if(needsZip64Uncompressed)
                        {
                            if(fieldCursor + 8 > fieldEnd)
                            {
                                throw std::runtime_error(
                                    "ZIP: missing ZIP64 uncompressed size");
                            }
                            info.uncompressedSize =
                                readUInt64(data, fieldCursor);
                            fieldCursor += 8;
                            needsZip64Uncompressed = false;
                        }
                        if(needsZip64Compressed)
                        {
                            if(fieldCursor + 8 > fieldEnd)
                            {
                                throw std::runtime_error(
                                    "ZIP: missing ZIP64 compressed size");
                            }
                            info.compressedSize = readUInt64(data, fieldCursor);
                            fieldCursor += 8;
                            needsZip64Compressed = false;
                        }
                        if(needsZip64Offset)
                        {
                            if(fieldCursor + 8 > fieldEnd)
                            {
                                throw std::runtime_error(
                                    "ZIP: missing ZIP64 local header offset");
                            }
                            info.localHeaderOffset =
                                readUInt64(data, fieldCursor);
                            fieldCursor += 8;
                            needsZip64Offset = false;
                        }
                    }

                    extraCursor = fieldEnd;
                }

                if(needsZip64Compressed || needsZip64Uncompressed ||
                   needsZip64Offset)
                {
                    throw std::runtime_error(
                        "ZIP: ZIP64 extended data missing required values");
                }

                if(extraEnd + info.fileCommentLength > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: file comment exceeds buffer bounds");
                }

                cursor = extraEnd + info.fileCommentLength;
                if(cursor > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: central directory entry exceeds buffer bounds");
                }

                return info;
            }

            std::pair<std::uint64_t, std::uint64_t>
            resolveCentralDirectoryZip64(const std::vector<Lib3MF_uint8>& data,
                                         size_t eocdOffset)
            {
                size_t locatorOffset = std::numeric_limits<size_t>::max();
                if(eocdOffset >= 20)
                {
                    size_t const candidate = eocdOffset - 20;
                    if(readUInt32(data, candidate) ==
                       kZip64EndOfCentralDirectoryLocatorSignature)
                    {
                        locatorOffset = candidate;
                    }
                }

                if(locatorOffset == std::numeric_limits<size_t>::max())
                {
                    size_t const maxScan =
                        std::min(eocdOffset, static_cast<size_t>(1024));
                    for(size_t back = 4; back <= maxScan; ++back)
                    {
                        size_t const offset = eocdOffset - back;
                        if(readUInt32(data, offset) ==
                           kZip64EndOfCentralDirectoryLocatorSignature)
                        {
                            locatorOffset = offset;
                            break;
                        }
                    }
                }

                if(locatorOffset == std::numeric_limits<size_t>::max())
                {
                    throw std::runtime_error("ZIP: ZIP64 locator not found");
                }

                auto const zip64EocdOffset =
                    readUInt64(data, locatorOffset + 8);
                if(zip64EocdOffset > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: ZIP64 EOCD offset outside buffer");
                }

                size_t const zip64EocdPos =
                    static_cast<size_t>(zip64EocdOffset);
                if(readUInt32(data, zip64EocdPos) !=
                   kZip64EndOfCentralDirectorySignature)
                {
                    throw std::runtime_error(
                        "ZIP: ZIP64 EOCD signature mismatch");
                }

                auto const centralDirectorySize =
                    readUInt64(data, zip64EocdPos + 40);
                auto const centralDirectoryOffset =
                    readUInt64(data, zip64EocdPos + 48);

                return {centralDirectorySize, centralDirectoryOffset};
            }

            size_t getLocalDataOffset(const std::vector<Lib3MF_uint8>& data,
                                      const ZipEntryInfo& info)
            {
                if(info.localHeaderOffset > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: local header offset outside buffer");
                }

                size_t const offset =
                    static_cast<size_t>(info.localHeaderOffset);
                if(offset + 30 > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: truncated local file header");
                }

                if(readUInt32(data, offset) != kLocalFileHeaderSignature)
                {
                    throw std::runtime_error(
                        "ZIP: invalid local file header signature");
                }

                std::uint16_t const fileNameLength =
                    readUInt16(data, offset + 26);
                std::uint16_t const extraFieldLength =
                    readUInt16(data, offset + 28);

                size_t const dataOffset =
                    offset + 30 + fileNameLength + extraFieldLength;
                if(dataOffset > data.size())
                {
                    throw std::runtime_error(
                        "ZIP: local file data exceeds buffer bounds");
                }

                return dataOffset;
            }

            std::string decompressDeflate(const Lib3MF_uint8* data,
                                          size_t compressedSize,
                                          size_t uncompressedSize)
            {
                if(uncompressedSize == 0)
                {
                    return std::string();
                }

                std::string result(uncompressedSize, '\0');

                z_stream stream;
                stream.zalloc = Z_NULL;
                stream.zfree = Z_NULL;
                stream.opaque = Z_NULL;
                stream.next_in = Z_NULL;
                stream.avail_in = 0;
                stream.next_out = Z_NULL;
                stream.avail_out = 0;

                // Use -MAX_WBITS to indicate raw deflate stream (no zlib headers/footers).
                int const initResult = inflateInit2(&stream, -MAX_WBITS);
                if(initResult != Z_OK)
                {
                    throw std::runtime_error("ZIP: inflateInit2 failed");
                }

                size_t inOffset = 0;
                size_t outOffset = 0;

                auto const maxChunk =
                    static_cast<size_t>(std::numeric_limits<uInt>::max());

                int status = Z_OK;
                while(status != Z_STREAM_END)
                {
                    if(stream.avail_in == 0 && inOffset < compressedSize)
                    {
                        size_t const remaining = compressedSize - inOffset;
                        size_t const chunkSize = std::min(remaining, maxChunk);
                        stream.avail_in = static_cast<uInt>(chunkSize);
                        stream.next_in = const_cast<Bytef*>(
                            reinterpret_cast<const Bytef*>(data + inOffset));
                        inOffset += chunkSize;
                    }

                    if(stream.avail_out == 0 && outOffset < uncompressedSize)
                    {
                        size_t const remaining = uncompressedSize - outOffset;
                        size_t const chunkSize = std::min(remaining, maxChunk);
                        stream.avail_out = static_cast<uInt>(chunkSize);
                        stream.next_out =
                            reinterpret_cast<Bytef*>(&result[outOffset]);
                        outOffset += chunkSize;
                    }

                    status = inflate(&stream, Z_NO_FLUSH);
                    if(status != Z_OK && status != Z_STREAM_END)
                    {
                        inflateEnd(&stream);
                        throw std::runtime_error("ZIP: inflate failed");
                    }
                }

                inflateEnd(&stream);

                if(outOffset != uncompressedSize)
                {
                    throw std::runtime_error(
                        "ZIP: unexpected uncompressed size");
                }

                return result;
            }
        }  // unnamed namespace

        std::string extractZipEntryFromBuffer(
            const std::vector<Lib3MF_uint8>& buffer,
            const std::string& entryName)
        {
            auto const eocdOffset = findEndOfCentralDirectory(buffer);
            auto const centralDirectorySize32 =
                readUInt32(buffer, eocdOffset + 12);
            auto const centralDirectoryOffset32 =
                readUInt32(buffer, eocdOffset + 16);

            std::uint64_t centralDirectorySize = centralDirectorySize32;
            std::uint64_t centralDirectoryOffset = centralDirectoryOffset32;

            if(centralDirectorySize32 == 0xFFFFFFFFu ||
               centralDirectoryOffset32 == 0xFFFFFFFFu)
            {
                auto const resolved =
                    resolveCentralDirectoryZip64(buffer, eocdOffset);
                centralDirectorySize = resolved.first;
                centralDirectoryOffset = resolved.second;
            }

            if(centralDirectoryOffset > buffer.size())
            {
                throw std::runtime_error(
                    "ZIP: central directory offset outside buffer");
            }

            size_t centralDirectoryEnd = buffer.size();
            if(centralDirectorySize != 0)
            {
                auto const declaredEnd64 =
                    centralDirectoryOffset + centralDirectorySize;
                if(declaredEnd64 < centralDirectoryOffset)
                {
                    throw std::runtime_error(
                        "ZIP: central directory size overflow");
                }
                centralDirectoryEnd = std::min(
                    centralDirectoryEnd, static_cast<size_t>(declaredEnd64));
            }

            centralDirectoryEnd = std::min(centralDirectoryEnd, eocdOffset);

            size_t cursor = static_cast<size_t>(centralDirectoryOffset);
            if(cursor > centralDirectoryEnd)
            {
                throw std::runtime_error(
                    "ZIP: central directory offset beyond computed end");
            }

            while(cursor < centralDirectoryEnd)
            {
                std::string currentName;
                ZipEntryInfo const entry =
                    readCentralDirectoryEntry(buffer, cursor, currentName);
                if(currentName == entryName)
                {
                    size_t const dataOffset = getLocalDataOffset(buffer, entry);
                    auto const maxAvailable = buffer.size() - dataOffset;

                    if(entry.compressedSize >
                       static_cast<std::uint64_t>(maxAvailable))
                    {
                        throw std::runtime_error(
                            "ZIP: entry data exceeds buffer bounds");
                    }

                    if(entry.compressedSize >
                           static_cast<std::uint64_t>(
                               std::numeric_limits<size_t>::max()) ||
                       entry.uncompressedSize >
                           static_cast<std::uint64_t>(
                               std::numeric_limits<size_t>::max()))
                    {
                        throw std::runtime_error(
                            "ZIP: entry size too large for platform string");
                    }

                    size_t const compressedSize =
                        static_cast<size_t>(entry.compressedSize);
                    size_t const uncompressedSize =
                        static_cast<size_t>(entry.uncompressedSize);

                    auto const* dataPtr = &buffer[dataOffset];
                    switch(entry.compressionMethod)
                    {
                        case 0:
                            if(uncompressedSize != compressedSize)
                            {
                                throw std::runtime_error(
                                    "ZIP: stored entry size mismatch");
                            }
                            {
                                auto const* start =
                                    reinterpret_cast<const char*>(dataPtr);
                                auto const* end = start + uncompressedSize;
                                return std::string(start, end);
                            }
                        case 8:
                            return decompressDeflate(dataPtr, compressedSize,
                                                     uncompressedSize);
                        default:
                            throw std::runtime_error(
                                "ZIP: unsupported compression method");
                    }
                }
            }

            throw std::runtime_error("ZIP: entry not found: " + entryName);
        }

    }  // namespace helper
}  // namespace Lib3MF
