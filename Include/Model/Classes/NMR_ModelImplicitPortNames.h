/*++

Copyright (C) 2023 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

--*/

#pragma once


#include <vector>
#include <string>

namespace NMR
{
    struct InputNames
    {
        static auto constexpr A = "A";
        static auto constexpr B = "B";
        static auto constexpr C = "C";
        static auto constexpr D = "D";
        static auto constexpr m00 = "m00";
        static auto constexpr m01 = "m01";
        static auto constexpr m02 = "m02";
        static auto constexpr m03 = "m03";
        static auto constexpr m10 = "m10";
        static auto constexpr m11 = "m11";
        static auto constexpr m12 = "m12";
        static auto constexpr m13 = "m13";
        static auto constexpr m20 = "m20";
        static auto constexpr m21 = "m21";
        static auto constexpr m22 = "m22";
        static auto constexpr m23 = "m23";
        static auto constexpr m30 = "m30";
        static auto constexpr m31 = "m31";
        static auto constexpr m32 = "m32";
        static auto constexpr m33 = "m33";
        static auto constexpr x = "x";
        static auto constexpr y = "y";
        static auto constexpr z = "z";
        static auto constexpr pos = "pos";
        static auto constexpr mesh = "mesh";
        static auto constexpr color = "color";
        static auto constexpr min = "min";
        static auto constexpr max = "max";
    };

    struct OutputNames
    {
        static auto constexpr result = "result";
        static auto constexpr value = "value";
        static auto constexpr vector = "vector";
        static auto constexpr matrix = "matrix";
        static auto constexpr color = "color";
        static auto constexpr distance = "distance";
    };
}
