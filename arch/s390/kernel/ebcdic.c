/*
 *  arch/s390/kernel/ebcdic.c
 *    ECBDIC -> ASCII, ASCII -> ECBDIC,
 *    upper to lower case (EBCDIC) conversion tables.
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 *               Martin Peschke <peschke@fh-brandenburg.de>
 */

#include <linux/module.h>
#include <asm/types.h>

/*
 * ASCII (IBM PC 437)  -> EBCDIC 037
 */
__u8 _ascebc[256] =
{
 /*00 NUL   SOH   STX   ETX   EOT   ENQ   ACK   BEL */
     0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,
 /*08  BS    HT    LF    VT    FF    CR    SO    SI */
 /*              ->NL                               */
     0x16, 0x05, 0x15, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 /*10 DLE   DC1   DC2   DC3   DC4   NAK   SYN   ETB */
     0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,
 /*18 CAN    EM   SUB   ESC    FS    GS    RS    US */
 /*                               ->IGS ->IRS ->IUS */
     0x18, 0x19, 0x3F, 0x27, 0x22, 0x1D, 0x1E, 0x1F,
 /*20  SP     !     "     #     $     %     &     ' */
     0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,
 /*28   (     )     *     +     ,     -    .      / */
     0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
 /*30   0     1     2     3     4     5     6     7 */
     0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
 /*38   8     9     :     ;     <     =     >     ? */
     0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
 /*40   @     A     B     C     D     E     F     G */
     0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
 /*48   H     I     J     K     L     M     N     O */
     0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
 /*50   P     Q     R     S     T     U     V     W */
     0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
 /*58   X     Y     Z     [     \     ]     ^     _ */
     0xE7, 0xE8, 0xE9, 0xBA, 0xE0, 0xBB, 0xB0, 0x6D,
 /*60   `     a     b     c     d     e     f     g */
     0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 /*68   h     i     j     k     l     m     n     o */
     0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 /*70   p     q     r     s     t     u     v     w */
     0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
 /*78   x     y     z     {     |     }     ~    DL */
     0xA7, 0xA8, 0xA9, 0xC0, 0x4F, 0xD0, 0xA1, 0x07,
 /*80*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*88*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*90*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*98*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*A0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*A8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*B0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*B8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*C0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*C8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*D0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*D8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*E0        sz						*/
     0x3F, 0x59, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*E8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*F0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*F8*/
     0x90, 0x3F, 0x3F, 0x3F, 0x3F, 0xEA, 0x3F, 0xFF
};

/*
 * EBCDIC 037 -> ASCII (IBM PC 437)
 */
__u8 _ebcasc[256] =
{
 /* 0x00   NUL   SOH   STX   ETX  *SEL    HT  *RNL   DEL */
          0x00, 0x01, 0x02, 0x03, 0x07, 0x09, 0x07, 0x7F,
 /* 0x08   -GE  -SPS  -RPT    VT    FF    CR    SO    SI */
          0x07, 0x07, 0x07, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 /* 0x10   DLE   DC1   DC2   DC3  -RES   -NL    BS  -POC
                                  -ENP  ->LF             */
          0x10, 0x11, 0x12, 0x13, 0x07, 0x0A, 0x08, 0x07,
 /* 0x18   CAN    EM  -UBS  -CU1  -IFS  -IGS  -IRS  -ITB
                                                    -IUS */
          0x18, 0x19, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
 /* 0x20   -DS  -SOS    FS  -WUS  -BYP    LF   ETB   ESC
                                  -INP                   */
          0x07, 0x07, 0x1C, 0x07, 0x07, 0x0A, 0x17, 0x1B,
 /* 0x28   -SA  -SFE   -SM  -CSP  -MFA   ENQ   ACK   BEL
                       -SW                               */ 
          0x07, 0x07, 0x07, 0x07, 0x07, 0x05, 0x06, 0x07,
 /* 0x30  ----  ----   SYN   -IR   -PP  -TRN  -NBS   EOT */
          0x07, 0x07, 0x16, 0x07, 0x07, 0x07, 0x07, 0x04,
 /* 0x38  -SBS   -IT  -RFF  -CU3   DC4   NAK  ----   SUB */
          0x07, 0x07, 0x07, 0x07, 0x14, 0x15, 0x07, 0x1A,
 /* 0x40    SP   RSP           ??              ----       */
          0x20, 0xFF, 0x83, 0x84, 0x85, 0xA0, 0x07, 0x86,
 /* 0x48                       .     <     (     +     | */
          0x87, 0xA4, 0x9B, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
 /* 0x50     &                                      ---- */
          0x26, 0x82, 0x88, 0x89, 0x8A, 0xA1, 0x8C, 0x07,
 /* 0x58           ??     !     $     *     )     ;       */
          0x8D, 0xE1, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0xAA,
 /* 0x60     -     /  ----     ??  ----  ----  ----       */
          0x2D, 0x2F, 0x07, 0x8E, 0x07, 0x07, 0x07, 0x8F,
 /* 0x68              ----     ,     %     _     >     ? */ 
          0x80, 0xA5, 0x07, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
 /* 0x70  ----        ----  ----  ----  ----  ----  ---- */
          0x07, 0x90, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
 /* 0x78     *     `     :     #     @     '     =     " */
          0x70, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
 /* 0x80     *     a     b     c     d     e     f     g */
          0x07, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 /* 0x88     h     i              ----  ----  ----       */
          0x68, 0x69, 0xAE, 0xAF, 0x07, 0x07, 0x07, 0xF1,
 /* 0x90     ??     j     k     l     m     n     o     p */
          0xF8, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
 /* 0x98     q     r                    ----        ---- */
          0x71, 0x72, 0xA6, 0xA7, 0x91, 0x07, 0x92, 0x07,
 /* 0xA0           ~     s     t     u     v     w     x */
          0xE6, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 /* 0xA8     y     z              ----  ----  ----  ---- */
          0x79, 0x7A, 0xAD, 0xAB, 0x07, 0x07, 0x07, 0x07,
 /* 0xB0     ^                    ----     ??  ----       */
          0x5E, 0x9C, 0x9D, 0xFA, 0x07, 0x07, 0x07, 0xAC,
 /* 0xB8        ----     [     ]  ----  ----  ----  ---- */
          0xAB, 0x07, 0x5B, 0x5D, 0x07, 0x07, 0x07, 0x07,
 /* 0xC0     {     A     B     C     D     E     F     G */
          0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 /* 0xC8     H     I  ----           ??              ---- */
          0x48, 0x49, 0x07, 0x93, 0x94, 0x95, 0xA2, 0x07,
 /* 0xD0     }     J     K     L     M     N     O     P */
          0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
 /* 0xD8     Q     R  ----           ??                   */
          0x51, 0x52, 0x07, 0x96, 0x81, 0x97, 0xA3, 0x98,
 /* 0xE0     \           S     T     U     V     W     X */
          0x5C, 0xF6, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 /* 0xE8     Y     Z        ----     ??  ----  ----  ---- */
          0x59, 0x5A, 0xFD, 0x07, 0x99, 0x07, 0x07, 0x07,
 /* 0xF0     0     1     2     3     4     5     6     7 */
          0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 /* 0xF8     8     9  ----  ----     ??  ----  ----  ---- */
          0x38, 0x39, 0x07, 0x07, 0x9A, 0x07, 0x07, 0x07
};


/*
 * ASCII (IBM PC 437)  -> EBCDIC 500
 */
__u8 _ascebc_500[256] =
{
 /*00 NUL   SOH   STX   ETX   EOT   ENQ   ACK   BEL */
     0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,
 /*08  BS    HT    LF    VT    FF    CR    SO    SI */
 /*              ->NL                               */
     0x16, 0x05, 0x15, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 /*10 DLE   DC1   DC2   DC3   DC4   NAK   SYN   ETB */
     0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,
 /*18 CAN    EM   SUB   ESC    FS    GS    RS    US */
 /*                               ->IGS ->IRS ->IUS */
     0x18, 0x19, 0x3F, 0x27, 0x22, 0x1D, 0x1E, 0x1F,
 /*20  SP     !     "     #     $     %     &     ' */
     0x40, 0x4F, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,
 /*28   (     )     *     +     ,     -    .      / */
     0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
 /*30   0     1     2     3     4     5     6     7 */
     0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
 /*38   8     9     :     ;     <     =     >     ? */
     0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
 /*40   @     A     B     C     D     E     F     G */
     0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
 /*48   H     I     J     K     L     M     N     O */
     0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
 /*50   P     Q     R     S     T     U     V     W */
     0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
 /*58   X     Y     Z     [     \     ]     ^     _ */
     0xE7, 0xE8, 0xE9, 0x4A, 0xE0, 0x5A, 0x5F, 0x6D,
 /*60   `     a     b     c     d     e     f     g */
     0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 /*68   h     i     j     k     l     m     n     o */
     0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 /*70   p     q     r     s     t     u     v     w */
     0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
 /*78   x     y     z     {     |     }     ~    DL */
     0xA7, 0xA8, 0xA9, 0xC0, 0xBB, 0xD0, 0xA1, 0x07,
 /*80*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*88*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*90*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*98*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*A0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*A8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*B0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*B8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*C0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*C8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*D0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*D8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*E0        sz						*/
     0x3F, 0x59, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*E8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*F0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*F8*/
     0x90, 0x3F, 0x3F, 0x3F, 0x3F, 0xEA, 0x3F, 0xFF
};

/*
 * EBCDIC 500 -> ASCII (IBM PC 437)
 */
__u8 _ebcasc_500[256] =
{
 /* 0x00   NUL   SOH   STX   ETX  *SEL    HT  *RNL   DEL */
          0x00, 0x01, 0x02, 0x03, 0x07, 0x09, 0x07, 0x7F,
 /* 0x08   -GE  -SPS  -RPT    VT    FF    CR    SO    SI */
          0x07, 0x07, 0x07, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 /* 0x10   DLE   DC1   DC2   DC3  -RES   -NL    BS  -POC
                                  -ENP  ->LF             */
          0x10, 0x11, 0x12, 0x13, 0x07, 0x0A, 0x08, 0x07,
 /* 0x18   CAN    EM  -UBS  -CU1  -IFS  -IGS  -IRS  -ITB
                                                    -IUS */
          0x18, 0x19, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
 /* 0x20   -DS  -SOS    FS  -WUS  -BYP    LF   ETB   ESC
                                  -INP                   */
          0x07, 0x07, 0x1C, 0x07, 0x07, 0x0A, 0x17, 0x1B,
 /* 0x28   -SA  -SFE   -SM  -CSP  -MFA   ENQ   ACK   BEL
                       -SW                               */ 
          0x07, 0x07, 0x07, 0x07, 0x07, 0x05, 0x06, 0x07,
 /* 0x30  ----  ----   SYN   -IR   -PP  -TRN  -NBS   EOT */
          0x07, 0x07, 0x16, 0x07, 0x07, 0x07, 0x07, 0x04,
 /* 0x38  -SBS   -IT  -RFF  -CU3   DC4   NAK  ----   SUB */
          0x07, 0x07, 0x07, 0x07, 0x14, 0x15, 0x07, 0x1A,
 /* 0x40    SP   RSP           ??              ----       */
          0x20, 0xFF, 0x83, 0x84, 0x85, 0xA0, 0x07, 0x86,
 /* 0x48                 [     .     <     (     +     ! */
          0x87, 0xA4, 0x5B, 0x2E, 0x3C, 0x28, 0x2B, 0x21,
 /* 0x50     &                                      ---- */
          0x26, 0x82, 0x88, 0x89, 0x8A, 0xA1, 0x8C, 0x07,
 /* 0x58           ??     ]     $     *     )     ;     ^ */
          0x8D, 0xE1, 0x5D, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
 /* 0x60     -     /  ----     ??  ----  ----  ----       */
          0x2D, 0x2F, 0x07, 0x8E, 0x07, 0x07, 0x07, 0x8F,
 /* 0x68              ----     ,     %     _     >     ? */ 
          0x80, 0xA5, 0x07, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
 /* 0x70  ----        ----  ----  ----  ----  ----  ---- */
          0x07, 0x90, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
 /* 0x78     *     `     :     #     @     '     =     " */
          0x70, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
 /* 0x80     *     a     b     c     d     e     f     g */
          0x07, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 /* 0x88     h     i              ----  ----  ----       */
          0x68, 0x69, 0xAE, 0xAF, 0x07, 0x07, 0x07, 0xF1,
 /* 0x90     ??     j     k     l     m     n     o     p */
          0xF8, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
 /* 0x98     q     r                    ----        ---- */
          0x71, 0x72, 0xA6, 0xA7, 0x91, 0x07, 0x92, 0x07,
 /* 0xA0           ~     s     t     u     v     w     x */
          0xE6, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 /* 0xA8     y     z              ----  ----  ----  ---- */
          0x79, 0x7A, 0xAD, 0xAB, 0x07, 0x07, 0x07, 0x07,
 /* 0xB0                          ----     ??  ----       */
          0x9B, 0x9C, 0x9D, 0xFA, 0x07, 0x07, 0x07, 0xAC,
 /* 0xB8        ----           |  ----  ----  ----  ---- */
          0xAB, 0x07, 0xAA, 0x7C, 0x07, 0x07, 0x07, 0x07,
 /* 0xC0     {     A     B     C     D     E     F     G */
          0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 /* 0xC8     H     I  ----           ??              ---- */
          0x48, 0x49, 0x07, 0x93, 0x94, 0x95, 0xA2, 0x07,
 /* 0xD0     }     J     K     L     M     N     O     P */
          0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
 /* 0xD8     Q     R  ----           ??                   */
          0x51, 0x52, 0x07, 0x96, 0x81, 0x97, 0xA3, 0x98,
 /* 0xE0     \           S     T     U     V     W     X */
          0x5C, 0xF6, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 /* 0xE8     Y     Z        ----     ??  ----  ----  ---- */
          0x59, 0x5A, 0xFD, 0x07, 0x99, 0x07, 0x07, 0x07,
 /* 0xF0     0     1     2     3     4     5     6     7 */
          0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 /* 0xF8     8     9  ----  ----     ??  ----  ----  ---- */
          0x38, 0x39, 0x07, 0x07, 0x9A, 0x07, 0x07, 0x07
};


/*
 * EBCDIC 037/500 conversion table:
 * from upper to lower case
 */
__u8 _ebc_tolower[256] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x48, 0x49, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	0x58, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9C, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
	0xA8, 0xA9, 0xAA, 0xAB, 0x8C, 0x8D, 0x8E, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
	0xA8, 0xA9, 0xEA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
	0xF8, 0xF9, 0xFA, 0xDB, 0xDC, 0xDD, 0xDE, 0xFF
};


/*
 * EBCDIC 037/500 conversion table:
 * from lower to upper case
 */
__u8 _ebc_toupper[256] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0x8A, 0x8B, 0xAC, 0xAD, 0xAE, 0x8F,
	0x90, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0x9A, 0x9B, 0x9E, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0xDA, 0xFB, 0xFC, 0xFD, 0xFE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

EXPORT_SYMBOL(_ascebc_500);
EXPORT_SYMBOL(_ebcasc_500);
EXPORT_SYMBOL(_ascebc);
EXPORT_SYMBOL(_ebcasc);
EXPORT_SYMBOL(_ebc_tolower);
EXPORT_SYMBOL(_ebc_toupper);

