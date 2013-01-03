/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\sys\int64.c
    @brief Library private functions
*/

/** @fn static void __do_udiv64(unsigned long long numerator, unsigned long long denominator, unsigned long long *quotient, unsigned long long *remainder)
    @brief Does an unsigned long long division
    @ingroup libs_libc_sys 
    @param numerator Numerator
    @param denominator Denominator
    @param quotient Quotient
    @param remainder Remainder
    @internal
*/
static void __do_udiv64(unsigned long long numerator, unsigned long long denominator, 
                        unsigned long long *quotient, unsigned long long *remainder)
{
    unsigned long long n, d, x, answer;

    if(denominator == 0)
	     n = 0 / 0; // intentionally do a division by zero
    for(n = numerator, d = denominator, x = 1; n >= d && ((d & 0x8000000000000000) == 0); x <<= 1, d <<= 1)
         ;
    for(answer = 0; x != 0; x >>= 1, d >>= 1) {
	     if(n >= d) {
	          n -= d;
	          answer |= x;
         }
    }
    *quotient = answer;
    *remainder = n;
}
/** @fn static void __do_div64(long long numerator, long long denominator, long long *quotient, long long *remainder)
    @brief Does a long long division
    @ingroup libs_libc_sys 
    @param numerator Numerator
    @param denominator Denominator
    @param quotient Quotient
    @param remainder Remainder
    @internal
*/
static void __do_div64(long long numerator, long long denominator, 
                       long long *quotient, long long *remainder)
{
    long long n, d, x, answer;

    if(denominator == 0)
	     n = 0 / 0; // intentionally do a division by zero
    for(n = numerator, d = denominator, x = 1; n >= d && ((d & 0x8000000000000000) == 0); x <<= 1, d <<= 1)
	     ;
    for(answer = 0; x != 0; x >>= 1, d >>= 1) {
	     if(n >= d) {
	          n -= d;
	          answer |= x;
         }
    }
    *quotient = answer;
    *remainder = n;
}
/** @fn unsigned long long __udivdi3(unsigned long long numerator, unsigned long long denominator)
    @brief Does an unsigned long long division, returns the quotient
    @ingroup libs_libc_sys 
    @param numerator Numerator
    @param denominator Denominator
    @return Quotient
    @internal
*/
unsigned long long __udivdi3(unsigned long long numerator, unsigned long long denominator)
{
    unsigned long long quotient, remainder;
    __do_udiv64(numerator, denominator, &quotient, &remainder);
    return quotient;
}
/** @fn unsigned long long __umoddi3(unsigned long long numerator, unsigned long long denominator)
    @brief Does an unsigned long long division, returns the remainder 
    @ingroup libs_libc_sys 
    @param numerator Numerator
    @param denominator Denominator
    @return Remainder
    @internal
*/
unsigned long long __umoddi3(unsigned long long numerator, unsigned long long denominator)
{
    unsigned long long quotient, remainder;
    __do_udiv64(numerator, denominator, &quotient, &remainder);
    return remainder;
}
/** @fn long long __divdi3(long long numerator, long long denominator)
    @brief Does a long long division, returns the quotient
    @ingroup libs_libc_sys 
    @param numerator Numerator
    @param denominator Denominator
    @return Quotient
    @internal
*/
long long __divdi3(long long numerator, long long denominator)
{
    long long quotient, remainder;
    __do_div64(numerator, denominator, &quotient, &remainder);
    return quotient;
}
