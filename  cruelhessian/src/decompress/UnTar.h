/*   UnTar.h (modified version of untar.c file; only unpacks .tar files)
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


#ifndef UNTAR_H
#define UNTAR_H


#include <cstdio>
#include <cstring>
#include <cstdlib>



#ifndef SEEK_SET
# define SEEK_SET 0
#endif

#ifdef _WEAK_POSIX
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
#endif


#ifdef _MSC_VER
# include <direct.h>
# include <io.h>
#else
# include <unistd.h>
  #define _mkdir mkdir
  #define _access access
#endif


#ifdef _POSIX_SOURCE
# include <sys/types.h>
# include <sys/stat.h>
# include <utime.h>
# ifdef _WEAK_POSIX
#  define mode_t int
# else
#  include <unistd.h>
# endif
#endif

#define WSIZE	32768	/* size of decompression buffer */
#define TSIZE	512	/* size of a "tape" block */
#define CR	13	/* carriage-return character */
#define LF	10	/* line-feed character */

typedef unsigned char	Uchar_t;
typedef unsigned short	Ushort_t;
typedef unsigned long	Ulong_t;


#define MAGIC0	0x1f
#define MAGIC1	0x8b
#define NAME	0x08



class UnTar
{

public:

    UnTar();
    ~UnTar();
    int Extract(char *file_in, char *dir_out);


private:

    typedef struct
    {
        Uchar_t	magic[2];	/* magic: 0x1F, 0x8b */
        Uchar_t	compression;	/* compression method: 8=deflated */
        Uchar_t	flags;		/* content flags: 0x08 bit -> name present */
        Uchar_t	mtime[4];	/* time_t when archive created */
        Uchar_t	extraflags;	/* ? */
        Uchar_t	os;		/* operating system: 3=UNIX */
        /* if flags&0x08, then original file name goes here, '\0'-terminated */
    } gzhdr_t;


    typedef struct
    {
        char	filename[100];	/*   0  name of next file */
        char	mode[8];	/* 100  Permissions and type (octal digits) */
        char	owner[8];	/* 108  Owner ID (ignored) */
        char	group[8];	/* 116  Group ID (ignored) */
        char	size[12];	/* 124  Bytes in file (octal digits) */
        char	mtime[12];	/* 136  Modification time stamp (octal digits)*/
        char	checksum[8];	/* 148  Header checksum (ignored) */
        char	type;		/* 156  File type (see below) */
        char	linkto[100];	/* 157  Linked-to name */
        char	brand[8];	/* 257  Identifies tar version (ignored) */
        char	ownername[32];	/* 265  Name of owner (ignored) */
        char	groupname[32];	/* 297  Name of group (ignored) */
        char	devmajor[8];	/* 329  Device major number (ignored) */
        char	defminor[8];	/* 337  Device minor number (ignored) */
        char	prefix[155];	/* 345  Prefix of name (optional) */
        char	RESERVED[12];	/* 500  Pad header size to 512 bytes */
    } tar_t;


    FILE *createpath(char *name2);
    void linkorcopy(char *src, char *dst, int sym);
    void cvtwrite(Uchar_t *blk, Ulong_t	size, FILE *fp);
    long checksum(tar_t *tblk, int sunny);
    void untar(Uchar_t *blk);


    int wp;	/* output counter */
    Uchar_t slide[WSIZE];
//#define error(desc)	{fprintf(stderr, "%s:%s", inname, (desc)); exit(1);}

    char *inname;/* name of input archive */
    FILE *infp;	/* input byte stream */
    FILE *outfp;	/* output stream, for file currently being extracted */
    Ulong_t	outsize;/* number of bytes remainin in file currently being extracted */
//    FILE *tarfp;	/* usually NULL; else file for writing gunzipped data */
    //int	maketar;/* -d: 1 to gunzip only, 0 to gunzip and extract tar files */
    int	listing;/* -t: 1 if listing, 0 if extracting */
    int	quiet;	/* -q: 1 to write nothing to stdout, 0 for normal chatter */
    int	verbose;/* -v: 1 to write extra information to stdout */
    int	force;	/* -f: 1 to overwrite existing files, 0 to skip them */
    int	abspath;/* -p: 1 to allow leading '/', 0 to strip leading '/' */
    int	convert;/* -c: 1 to convert newlines, 0 to leave unchanged */
    int	noname;	/* -n: 1 to ignore gzip name, 0 to use gzip name */
    char **only;	/* array of filenames to extract/list */
    unsigned int	nonlys;	/* number of filenames in "only" array; 0=extract all */
    int	didabs;	/* were any filenames affected by the absence of -p? */
    char *XDIR_IN;
};


#define ISREGULAR(hdr)	((hdr).type < '1' || (hdr).type > '6')


#endif
