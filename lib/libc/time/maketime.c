/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\time\maketime.c
    @brief Time and Date functions
*/

/** @brief Seconds in a minute */
#define MINUTE	60
/** @brief Seconds in an hour */
#define HOUR	(60 * MINUTE)
/** @brief Seconds in an day */
#define DAY		(24 * HOUR)
/** @brief Seconds in an year */
#define YEAR	(365 * DAY)

/** @brief Seconds in the 12 months */
static signed long int Month[12] =
{
	0,
	DAY * ( 31 ),
	DAY * ( 31 + 29 ),
	DAY * ( 31 + 29 + 31 ),
	DAY * ( 31 + 29 + 31 + 30 ),
	DAY * ( 31 + 29 + 31 + 30 + 31 ),
	DAY * ( 31 + 29 + 31 + 30 + 31 + 30 ),
	DAY * ( 31 + 29 + 31 + 30 + 31 + 30 + 31),
	DAY * ( 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
	DAY * ( 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
	DAY * ( 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 ),
	DAY * ( 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 )
};

signed long int maketime(unsigned long int year, unsigned long int month,
                         unsigned long int day, unsigned long int hour,
                         unsigned long int minute, unsigned long int second)
{
	signed long int res, year_int;

	year_int = year - 70 - 1900;

	res = YEAR * year_int + DAY * ((year_int + 1)/4);
	res += Month[month - 1];
	if(((month - 1) > 1) && ((year_int + 2)%4))
		 res -= DAY;

	res += DAY * (day - 1);
	res += HOUR * hour;
	res += MINUTE * minute;
	res += second;

	return res;
}
