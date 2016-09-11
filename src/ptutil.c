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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "pt/gpt.h"

int main(int argc, char* argv[])
{
    /* FIXME just a primitive first test app... */

    (void)argc;
    (void)argv;

    struct gpt_device device;

    if(-1 == gpt_init(&device))
    {
        fprintf(stderr, "Init failed\n");
        return EXIT_FAILURE;
    }

    if(-1 == gpt_validate())
    {
        fprintf(stderr, "Primary gpt invalid\n");
    }

    printf("Dump primary gpt\n");
    gpt_dump();

    if(-1 == gpt_validate())
    {
        fprintf(stderr, "Recovery gpt invalid\n");
    }

    printf("Dump Recovery gpt\n");
    gpt_dump();

    if(-1 == gpt_deInit(&device))
    {
        fprintf(stderr, "DeInit failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
