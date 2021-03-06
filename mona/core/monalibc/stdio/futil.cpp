/*************************************************************
 * Copyright (c) 2006 Shotaro Tsuji
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is     * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *************************************************************/

/* Please send bug reports to
    Shotaro Tsuji
    4-1010,
    Sakasedai 1-chome,
    Takaraduka-si,
    Hyogo-ken,
    665-0024
    Japan
    negi4d41@yahoo.co.jp
*/

#include <monapi/messages.h>
#include <monapi.h>
#include <sys/error.h>
#include <stdlib.h>
#include <string.h>
#include "stdio_p.h"

using namespace MonAPI;

extern "C" void stream_opener();

FILE __sF[3];

int _read(void *self, void *buf, size_t size)
{
    FILE *f;
    uint32_t fid;
    unsigned char *p = (unsigned char*)buf;
    int readsize;
    int i;
    f = (FILE*)self;
    fid = f->_file;
    scoped_ptr<SharedMemory> shm(monapi_file_read(fid, (uint32_t)size));
    // NULL means, EOF.
    if( shm.get() == NULL )
    {
        return 0;
    }
    readsize = (int)shm->size();
    memcpy(p, shm->data(), readsize);
    return readsize;
}

int _write(void *self, void *buf, size_t size)
{
    uint32_t result;
    FILE *f;

    f = (FILE*)self;
    if (0 == size) {
        return 0;
    }

    MonAPI::SharedMemory shm(size);
    if(shm.map() != M_OK) {
        return -1;
    }
    memcpy(shm.data(), buf, shm.size());

    result = monapi_file_write((uint32_t)f->_file, shm, shm.size());

    return (int)result;
}

int _seek(void *self, fpos_t pos, int whence)
{
    MONAPI_BOOL result;
    FILE *f = (FILE*)self;

    result = monapi_file_seek((uint32_t)f->_file, (uint32_t)pos, (uint32_t)whence);
    if( result < 0 )
    {
        return -1;
    }

    return 0;
}

void _open_stdin(FILE *fp)
{
    memset(fp, 0, sizeof(FILE));
    fp->_flags = 0;
    fp->_flags = __SRD|__SLBF;
    fp->_extra = (struct __sFILEX*)malloc(sizeof(struct __sFILEX));
    fp->_extra->stds = __STDIN;
}

void _open_stdout(FILE *fp)
{
    memset(fp, 0, sizeof(FILE));

    fp->_flags = 0;
    fp->_flags = __SWR|__SLBF;
    fp->_extra = (struct __sFILEX*)malloc(sizeof(struct __sFILEX));
    fp->_extra->stds = __STDOUT;
}

void _open_stderr(FILE *fp)
{
    memset(fp, 0, sizeof(FILE));

    fp->_flags = 0;
    fp->_flags = __SWR|__SNBF;
    fp->_extra = (struct __sFILEX*)malloc(sizeof(struct __sFILEX));
    fp->_extra->stds = __STDERR;
}

extern "C" void init_stdio(void)
{
    _open_stdin(&(__sF[0]));
    _open_stdout(&(__sF[1]));
    _open_stderr(&(__sF[2]));
    stream_opener();
/*  _logprintf("monalibc: stdin = %x\n", stdin); */
/*  _logprintf("monalibc: stdout= %x\n", stdout); */
/*  _logprintf("monalibc: stderr= %x\n", stderr); */
/*  _logprintf("monalibc: __sF[0]._extra = %x\n", __sF[0]._extra); */
/*  _logprintf("monalibc: __sF[1]._extra = %x\n", __sF[1]._extra); */
/*  _logprintf("monalibc: __sF[2]._extra = %x\n", __sF[2]._extra); */
/*  logprintf("monalibc: stdin = %x\n", stdin); */
/*  logprintf("monalibc: stdout= %x\n", stdout); */
/*  logprintf("monalibc: stderr= %x\n", stderr); */
}

void fini_stdio(void)
{
    fclose(&__sF[2]);
    fclose(&__sF[1]);
    fclose(&__sF[0]);
}
