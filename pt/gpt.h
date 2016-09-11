/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2016 rose-project
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

#include <stdint.h>

typedef struct gpt_header gpt_header;

struct gpt_device
{
    const char *path;
    int         fd;

    gpt_header *primary;
    gpt_header *backup;
};


/**
 * @brief gpt_init
 * @return -1 on error
 */
int gpt_init(struct gpt_device *device, char* device_path);

/**
 * @brief gpt_deInit
 * @return -1 on error
 */
int gpt_deInit(struct gpt_device *device);

/**
 * @brief gpt_validate
 * @return -1 on error
 */
int gpt_validate();

/**
 * @brief gpt_invalidate
 * @return -1 on error
 */
int gpt_invalidate();

/**
 * @brief gpt_dump
 */
void gpt_dump();

