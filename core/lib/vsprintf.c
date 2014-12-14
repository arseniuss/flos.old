/**
 * @file    lib/vsprintf.c   
 * @brief   String formatting
 * @version 1.0
 * @date    13.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/ctype.h>
#include <flos/defs.h>
#include <flos/types.h>
#include <flos/vaargs.h>
#include <flos/string.h>

enum {
    FORMAT_TYPE_NONE = 0,
    FORMAT_TYPE_WIDTH,
    FORMAT_TYPE_PRECISION,
    FORMAT_TYPE_CHAR,
    FORMAT_TYPE_STR,
    FORMAT_TYPE_PTR,
    FORMAT_TYPE_PERCENT_CHAR,
    FORMAT_TYPE_ULONG,
    FORMAT_TYPE_LONG,
    FORMAT_TYPE_LONG_LONG,
    FORMAT_TYPE_SIZE_T,
    FORMAT_TYPE_PTRDIFF,
    FORMAT_TYPE_UBYTE,
    FORMAT_TYPE_BYTE,
    FORMAT_TYPE_USHORT,
    FORMAT_TYPE_SHORT,
    FORMAT_TYPE_UINT,
    FORMAT_TYPE_INT,
    FORMAT_TYPE_INVALID,

};

#define FLAG_LEFT       1
#define FLAG_PLUS       2
#define FLAG_SPACE      4
#define FLAG_SPEC       8
#define FLAG_ZERO       16
#define FLAG_SMALL      32
#define FLAG_SIGN       64

struct printf_spec {
    u8 type;
    u8 flags;
    u16 width;
    u16 precision;
    u16 length;
    u16 base;
    u8 qualifier;
};

int skip_atoi(const char **s) {
    int i = 0;

    while(isdigit(**s))
        i = i * 10 + *((*s)++) - '0';

    return i;
}

int format_decode(const char *fmt, struct printf_spec *spec) {
    const char *start = fmt;

    /* we finished early by reading the field width */
    if(spec->type == FORMAT_TYPE_WIDTH) {
        if(spec->width < 0) {
            spec->width = -spec->width;
            spec->flags |= FLAG_LEFT;
        }
        spec->type = FORMAT_TYPE_NONE;
        goto precision;
    }

    /* we finished early by reading the precision */
    if(spec->type == FORMAT_TYPE_PRECISION) {
        if(spec->precision < 0)
            spec->precision = 0;
        spec->type = FORMAT_TYPE_NONE;
        goto qualifier;
    }

    /* By default */
    spec->type = FORMAT_TYPE_NONE;

    for(; *fmt; ++fmt) {
        if(*fmt == '%')
            break;
    }

    /* Return the current non-format string */
    if(fmt != start || !*fmt)
        return fmt - start;

    /* Process flags */
    spec->flags = 0;

    bool found = TRUE;
    do {
        ++fmt;

        switch(*fmt) {
            case '-': spec->flags |= FLAG_LEFT;
                break;
            case '+': spec->flags |= FLAG_PLUS;
                break;
            case ' ': spec->flags |= FLAG_SPACE;
                break;
            case '#': spec->flags |= FLAG_SPEC;
                break;
            case '0': spec->flags |= FLAG_ZERO;
                break;
            default: found = FALSE;
        }
    } while(!found);

    /* get field width */
    spec->width = -1;

    if(isdigit(*fmt)) {
        spec->width = skip_atoi(&fmt);
    } else if(*fmt == '*') {
        spec->type = FORMAT_TYPE_WIDTH;
        return ++fmt - start;
    }

precision:
    /* get the precision */
    spec->precision = -1;
    if(*fmt == '.') {
        ++fmt;
        if(isdigit(*fmt)) {
            spec->precision = skip_atoi(&fmt);
            if(spec->precision < 0)
                spec->precision = 0;
        } else if(*fmt == '*') {
            /* it's the next argument */
            spec->type = FORMAT_TYPE_PRECISION;
            return ++fmt - start;
        }
    }
qualifier:
    /* get the conversion qualifier */
    spec->qualifier = -1;
    if(*fmt == 'h' || _tolower(*fmt) == 'l' ||
       _tolower(*fmt) == 'z' || *fmt == 't') {
        spec->qualifier = *fmt++;
        if(unlikely(spec->qualifier == *fmt)) {
            if(spec->qualifier == 'l') {
                spec->qualifier = 'L';
                ++fmt;
            } else if(spec->qualifier == 'h') {
                spec->qualifier = 'H';
                ++fmt;
            }
        }
    }

    /* default base */
    spec->base = 10;
    switch(*fmt) {
        case 'c':
            spec->type = FORMAT_TYPE_CHAR;
            return ++fmt - start;

        case 's':
            spec->type = FORMAT_TYPE_STR;
            return ++fmt - start;

        case 'p':
            spec->type = FORMAT_TYPE_PTR;
            return fmt - start;
            /* skip alnum */

        case '%':
            spec->type = FORMAT_TYPE_PERCENT_CHAR;
            return ++fmt - start;

            /* integer number formats - set up the flags and "break" */
        case 'o':
            spec->base = 8;
            break;

        case 'x':
            spec->flags |= FLAG_SMALL;

        case 'X':
            spec->base = 16;
            break;

        case 'd':
        case 'i':
            spec->flags |= FLAG_SIGN;
        case 'u':
            break;

        default:
            spec->type = FORMAT_TYPE_INVALID;
            return fmt - start;
    }

    if(spec->qualifier == 'L')
        spec->type = FORMAT_TYPE_LONG_LONG;
    else if(spec->qualifier == 'l') {
        if(spec->flags & FLAG_SIGN)
            spec->type = FORMAT_TYPE_LONG;
        else
            spec->type = FORMAT_TYPE_ULONG;
    } else if(_tolower(spec->qualifier) == 'z') {
        spec->type = FORMAT_TYPE_SIZE_T;
    } else if(spec->qualifier == 't') {
        spec->type = FORMAT_TYPE_PTRDIFF;
    } else if(spec->qualifier == 'H') {
        if(spec->flags & FLAG_SIGN)
            spec->type = FORMAT_TYPE_BYTE;
        else
            spec->type = FORMAT_TYPE_UBYTE;
    } else if(spec->qualifier == 'h') {
        if(spec->flags & FLAG_SIGN)
            spec->type = FORMAT_TYPE_SHORT;
        else
            spec->type = FORMAT_TYPE_USHORT;
    } else {
        if(spec->flags & FLAG_SIGN)
            spec->type = FORMAT_TYPE_INT;
        else
            spec->type = FORMAT_TYPE_UINT;
    }

    return ++fmt - start;
}

static
char *put_dec_trunc8(char *buf, unsigned r) {
    unsigned q;

    /* Copy of previous function's body with added early returns */
    while(r >= 10000) {
        q = r + '0';
        r = (r * (u64) 0x1999999a) >> 32;
        *buf++ = q - 10 * r;
    }

    q = (r * 0x199a) >> 16; /* r <= 9999 */
    *buf++ = (r - 10 * q) + '0';
    if(q == 0)
        return buf;
    r = (q * 0xcd) >> 11; /* q <= 999 */
    *buf++ = (q - 10 * r) + '0';
    if(r == 0)
        return buf;
    q = (r * 0xcd) >> 11; /* r <= 99 */
    *buf++ = (r - 10 * q) + '0';
    if(q == 0)
        return buf;
    *buf++ = q + '0'; /* q <= 9 */
    return buf;
}

static
void put_dec_full4(char *buf, unsigned q) {
    unsigned r;
    r = (q * 0xccd) >> 15;
    buf[0] = (q - 10 * r) + '0';
    q = (r * 0xcd) >> 11;
    buf[1] = (r - 10 * q) + '0';
    r = (q * 0xcd) >> 11;
    buf[2] = (q - 10 * r) + '0';
    buf[3] = r + '0';
}

static
unsigned put_dec_helper4(char *buf, unsigned x) {
    u32 q = (x * (u64) 0x346DC5D7) >> 43;

    put_dec_full4(buf, x - q * 10000);
    return q;
}

static
char *put_dec(char *buf, unsigned long long n) {
    u32 d3, d2, d1, q, h;

    if(n < 100 * 1000 * 1000)
        return put_dec_trunc8(buf, n);

    d1 = ((u32) n >> 16); /* implicit "& 0xffff" */
    h = (n >> 32);
    d2 = (h) & 0xffff;
    d3 = (h >> 16); /* implicit "& 0xffff" */

    q = 656 * d3 + 7296 * d2 + 5536 * d1 + ((u32) n & 0xffff);
    q = put_dec_helper4(buf, q);

    q += 7671 * d3 + 9496 * d2 + 6 * d1;
    q = put_dec_helper4(buf + 4, q);

    q += 4749 * d3 + 42 * d2;
    q = put_dec_helper4(buf + 8, q);

    q += 281 * d3;
    buf += 12;
    if(q)
        buf = put_dec_trunc8(buf, q);
    else while(buf[-1] == '0')
            --buf;

    return buf;
}

char *number(char *buf, char *end, unsigned long long num,
             struct printf_spec spec) {
    /* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
    static const char digits[16] = "0123456789ABCDEF"; /* "GHIJKLMNOPQRSTUVWXYZ"; */

    char tmp[66];
    char sign;
    char locase;
    int need_pfx = ((spec.flags & FLAG_SPEC) && spec.base != 10);
    int i;
    bool is_zero = num == 0LL;

    /* locase = 0 or 0x20. ORing digits or letters with 'locase'
     * produces same digits or (maybe lowercased) letters */
    locase = (spec.flags & FLAG_SMALL);
    if(spec.flags & FLAG_LEFT)
        spec.flags &= ~FLAG_ZERO;
    sign = 0;
    if(spec.flags & FLAG_SIGN) {
        if((signed long long) num < 0) {
            sign = '-';
            num = -(signed long long) num;
            spec.width--;
        } else if(spec.flags & FLAG_PLUS) {
            sign = '+';
            spec.width--;
        } else if(spec.flags & FLAG_SPACE) {
            sign = ' ';
            spec.width--;
        }
    }
    if(need_pfx) {
        if(spec.base == 16)
            spec.width -= 2;
        else if(!is_zero)
            spec.width--;
    }

    /* generate full string in tmp[], in reverse order */
    i = 0;
    if(num < spec.base)
        tmp[i++] = digits[num] | locase;
        /* Generic code, for any base:
        else do {
                tmp[i++] = (digits[do_div(num,base)] | locase);
        } while (num != 0);
         */
    else if(spec.base != 10) { /* 8 or 16 */
        int mask = spec.base - 1;
        int shift = 3;

        if(spec.base == 16)
            shift = 4;
        do {
            tmp[i++] = (digits[((unsigned char) num) & mask] | locase);
            num >>= shift;
        } while(num);
    } else { /* base 10 */
        i = put_dec(tmp, num) - tmp;
    }

    /* printing 100 using %2d gives "100", not "00" */
    if(i > spec.precision)
        spec.precision = i;
    /* leading space padding */
    spec.width -= spec.precision;
    if(!(spec.flags & (FLAG_ZERO + FLAG_LEFT))) {
        while(--spec.width >= 0) {
            if(buf < end)
                *buf = ' ';
            ++buf;
        }
    }
    /* sign */
    if(sign) {
        if(buf < end)
            *buf = sign;
        ++buf;
    }
    /* "0x" / "" prefix */
    if(need_pfx) {
        if(spec.base == 16 || !is_zero) {
            if(buf < end)
                *buf = '0';
            ++buf;
        }
        if(spec.base == 16) {
            if(buf < end)
                *buf = ('X' | locase);
            ++buf;
        }
    }
    /* zero or space padding */
    if(!(spec.flags & FLAG_LEFT)) {
        char c = (spec.flags & FLAG_ZERO) ? '0' : ' ';
        while(--spec.width >= 0) {
            if(buf < end)
                *buf = c;
            ++buf;
        }
    }
    /* hmm even more zero padding? */
    while(i <= --spec.precision) {
        if(buf < end)
            *buf = '0';
        ++buf;
    }
    /* actual digits of result */
    while(--i >= 0) {
        if(buf < end)
            *buf = tmp[i];
        ++buf;
    }
    /* trailing space padding */
    while(--spec.width >= 0) {
        if(buf < end)
            *buf = ' ';
        ++buf;
    }

    return buf;
}

char *string(char *buf, char *end, const char *s, struct printf_spec spec) {
    int len, i;

    if((unsigned long) s < 4096) // TODO: replace with PAGE_SIZE
        s = "(null)";

    len = strnlen(s, spec.precision);

    if(!(spec.flags & FLAG_LEFT)) {
        while(len < spec.width--) {
            if(buf < end)
                *buf = ' ';
            ++buf;
        }
    }
    for(i = 0; i < len; ++i) {
        if(buf < end)
            *buf = *s;
        ++buf;
        ++s;
    }
    while(len < spec.width--) {
        if(buf < end)
            *buf = ' ';
        ++buf;
    }

    return buf;
}

char *pointer(const char *fmt, char *buf, char *end, void *ptr,
              struct printf_spec spec) {
    int default_width = 2 * sizeof (void *) + (spec.flags & FLAG_SPEC ? 2 : 0);

    if(!ptr) {
        /*
         * Print (null) with the same width as a pointer so it makes
         * tabular output look nice.
         */
        if(spec.width == -1)
            spec.width = default_width;
        return string(buf, end, "(null)", spec);
    }

    spec.flags |= FLAG_SMALL;
    if(spec.width == -1) {
        spec.width = default_width;
        spec.flags |= FLAG_ZERO;
    }
    spec.base = 16;

    return number(buf, end, (unsigned long) ptr, spec);
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args) {
    unsigned long long num;
    char *str, *end;
    struct printf_spec spec = {0};

    str = buf;
    end = buf + size;

    while(*fmt) {
        const char *old_fmt = fmt;
        int read = format_decode(fmt, &spec);

        fmt += read;

        switch(spec.type) {
            case FORMAT_TYPE_NONE:
            {
                int copy = read;
                if(str < end) {
                    if(copy > end - str)
                        copy = end - str;
                    memcpy(str, old_fmt, copy);
                }
                str += read;
                break;
            }
            case FORMAT_TYPE_WIDTH:
                spec.width = va_arg(args, int);
                break;
            case FORMAT_TYPE_PRECISION:
                spec.precision = va_arg(args, int);
                break;
            case FORMAT_TYPE_CHAR:
            {
                char c;

                if(!(spec.flags & FLAG_LEFT)) {
                    while(--spec.width > 0) {
                        if(str < end)
                            *str = ' ';
                        ++str;
                    }
                }
                c = (unsigned char) va_arg(args, int);
                if(str < end)
                    *str = c;
                while(--spec.width > 0) {
                    if(str < end)
                        *str = ' ';
                    ++str;
                }
                break;
            }

            case FORMAT_TYPE_STR:
                str = string(str, end, va_arg(args, char *), spec);
                break;

            case FORMAT_TYPE_PTR:
                str = pointer(fmt + 1, str, end, va_arg(args, void *), spec);
                while(isalnum(*fmt))
                    fmt++;
                break;
            case FORMAT_TYPE_PERCENT_CHAR:
                if(str < end)
                    *str = '%';
                ++str;
                break;

            case FORMAT_TYPE_INVALID:
                if(str < end)
                    *str = '%';
                ++str;
                break;
            default:
                switch(spec.type) {
                    case FORMAT_TYPE_LONG_LONG:
                        num = va_arg(args, long long);
                        break;
                    case FORMAT_TYPE_ULONG:
                        num = va_arg(args, unsigned long);
                        break;
                    case FORMAT_TYPE_LONG:
                        num = va_arg(args, long);
                        break;
                    case FORMAT_TYPE_SIZE_T:
                        if(spec.flags & FLAG_SIGN)
                            num = va_arg(args, ssize_t);
                        else
                            num = va_arg(args, size_t);
                        break;
                    case FORMAT_TYPE_PTRDIFF:
                        num = va_arg(args, ptrdiff_t);
                        break;
                    case FORMAT_TYPE_UBYTE:
                        num = (unsigned char) va_arg(args, int);
                        break;
                    case FORMAT_TYPE_BYTE:
                        num = (signed char) va_arg(args, int);
                        break;
                    case FORMAT_TYPE_USHORT:
                        num = (unsigned short) va_arg(args, int);
                        break;
                    case FORMAT_TYPE_SHORT:
                        num = (short) va_arg(args, int);
                        break;
                    case FORMAT_TYPE_INT:
                        num = (int) va_arg(args, int);
                        break;
                    default:
                        num = va_arg(args, unsigned int);
                }

                str = number(str, end, num, spec);
        }
    }

    if(size > 0) {
        if(str < end)
            *str = '\0';
        else
            end[-1] = '\0';
    }

    /* the trailing null byte doesn't count towards the total */
    return str - buf;
}

int sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsnprintf(buf, u32_max, fmt, args);
    va_end(args);

    return i;
}

int snprintf(char *buf, size_t size, const char *fmt, ...) {
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsnprintf(buf, size, fmt, args);
    va_end(args);

    return i;
}

int vsprintf(char *buf, const char *fmt, va_list args) {
    return vsnprintf(buf, u32_max, fmt, args);
}
