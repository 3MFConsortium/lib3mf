#pragma once

#include <string>
#include <vector>

#include "lib3mf_types.hpp"

namespace Lib3MF
{
    namespace helper
    {
        std::string extractZipEntryFromBuffer(
            const std::vector<Lib3MF_uint8>& buffer,
            const std::string& entryName);
    }  // namespace helper
}  // namespace Lib3MF
