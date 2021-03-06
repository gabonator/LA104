/*
	Copyright 2001, 2002 Georges Menie (www.menie.org)
	stdarg version contributed by Christian Ettinger

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
	putchar is the only external dependency for this file,
	if you have a working putchar, leave it commented out.
	If not, uncomment the define below and
	replace outbyte(c) by your own function call.

#define putchar(c) outbyte(c)
*/

#include <stdarg.h>

void sfp_printchar(char **str, int c)
{
	//extern int putchar(int c);
	
	if (str) {
		**str = c;
		++(*str);
	}
	//else (void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PAD_DECIMALS 4

#if defined(EMSCRIPTEN)
// Required by older version of emscripten
// #define va_list SPF::va_list
#endif
#if defined(EMSCRIPTEN) || defined(__APPLE__)
#define register
#endif

int sfp_prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			sfp_printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		sfp_printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		sfp_printchar (out, padchar);
		++pc;
	}

	return pc;
}


/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

int sfp_printf(char **out, float f, int width)
{
	char print_buf[PRINT_BUF_LEN];
	char* pb = print_buf;
	if (f<0)
		*pb++ = '-';
	
	if ( f < 0 )
		f = -f;

	int nNumber = (int)f;

	int nBase = 1;
	while ( nNumber >= nBase*10 )
		nBase *= 10;

	while ( nBase >= 1 )
	{
		int digit = nNumber / nBase;
		*pb++ = '0' + digit;
		nNumber -= digit * nBase;
		nBase /= 10;
	}
	*pb++ = '.';
	if ( width == 0 )
		width = 3;

	for (int i=0; i<width; i++)
	{
		f -= (int)f;
		f *= 10.0f;
		*pb++ = '0' + (int)f;
	}
	*pb = 0;
	return sfp_prints (out, print_buf, 0, ' ');
}

int sfp_printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return sfp_prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			sfp_printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + sfp_prints (out, s, width, pad);
}

int sfp_printf_decimals(char **out, float f, int width)
{
	int pc = 0;
	if (f < 0)
	{
		pc += sfp_prints(out, "-", 0, 0);
		f = -f;
	}
	pc += sfp_printi(out, (int)f, 10, false, 0, 0, 'a');
	pc += sfp_prints(out, ".", 0, 0);
	f -= (int)f;
	for (int i=0; i<width; i++)
		f *= 10.0f;

	pc += sfp_printi(out, (int)f, 10, false, width, PAD_ZERO, 'a');
	return pc;
}

int vsprintf(char *out_, const char *format, va_list args )
{
	register int width, pad;
	register int pc = 0;
	char scr[2];
	char** out = &out_;

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			if (*format == '.')
			{
				++format;
				pad |= PAD_DECIMALS;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, long );
				pc += sfp_prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += sfp_printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'f' ) {
#ifdef _ARM
				// TODO: check if we can avoid using "double"
				float f = (float)va_arg( args, double );
#else
				float f = (float)va_arg( args, double );
#endif

				if (pad & PAD_DECIMALS)
					pc += sfp_printf_decimals(out, f, width);
				else
					pc += sfp_printf (out, f, width);
				continue;
			}
			if( *format == 'x' ) {
				pc += sfp_printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += sfp_printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += sfp_printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += sfp_prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			sfp_printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	va_end( args );
	return pc;
}

/*
int printf(const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return print( 0, format, args );
}
*/
int sprintf(char *out, const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return vsprintf( out, format, args );
}

#ifdef TEST_PRINTF
int main(void)
{
	char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int)*8;
	int mi;
	char buf[80];

	mi = (1 << (bs-1)) + 1;
	printf("%s\n", ptr);
	printf("printf test\n");
	printf("%s is null pointer\n", np);
	printf("%d = 5\n", i);
	printf("%d = - max int\n", mi);
	printf("char %c = 'a'\n", 'a');
	printf("hex %x = ff\n", 0xff);
	printf("hex %02x = 00\n", 0);
	printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	printf("%d %s(s)%", 0, "message");
	printf("\n");
	printf("%d %s(s) with %%\n", 0, "message");
	sprintf(buf, "justif: \"%-10s\"\n", "left"); printf("%s", buf);
	sprintf(buf, "justif: \"%10s\"\n", "right"); printf("%s", buf);
	sprintf(buf, " 3: %04d zero padded\n", 3); printf("%s", buf);
	sprintf(buf, " 3: %-4d left justif.\n", 3); printf("%s", buf);
	sprintf(buf, " 3: %4d right justif.\n", 3); printf("%s", buf);
	sprintf(buf, "-3: %04d zero padded\n", -3); printf("%s", buf);
	sprintf(buf, "-3: %-4d left justif.\n", -3); printf("%s", buf);
	sprintf(buf, "-3: %4d right justif.\n", -3); printf("%s", buf);

	return 0;
}

/*
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 */

#endif

