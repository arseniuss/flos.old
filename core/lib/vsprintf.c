/**
 * @file    lib/vsprintf.c
 * @brief   String formatting
 * @version 1.0
 * @date    13.12.2014.
 *
 * @author  Lars Wirzenius & Linus Torvalds et al.
 * @source  http://lxr.free-electrons.com/source/lib/vsprintf.c [2014]
 * @copyright GNU GPL v2
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright GNU GPL v2
 * 
 * TODO: Despite Linux and staff™ this code have a lot of bug. Rewrite ot debug!
 */

#include <flos/ctype.h>
#include <flos/defs.h>
#include <flos/kprintf.h>
#include <flos/string.h>
#include <flos/types.h>
#include <flos/vaargs.h>

#include <flos/assert.h>

#define FLAG_LEFT           1          /* Left justify */
#define FLAG_ZERO           2          /* Fill with zeros */
#define FLAG_SPACE          4          /* use space as number's + sign */
#define FLAG_PLUS           8          /* always show number's sign */
#define FLAG_SIGN           16         /* it is signed number */
#define FLAG_SMALL          32         /* use lowercase in hex (must be 32 = 0x20) */
#define FLAG_SPEC           64

enum format_type {
    FORMAT_TYPE_NONE,           /* Just a string part */
    FORMAT_TYPE_WIDTH,
    FORMAT_TYPE_PRECISION,
    FORMAT_TYPE_CHAR,
    FORMAT_TYPE_STR,
    FORMAT_TYPE_PTR,
    FORMAT_TYPE_PERCENT_CHAR,
    FORMAT_TYPE_INVALID,
    FORMAT_TYPE_LONG_LONG,
    FORMAT_TYPE_ULONG,
    FORMAT_TYPE_LONG,
    FORMAT_TYPE_UBYTE,
    FORMAT_TYPE_BYTE,
    FORMAT_TYPE_USHORT,
    FORMAT_TYPE_SHORT,
    FORMAT_TYPE_UINT,
    FORMAT_TYPE_INT,
    FORMAT_TYPE_SIZE_T,
    FORMAT_TYPE_PTRDIFF
};

struct printf_spec {
    u8 type;
    u8 base;
    u8 flags;
    s16 width;
    s16 precision;
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

    /*
     * we finished early by reading the field width
     */
    if(spec->type == FORMAT_TYPE_WIDTH) {
        if(spec->width < 0) {
            spec->width = -spec->width;
            spec->flags |= FLAG_LEFT;
        }
        spec->type = FORMAT_TYPE_NONE;
        goto precision;
    }

    /*
     * we finished early by reading the precision
     */
    if(spec->type == FORMAT_TYPE_PRECISION) {
        if(spec->precision < 0)
            spec->precision = 0;

        spec->type = FORMAT_TYPE_NONE;
        goto qualifier;
    }

    spec->type = FORMAT_TYPE_NONE;

    for(; *fmt; ++fmt) {
        if(*fmt == '%')
            break;
    }

    /*
     * Return the current non-format string
     */
    if(!*fmt || fmt != start)
        return fmt - start;

    /*
     * Process flags
     */
    spec->flags = 0;

    bool found;
    do {
        found = TRUE;

        ++fmt;

        switch (*fmt) {
            case '-':
                spec->flags |= FLAG_LEFT;
                break;
            case '+':
                spec->flags |= FLAG_PLUS;
                break;
            case ' ':
                spec->flags |= FLAG_SPACE;
                break;
            case '#':
                spec->flags |= FLAG_SPEC;
                break;
            case '0':
                spec->flags |= FLAG_ZERO;
                break;
            default:
                found = FALSE;
        }
    } while(found);

    /*
     * get field width
     */
    spec->width = -1;

    if(isdigit(*fmt))
        spec->width = skip_atoi(&fmt);
    else if(*fmt == '*') {
        /*
         * it's the next argument
         */
        spec->type = FORMAT_TYPE_WIDTH;
        return ++fmt - start;
    }

  precision:
    /*
     * get the precision
     */
    spec->precision = -1;
    if(*fmt == '.') {
        ++fmt;
        if(isdigit(*fmt)) {
            spec->precision = skip_atoi(&fmt);
            if(spec->precision < 0)
                spec->precision = 0;
        } else if(*fmt == '*') {
            /*
             * it's the next argument
             */
            spec->type = FORMAT_TYPE_PRECISION;
            return ++fmt - start;
        }
    }

  qualifier:
    /*
     * get the conversion qualifier
     */
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

    /*
     * default base
     */
    spec->base = 10;
    switch (*fmt) {
        case 'c':
            spec->type = FORMAT_TYPE_CHAR;
            return ++fmt - start;

        case 's':
            spec->type = FORMAT_TYPE_STR;
            return ++fmt - start;
        case 'p':
            spec->flags |= FLAG_SMALL;
        case 'P':
            spec->flags |= FLAG_SPEC;
            spec->type = FORMAT_TYPE_PTR;
            return ++fmt - start;

        case '%':
            spec->type = FORMAT_TYPE_PERCENT_CHAR;
            return ++fmt - start;

            /*
             * integer number formats - set up the flags and "break"
             */
        case 'b':
            spec->flags |= FLAG_SMALL;
        case 'B':
            spec->base = 2;
            break;
        case 'o':
            spec->flags |= FLAG_SMALL;
        case 'O':
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

char *number(char *buf, char *end, unsigned long long num,
             struct printf_spec spec) {
    static const char digits[16] = "0123456789ABCDEF";
    char tmp[66];

    char sign;                  /* Sign before number */
    char *no;                   /* Points to number in ASCII form */
    int width;                  /* Width of the number */
    char locase = (spec.flags & FLAG_SMALL);
    int need_prefix = ((spec.flags & FLAG_SPEC) && spec.base != 10);

    /*
     * TODO: We are using simplified version so needed padding is not printed 
     */

    /*
     * Determent sign printing
     */
    sign = 0;
    if(spec.flags & FLAG_SIGN) {
        if((signed long)num < 0) {
            sign = '-';
            num = -(signed long)num;
            spec.width--;
        } else if(spec.flags & FLAG_PLUS) {
            sign = '+';
            spec.width--;
        } else if(spec.flags & FLAG_SPACE) {
            sign = ' ';
            spec.width--;
        }
    }

    /*
     * Prefix
     */
    if(need_prefix) {
        spec.width -= 2;
    }

    /*
     * Convert number to ASCII form
     */
    no = &tmp[65];
    *no = '\0';
    do {
        unsigned long long temp;

        no--;
        width++;
        temp = num % spec.base;
        *no = digits[temp] | locase;
        num = num / spec.base;
    } while(num != 0);

    /*
     * Sign
     */
    if(sign) {
        if(buf < end)
            *buf = sign;
        ++buf;
    }

    /*
     * Prefix
     */
    if(need_prefix) {
        if(buf < end)
            *buf = '0';
        ++buf;
        if(buf < end)
            switch (spec.base) {
                case 2:
                    *buf = 'b';
                    break;
                case 8:
                    *buf = 'o';
                    break;
                case 16:
                    *buf = 'x';
                    break;
            }
        ++buf;
    }

    /*
     * Buffer with number
     */
    for(; *no; ++no) {
        if(buf < end)
            *buf = *no;
        ++buf;
    }

    return buf;
}

char *string(char *buf, char *end, const char *s, struct printf_spec spec) {
    int len, i;

    if((unsigned long)s < 4096)        //TODO: replace 4096 with PAGE_SIZE
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
    int default_width = sizeof(void *) + (spec.flags & FLAG_SPEC ? 2 : 0);

    if(spec.width == -1) {
        spec.width = default_width;
        spec.flags |= FLAG_ZERO;
    }
    spec.base = 16;
    spec.precision = 0;

    return number(buf, end, (unsigned long)ptr, spec);
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args) {
    unsigned long long num;
    struct printf_spec spec = { 0 };
    char *str = buf;
    char *end = buf + size;

    while(*fmt) {
        const char *old_fmt = fmt;
        int read = format_decode(fmt, &spec);

        fmt += read;

        switch (spec.type) {
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
                c = (unsigned char)va_arg(args, int);
                if(str < end)
                    *str = c;
                ++str;
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


                /*
                 * Decimals
                 */
            default:
                switch (spec.type) {
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
                        num = (unsigned char)va_arg(args, int);
                        break;
                    case FORMAT_TYPE_BYTE:
                        num = (signed char)va_arg(args, int);
                        break;
                    case FORMAT_TYPE_USHORT:
                        num = (unsigned short)va_arg(args, int);
                        break;
                    case FORMAT_TYPE_SHORT:
                        num = (short)va_arg(args, int);
                        break;
                    case FORMAT_TYPE_INT:
                        num = (int)va_arg(args, int);
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

    return str - buf;
}

int sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsnprintf(buf, MAX_CHAR_BUF_LEN, fmt, args);
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
