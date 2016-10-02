/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2016 rose-project
 * Copyright (c) 2016 Christian Fries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#include "pt/gpt.h"

#include <stdint.h>
#include <stdio.h>

extern uint32_t calculate_crc32(const unsigned char *data, size_t lenght)
{
    int i, j;
    uint32_t byte, crc32, mask;

    if(!data || lenght == 0)
        return 0;

    i = 0;
    crc32 = 0xFFFFFFFF;
    while (lenght--)
    {
        byte = data[i];
        crc32 = crc32 ^ byte;
        for (j = 7; j >= 0; j--)
        {
            mask = -(crc32 & 1);
            crc32 = (crc32 >> 1) ^ (0xEDB88320 & mask);
        }
        ++i;
    }
    return ~crc32;
}
