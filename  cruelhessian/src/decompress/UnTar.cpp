/*   UnTar.cpp (modified version of untar.c file; )

Changes:
 - converted to c++ and fixed many warnings
 - only unpacks .tar files (removed unused code)

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


/* DESCRIPTION:
 *	Untar extracts files from an uncompressed tar archive, or one which
 *	has been compressed with gzip. Usually such archives will have file
 *	names that end with ".tar" or ".tgz" respectively, although untar
 *	doesn't depend on any naming conventions.  For a summary of the
 *	command-line options, run untar with no arguments.
 *
 * HOW TO COMPILE:
 *	Untar doesn't require any special libraries or compile-time flags.
 *	A simple "cc untar.c -o untar" (or the local equivalent) is
 *	sufficient.  Even "make untar" works, without needing a Makefile.
 *	For Microsoft Visual C++, the command is "cl /D_WEAK_POSIX untar.c"
 *	(for 32 bit compilers) or "cl /F 1400 untar.c" (for 16-bit).
 *
 *	IF YOU SEE COMPILER WARNINGS, THAT'S NORMAL; you can ignore them.
 *	Most of the warnings could be eliminated by adding #include <string.h>
 *	but that isn't portable -- some systems require <strings.h> and
 *	<malloc.h>, for example.  Because <string.h> isn't quite portable,
 *	and isn't really necessary in the context of this program, it isn't
 *	included.
 *
 * PORTABILITY:
 *	Untar only requires the <stdio.h> header.  It uses old-style function
 *	definitions.  It opens all files in binary mode.  Taken together,
 *	this means that untar should compile & run on just about anything.
 *
 *	If your system supports the POSIX chmod(2), utime(2), link(2), and
 *	symlink(2) calls, then you may wish to compile with -D_POSIX_SOURCE,
 *	which will enable untar to use those system calls to restore the
 *	timestamp and permissions of the extracted files, and restore links.
 *	(For Linux, _POSIX_SOURCE is always defined.)
 *
 *	For systems which support some POSIX features but not enough to support
 *	-D_POSIX_SOURCE, you might be able to use -D_WEAK_POSIX.  This allows
 *	untar to restore time stamps and file permissions, but not links.
 *	This should work for Microsoft systems, and hopefully others as well.
 *
 * AUTHOR & COPYRIGHT INFO:
 *	Written by Steve Kirkendall, kirkenda@cs.pdx.edu
 *	Placed in public domain, 6 October 1995
 *
 *	Portions derived from inflate.c -- Not copyrighted 1992 by Mark Adler
 *	version c10p1, 10 January 1993
 */



#include "UnTar.h"



/*----------------------------------------------------------------------------*/

/* create a file for writing.  If necessary, create the directories leading up
 * to that file as well.
 */
FILE *UnTar::createpath(char *name2)
{
    FILE *fp;
    int	i;
    char name[1000];

    strcpy(name, XDIR_IN);
    strcat(name, name2);

    /* if we aren't allowed to overwrite and this file exists, return NULL */
    if (!force && _access(name, 0) == 0)
    {
        fprintf(stderr, "%s: exists, will not overwrite without \"-f\"\n", name);
        return NULL;
    }

    /* first try creating it the easy way */
    fp = fopen(name, convert ? "w" : "wb");
    if (fp)
        return fp;

    /* Else try making all of its directories, and then try creating
     * the file again.
     */
    for (i = 0; name[i]; i++)
    {
        /* If this is a slash, then temporarily replace the '/'
         * with a '\0' and do a mkdir() on the resulting string.
         * Ignore errors for now.
         */
        if (name[i] == '/')
        {
            name[i] = '\0';
            #ifdef _MSC_VER
            (void)_mkdir(name);
            #else
            (void)_mkdir(name, 0777);
            #endif
            name[i] = '/';
        }
        else
        {
            //perror("Possible error in createpath function");
        }
    }
    fp = fopen(name, convert ? "w" : "wb");
    if (!fp)
        perror(name);
    return fp;
}

/* Create a link, or copy a file.  If the file is copied (not linked) then
 * give a warning.
 */
void UnTar::linkorcopy(char *src, char *dst, int sym)
{
    FILE	*fpsrc;
    FILE	*fpdst;
    int	c;

    /* Open the source file.  We do this first to make sure it exists */
    fpsrc = fopen(src, "rb");
    if (!fpsrc)
    {
        perror(src);
        return;
    }

    /* Create the destination file.  On POSIX systems, this is just to
     * make sure the directory path exists.
     */
    fpdst = createpath(dst);
    if (!fpdst)
    {
        fclose(fpsrc);
        /* error message already given */
        return;
    }


#ifdef _POSIX_SOURCE
# ifndef _WEAK_POSIX
    /* first try to link it over, instead of copying */
    fclose(fpdst);
    unlink(dst);
    if (sym)
    {
        if (symlink(src, dst))
        {
            perror(dst);
        }
        fclose(fpsrc);
        return;
    }
    if (!link(src, dst))
    {
        /* This story had a happy ending */
        fclose(fpsrc);
        return;
    }

    /* Dang.  Reopen the destination again */
    fpdst = fopen(dst, "wb");
    /* This *can't* fail */

# endif /* _WEAK_POSIX */
#endif /* _POSIX_SOURCE */

    /* Copy characters */
    while ((c = getc(fpsrc)) != EOF)
        putc(c, fpdst);

    /* Close the files */
    fclose(fpsrc);
    fclose(fpdst);

    /* Give a warning */
    printf("%s: copy instead of link\n", dst);
}

/* This calls fwrite(), possibly after converting CR-LF to LF */
void UnTar::cvtwrite(Uchar_t *blk, Ulong_t size, FILE *fp)
{
    unsigned int i, j;
    static Uchar_t mod[TSIZE];

    if (convert)
    {
        for (i = j = 0; i < size; i++)
        {
            /* convert LF to local newline convention */
            if (blk[i] == LF)
                mod[j++] = '\n';
            /* If CR-LF pair, then delete the CR */
            else if (blk[i] == CR && (i+1 >= size || blk[i+1] == LF))
                ;
            /* other characters copied literally */
            else
                mod[j++] = blk[i];
        }
        size = j;
        blk = mod;
    }

    fwrite(blk, (size_t)size, sizeof(Uchar_t), fp);
}


/* Compute the checksum of a tar header block, and return it as a long int.
 * The checksum can be computed using either POSIX rules (unsigned bytes)
 * or Sun rules (signed bytes).
 */
long UnTar::checksum(tar_t *tblk, int sunny)
{
    long	sum;
    char	*scan;

    /* compute the sum of the first 148 bytes -- everything up to but not
     * including the checksum field itself.
     */
    sum = 0L;
    for (scan = (char *)tblk; scan < tblk->checksum; scan++)
    {
        sum += (*scan) & 0xff;
        if (sunny && (*scan & 0x80) != 0)
            sum -= 256;
    }

    /* for the 8 bytes of the checksum field, add blanks to the sum */
    sum += ' ' * sizeof tblk->checksum;
    scan += sizeof tblk->checksum;

    /* finish counting the sum of the rest of the block */
    for (; scan < (char *)tblk + sizeof *tblk; scan++)
    {
        sum += (*scan) & 0xff;
        if (sunny && (*scan & 0x80) != 0)
            sum -= 256;
    }

    return sum;
}



/* list files in an archive, and optionally extract them as well */
void UnTar::untar(Uchar_t *blk)
{
    static char	nbuf[256];/* storage space for prefix+name, combined */
    static char	*name,*n2;/* prefix and name, combined */
    static int	first = 1;/* Boolean: first block of archive? */
    //long		sum;	  /* checksum for this block */
    unsigned int		i;
    tar_t		tblk[1];

#ifdef _POSIX_SOURCE
    static mode_t		mode;		/* file permissions */
    static struct utimbuf	timestamp;	/* file timestamp */
#endif

    /* make a local copy of the block, and treat it as a tar header */
    tblk[0] = *(tar_t *)blk;

    /* process each type of tape block differently */
    if (outsize > TSIZE)
    {
        /* data block, but not the last one */
        if (outfp)
            cvtwrite(blk, (Ulong_t)TSIZE, outfp);
        //cvtwrite(blk, TSIZE, outfp);
        outsize -= TSIZE;
    }
    else if (outsize > 0)
    {
        /* last data block of current file */
        if (outfp)
        {
            cvtwrite(blk, outsize, outfp);
            fclose(outfp);
            outfp = NULL;
#ifdef _POSIX_SOURCE
            utime(nbuf, &timestamp);
            chmod(nbuf, mode);
#endif
        }
        outsize = 0;
    }
    else if ((tblk)->filename[0] == '\0')
    {
        /* end-of-archive marker */
        if (didabs)
            fprintf(stderr, "WARNING: Removed leading slashes because \"-p\" wasn't given.\n");
        //exit(0);
        return;
    }
    else
    {
        /* file header */

        /* half-assed verification -- does it look like header? */
        if ((tblk)->filename[99] != '\0'
                || ((tblk)->size[0] < '0'
                    && (tblk)->size[0] != ' ')
                || (tblk)->size[0] > '9')
        {
            if (first)
            {
                fprintf(stderr, "%s: not a valid tar file\n", inname);
                exit(2);
            }
            else
            {
                printf("WARNING: Garbage detected; preceding file may be damaged\n");
                exit(2);
            }
        }

        /* combine prefix and filename */
        memset(nbuf, 0, sizeof nbuf);
        name = nbuf;
        if ((tblk)->prefix[0])
        {
            strncpy(name, (tblk)->prefix, sizeof (tblk)->prefix);
            strcat(name, "/");
            strncat(name + strlen(name), (tblk)->filename,
                    sizeof (tblk)->filename);
        }
        else
        {
            strncpy(name, (tblk)->filename,
                    sizeof (tblk)->filename);
        }

        /* Convert any backslashes to forward slashes, and guard
         * against doubled-up slashes. (Some DOS versions of "tar"
         * get this wrong.)  Also strip off leading slashes.
         */
        if (!abspath && (*name == '/' || *name == '\\'))
            didabs = 1;
        for (n2 = nbuf; *name; name++)
        {
            if (*name == '\\')
                *name = '/';
            if (*name != '/'
                    || (abspath && n2 == nbuf)
                    || (n2 != nbuf && n2[-1] != '/'))
                *n2++ = *name;
        }
        if (n2 == nbuf)
            *n2++ = '/';
        *n2 = '\0';

        long sum;
        /* verify the checksum */
        for (sum = 0L, i = 0; i < sizeof((tblk)->checksum); i++)
        {
            if ((tblk)->checksum[i] >= '0'
                    && (tblk)->checksum[i] <= '7')
                sum = sum * 8 + (tblk)->checksum[i] - '0';
        }
        if (sum != checksum(tblk, 0) && sum != checksum(tblk, 1))
        {
            if (!first)
                printf("WARNING: Garbage detected; preceding file may be damaged\n");
            fflush(stdout);
            fprintf(stderr, "%s: header has bad checksum for %s\n", inname, nbuf);
            exit(2);
        }

        /* From this point on, we don't care whether this is the first
         * block or not.  Might as well reset the "first" flag now.
         */
        first = 0;

        /* if last character of name is '/' then assume directory */
        if (*nbuf && nbuf[strlen(nbuf) - 1] == '/')
            (tblk)->type = '5';

        /* convert file size */
        for (outsize = 0L, i = 0; i < sizeof((tblk)->size); i++)
        {
            if ((tblk)->size[i] >= '0' && (tblk)->size[i] <= '7')
                outsize = outsize * 8 + (tblk)->size[i] - '0';
        }

#ifdef _POSIX_SOURCE
        /* convert file timestamp */
        for (timestamp.modtime=0L, i=0; i < sizeof((tblk)->mtime); i++)
        {
            if ((tblk)->mtime[i] >= '0' && (tblk)->mtime[i] <= '7')
                timestamp.modtime = timestamp.modtime * 8
                                    + (tblk)->mtime[i] - '0';
        }
        timestamp.actime = timestamp.modtime;

        /* convert file permissions */
        for (mode = i = 0; i < sizeof((tblk)->mode); i++)
        {
            if ((tblk)->mode[i] >= '0' && (tblk)->mode[i] <= '7')
                mode = mode * 8 + (tblk)->mode[i] - '0';
        }
#endif

        /* If we have an "only" list, and this file isn't in it,
         * then skip it.
         */
        if (nonlys > 0)
        {
            for (i = 0;
                    i < nonlys
                    && strcmp(only[i], nbuf)
                    && (strncmp(only[i], nbuf, strlen(only[i]))
                        || nbuf[strlen(only[i])] != '/');
                    i++)
            {
            }
            if (i >= nonlys)
            {
                outfp = NULL;
                return;
            }
        }

        /* list the file */
        if (verbose)
            printf("%c %s",
                   ISREGULAR(*tblk) ? '-' : ("hlcbdp"[(tblk)->type - '1']),
                   nbuf);
        else if (!quiet)
            printf("%s\n", nbuf);

        /* if link, then do the link-or-copy thing */
        if (tblk->type == '1' || tblk->type == '2')
        {
            if (verbose)
                printf(" -> %s\n", tblk->linkto);
            if (!listing)
                linkorcopy(tblk->linkto, nbuf, tblk->type == '2');
            outsize = 0L;
            return;
        }

        /* If directory, then make a weak attempt to create it.
         * Ideally we would do the "create path" thing, but that
         * seems like more trouble than it's worth since traditional
         * tar archives don't contain directories anyway.
         */
        /*if (tblk->type == '5')
        {
        	printf("FILE : %s\n", nbuf);
        	if (listing)
        		n2 = " directory";
        #ifdef _POSIX_SOURCE
        	else if (mkdir(nbuf, mode) == 0)
        #else
        	else if (mkdir(nbuf, 0755) == 0)
        #endif
        		n2 = " created";
        	else
        		n2 = " ignored";
        	if (verbose)
        		printf("%s\n", n2);
        	return;
        }
        */
        /* if not a regular file, then skip it */
        if (!ISREGULAR(*tblk))
        {
            if (verbose)
                printf(" ignored\n");
            outsize = 0L;
            return;
        }

        /* print file statistics */
        if (verbose)
        {
            printf(" (%ld byte%s, %ld tape block%s)\n",
                   outsize,
                   outsize == 1 ? "" : "s",
                   (outsize + TSIZE - 1) / TSIZE,
                   (outsize > 0  && outsize <= TSIZE) ? "" : "s");
        }

        /* if extracting, then try to create the file */
        if (!listing)
            outfp = createpath(nbuf);
        else
            outfp = NULL;

        /* if file is 0 bytes long, then we're done already! */
        if (outsize == 0 && outfp)
        {
            fclose(outfp);
#ifdef _POSIX_SOURCE
            utime(nbuf, &timestamp);
            chmod(nbuf, mode);
#endif
        }
    }
}



/* Process an archive file.  This involves reading the blocks one at a time
 * (decompressing if necessary as it goes along) and passing them to a untar()
 * function.
 */
int UnTar::Extract(char *file_in, char *dir_out)
{

    /* open the archive */
    inname = file_in;
    XDIR_IN = dir_out;
    infp = fopen(file_in, "rb");
    if (!infp)
    {
        perror(file_in);
        return 1;
    }

    /* read the first few bytes, so we can determine whether to decompress */
    size_t q = fread(slide, 1, sizeof(gzhdr_t), infp);
    if ((((gzhdr_t *)slide)->magic[0] == MAGIC0
            && ((gzhdr_t *)slide)->magic[1] == MAGIC1) == 0)
    {

        /* UNCOMPRESSED */

        /* if we were supposed to just decompress, complain */
        /*if (maketar)
        {
            fprintf(stderr, "%s: isn't gzipped\n", file_in);
            fclose(infp);
            return 1;
        }*/

        /* read the rest of the first block */
        q = fread(&slide[sizeof(gzhdr_t)], 1, TSIZE - sizeof(gzhdr_t), infp);

        /* send each block to the untar() function */
        do
        {
            //printf("QWE\n");
            untar(slide);
        }
        while (fread(slide, 1, TSIZE, infp) == TSIZE);
        //printf("LALA\n");
    }

    /* close the archive file. */
    fclose(infp);

    if (outsize > 0)
    {
        printf("WARNING: Last file might be truncated!\n");
        fclose(outfp);
        outfp = NULL;
    }

    return 0;

}


UnTar::UnTar()
{
    listing = 0;
    quiet = 1;
    force = 1;
    abspath = 0;
    convert = 0;
    verbose = 0;
    noname = 0;
    didabs = 0;
    nonlys = 0;
    outsize = 0;
    infp = outfp = NULL;
    inname = NULL;
    XDIR_IN = NULL;
    wp = 0;

    for (int i = 0; i < WSIZE; ++i)
        slide[i] = NULL;

}

UnTar::~UnTar()
{

}
