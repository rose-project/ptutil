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
#ifndef GPT_H
#define GPT_H

#include <stdint.h>
#include <stdbool.h>

enum gpt_header_type {
    GPT_PRIMARY = 0,
    GPT_BACKUP  = 1,
};

typedef struct gpt_header gpt_header;

struct gpt_device
{
    const char *path;
    int         fd;

    gpt_header *primary;
    gpt_header *backup;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief gpt_init must be called to initialize the gpt_device structure
 * and to open the given device or file container
 * @param device uninitialized device struct
 * @param device_path path to a gpt image or disk
 * @return false on error
 */
extern bool gpt_init(struct gpt_device *device,
                     const char* device_path);

/**
 * @brief gpt_deInit deallocates gpt_device and close all open file descriptors
 * @param device initialized structure
 * @return false on error
 */
extern bool gpt_deInit(struct gpt_device *device);

/**
 * @brief gpt_validate
 * @param device initialized structure
 * @param index primary/backup gpt table index
 * @return -1 on error
 */
extern int gpt_validate(const struct gpt_device *device,
                        uint8_t index,
                        bool repair_crc);

/**
 * @brief gpt_invalidate
 * @param device initialized structure
 * @param index primary/backup gpt table index
 * @param force if set to true, other table will not be validated!
 *        Use only with caution! Should never done on a boot disk!
 * @return -1 on error
 */
extern int gpt_invalidate(const struct gpt_device *device,
                          uint8_t index,
                          bool force);

/**
 * @brief gpt_dump display all relevant gpt table informations and partition entries
 * @param device initialized structure
 * @param index primary/backup gpt table index
 */
extern void gpt_dump(const struct gpt_device *device,
                     uint8_t index);

#ifdef __cplusplus
}
#endif

#endif // GPT_H
