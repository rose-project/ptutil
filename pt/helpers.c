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

#include "helpers.h"

#include <stdlib.h>

extern int encode_utf16_to_utf8(unsigned char *dest, const uint16_t *src, size_t count)
{
    uint16_t c;
    uint8_t *conv = (uint8_t*)src;
    size_t i, j;

    if (!dest)
        return -1;

    for (i = j = 0; i + 2 <= count; i += 2)
    {
        c = (conv[i+1] << 8) | conv[i];
        if (c == 0)	/* Null termination*/
        {
            dest[j] = '\0';
            return 0;
        }
        else if (c < 0x80)
        {
            if (j+1 >= count)
                break;
            dest[j++] = (uint8_t) c;
        }
        else if (c < 0x800)
        {
            if (j+2 >= count)
                break;
            dest[j++] = (uint8_t) (0xc0 | (c >> 6));
            dest[j++] = (uint8_t) (0x80 | (c & 0x3f));
        }
        else
        {
            if (j+3 >= count)
                break;
            dest[j++] = (uint8_t) (0xe0 | (c >> 12));
            dest[j++] = (uint8_t) (0x80 | ((c >> 6) & 0x3f));
            dest[j++] = (uint8_t) (0x80 | (c & 0x3f));
        }
    }
    dest[j] = '\0';
    return 0;
}
