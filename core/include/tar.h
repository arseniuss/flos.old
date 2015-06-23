/**
 * @file    include/tar.h
 * @brief   Tape ARchive format
 * @version 1.0
 * @date    23.06.2015.
 * @source  http://www.scs.stanford.edu/histar/src/pkg/uclibc/include/tar.h
 *
 * @author David J. MacKenzie
 * @copyright GNU LGPL v2.1
 */

#ifndef __flos__TAR_H__
#    define __flos__TAR_H__

/* A tar archive consists of 512-byte blocks.
   Each file in the archive has a header block followed by 0+ data blocks.
   Two blocks of NUL bytes indicate the end of the archive.  */

/* The fields of header blocks:
  All strings are stored as ISO 646 (approximately ASCII) strings.

  Fields are numeric unless otherwise noted below; numbers are ISO 646
  representations of octal numbers, with leading zeros as needed.

  linkname is only valid when typeflag==LNKTYPE.  It doesn't use prefix;
  files that are links to pathnames >100 chars long can not be stored
  in a tar archive.

  If typeflag=={LNKTYPE,SYMTYPE,DIRTYPE} then size must be 0.

  devmajor and devminor are only valid for typeflag=={BLKTYPE,CHRTYPE}.

  chksum contains the sum of all 512 bytes in the header block,
  treating each byte as an 8-bit unsigned value and treating the
  8 bytes of chksum as blank characters.

  uname and gname are used in preference to uid and gid, if those
  names exist locally.

  Field Name	Byte Offset	Length in Bytes	Field Type
  name		    0		    100		NUL-terminated if NUL fits
  mode		    100		    8
  uid		    108		    8
  gid		    116		    8
  size		    124		    12
  mtime	        136		    12
  chksum	    148		    8
  typeflag	    156		    1		see below
  linkname	    157		    100		NUL-terminated if NUL fits
  magic	        257		    6		must be TMAGIC (NUL term.)
  version	    263		    2		must be TVERSION
  uname	        265		    32		NUL-terminated
  gname	        297		    32		NUL-terminated
  devmajor	    329		    8
  devminor	    337		    8
  prefix	    345		    155		NUL-terminated if NUL fits

  If the first character of prefix is '\0', the file name is name;
  otherwise, it is prefix/name.  Files whose pathnames don't fit in that
  length can not be stored in a tar archive.  */

struct tar_hdr {                /* byte offset */
    char name[100];             /*   0 */
    char mode[8];               /* 100 */
    char uid[8];                /* 108 */
    char gid[8];                /* 116 */
    char size[12];              /* 124 */
    char mtime[12];             /* 136 */
    char chksum[8];             /* 148 */
    char typeflag;              /* 156 */
    char linkname[100];         /* 157 */
    char magic[6];              /* 257 */
    char version[2];            /* 263 */
    char uname[32];             /* 265 */
    char gname[32];             /* 297 */
    char devmajor[8];           /* 329 */
    char devminor[8];           /* 337 */
    char prefix[155];           /* 345 */
};

/* The bits in mode: */
#    define TSUID	04000
#    define TSGID	02000
#    define TSVTX	01000
#    define TUREAD	00400
#    define TUWRITE	00200
#    define TUEXEC	00100
#    define TGREAD	00040
#    define TGWRITE	00020
#    define TGEXEC	00010
#    define TOREAD	00004
#    define TOWRITE	00002
#    define TOEXEC	00001

/* The values for typeflag:
  Values 'A'-'Z' are reserved for custom implementations.
  All other values are reserved for future POSIX.1 revisions.  */

#    define REGTYPE		'0'            /* Regular file (preferred code).  */
#    define AREGTYPE	'\0'           /* Regular file (alternate code).  */
#    define LNKTYPE		'1'            /* Hard link.  */
#    define SYMTYPE		'2'            /* Symbolic link (hard if not supported).  */
#    define CHRTYPE		'3'            /* Character special.  */
#    define BLKTYPE		'4'            /* Block special.  */
#    define DIRTYPE		'5'            /* Directory.  */
#    define FIFOTYPE	'6'            /* Named pipe.  */
#    define CONTTYPE	'7'            /* Contiguous file */
/* (regular file if not supported).  */

/* Contents of magic field and its length.  */
#    define TMAGIC	"ustar"
#    define TMAGLEN	6

/* Contents of the version field and its length.  */
#    define TVERSION	"00"
#    define TVERSLEN	2

#endif /* __flos__TAR_H__ */
