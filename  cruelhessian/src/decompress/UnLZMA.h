/*   UnLZMA.h (modified version of LzmaUtil.c file; only decompress .lzma files)
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

#ifndef LZMAX_H
#define LZMAX_H

#define _CRT_SECURE_NO_WARNINGS



#include "lzmadec/Alloc.h"
#include "lzmadec/7zFile.h"
#include "lzmadec/7zVersion.h"
#include "lzmadec/LzmaDec.h"


class UnLZMA
{

public:

    UnLZMA();
    ~UnLZMA();
    int Extract(const char* file_in, const char* dst_file);

private:

    SRes Decode2(CLzmaDec *state, ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 unpackSize);
    SRes Decode(ISeqOutStream *outStream, ISeqInStream *inStream);

};


#endif
