/*   UnLZMA.cpp (modified version of LzmaUtil.c file from lzma465 package;
     only decompress .lzma files)
 *
 *   Cruel Hessian
 *   Copyright (C) 2010 by Pawel Konieczny <konp84 at gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * aint with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


/* LzmaUtil.c -- Test application for LZMA compression
2008-11-23 : Igor Pavlov : Public domain */


#include <iostream>

#include "UnLZMA.h"


static void *SzAlloc(void *p, size_t size)
{
    p = p;
    return MyAlloc(size);
}

static void SzFree(void *p, void *address)
{
    p = p;
    MyFree(address);
}


SRes UnLZMA::Decode2(CLzmaDec *state, ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 unpackSize)
{

    const int IN_BUF_SIZE = 1 << 16;
    const int OUT_BUF_SIZE = 1 << 16;
    int thereIsSize = (unpackSize != (UInt64)(Int64)-1);
    Byte inBuf[IN_BUF_SIZE];
    Byte outBuf[OUT_BUF_SIZE];
    size_t inPos = 0, inSize = 0, outPos = 0;
    LzmaDec_Init(state);

    for (;;)
    {
        if (inPos == inSize)
        {
            inSize = IN_BUF_SIZE;
            RINOK(inStream->Read(inStream, inBuf, &inSize));
            inPos = 0;
        }

        SRes res;
        SizeT inProcessed = inSize - inPos;
        SizeT outProcessed = OUT_BUF_SIZE - outPos;
        ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
        ELzmaStatus status;
        if (thereIsSize && outProcessed > unpackSize)
        {
            outProcessed = (SizeT)unpackSize;
            finishMode = LZMA_FINISH_END;
        }

        res = LzmaDec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
                                  inBuf + inPos, &inProcessed, finishMode, &status);
        inPos += inProcessed;
        outPos += outProcessed;
        unpackSize -= outProcessed;

        if (outStream)
            if (outStream->Write(outStream, outBuf, outPos) != outPos)
                return SZ_ERROR_WRITE;

        outPos = 0;

        if (res != SZ_OK || (thereIsSize && unpackSize == 0))
            return res;

        if (inProcessed == 0 && outProcessed == 0)
        {
            if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
                return SZ_ERROR_DATA;
            return res;
        }
    }
}

SRes UnLZMA::Decode(ISeqOutStream *outStream, ISeqInStream *inStream)
{
    UInt64 unpackSize = 0;
    ISzAlloc g_Alloc = { SzAlloc, SzFree };
    CLzmaDec state;

    /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    unsigned char header[LZMA_PROPS_SIZE + 8];

    /* Read and parse header */

    RINOK(SeqInStream_Read(inStream, header, sizeof(header)));

    for (int i = 0; i < 8; i++)
        unpackSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);

    LzmaDec_Construct(&state);
    RINOK(LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc));
    SRes res = Decode2(&state, outStream, inStream, unpackSize);
    LzmaDec_Free(&state, &g_Alloc);
    return res;
}


int UnLZMA::Extract(const char* file_in, const char* dst_file)
{

    CFileSeqInStream inStream;
    CFileOutStream outStream;
    Bool useOutFile = False;

    FileSeqInStream_CreateVTable(&inStream);
    File_Construct(&inStream.file);

    FileOutStream_CreateVTable(&outStream);
    File_Construct(&outStream.file);

    size_t t4 = sizeof(UInt32);
    size_t t8 = sizeof(UInt64);
    if (t4 != 4 || t8 != 8)
        std::cout << "Incorrect UInt32 or UInt64" << std::endl;

    if (InFile_Open(&inStream.file, file_in) != 0)
        std::cout << "Can not open input file" << std::endl;

    useOutFile = True;
    if (OutFile_Open(&outStream.file, dst_file) != 0)
        std::cout << "Can not open output file" << std::endl;

    int res = Decode(&outStream.s, useOutFile ? &inStream.s : NULL);

    if (useOutFile)
        File_Close(&outStream.file);
    File_Close(&inStream.file);

    if (res != SZ_OK)
    {
        if (res == SZ_ERROR_MEM)
            std::cout << "Can not allocate memory" << std::endl;
        else if (res == SZ_ERROR_DATA)
            std::cout << "Data error" << std::endl;
        else if (res == SZ_ERROR_WRITE)
            std::cout << "Can not write output file" << std::endl;
        else if (res == SZ_ERROR_READ)
            std::cout << "Can not read input file" << std::endl;
        else
            std::cout << "Unknown error in Lzma.cpp" << std::endl;
        return 1;
    }

    return 0;
}

UnLZMA::UnLZMA()
{

}

UnLZMA::~UnLZMA()
{

}
