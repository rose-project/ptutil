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

#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>
#include <string.h>

/*
 * Logging
 */

#ifdef ENABLE_LOGGING
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define _logging_macro(chan, title, format, ...) do { fprintf(chan, title "%s (%d): " format "\n", __FILENAME__, __LINE__,  ##__VA_ARGS__); fflush(chan); } while(0)
#else
#define _logging_macro(chan, format, ...)
#endif // ENABLE_LOGGING

#define logDbg(format, ...)  _logging_macro(stdout, "Debug ",   format, ##__VA_ARGS__)
#define logWarn(format, ...) _logging_macro(stderr, "Warning ", format, ##__VA_ARGS__)
#define logErr(format, ...)  _logging_macro(stderr, "Error ",   format, ##__VA_ARGS__)


/*
 * Assert Macro
 */
#ifdef ENABLE_ASSERTS
#   define debugbreak() __builtin_trap()
#   define reportAssertionFailure(...) printf("%s (%d): %s\n", __VA_ARGS__)

#   define ASSERT(expr) do { \
        if(!(expr)) {    \
            reportAssertionFailure(__FILE__, __LINE__, #expr); \
            abort(); \
        }} while(0)
#else
#   define ASSERT(expr)
#endif // ENABLE_ASSERTS


#endif // HELPERS_H
