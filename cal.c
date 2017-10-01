/***********************************************************************
*
* cal.c - an appointment/birthday/etc reminder utility
*
* Written by Graham Wheeler, December 1989
*
* This program may be freely copied, modified, distributed, etc,
* provided that this header comment is left intact, and that any
* enhancements are commented and credited.
*
* USAGE:
*	This program scans a file called `cal.dat' which should
*	contain a number of text lines. Each line should be of the
*	form `day/month text'; for example:
*
* 10/5 Graham Wheeler's birthday
*
*	The program takes an argument on the command line, which
*	is the number of days ahead that it should scan. All events
*	occuring within that number of days will be printed. The
*	program doesn't care about leap years, but does work
*	intelligently over month and year wraps. I use it in my
*	AUTOEXEC.BAT file to remind me of everything that is scheduled
*	for the next week.
*
*	Written in TurboC v2.0 under MS-DOS 3.3
**********************************************************************/

#include <stdio.h>
#include <dos.h>

const int       monthdays[] =
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

main(int argc, char *argv[])
{
    FILE           *fp;
    if ((fp = fopen("c:\\cal.dat", "rt")) == NULL)
    {
	fprintf(stderr, "cal: cannot open cal.dat\n");
	exit(0);
    }
    else
    {
	int             limit, lastday, lastmonth, firstday, firstmonth;
	struct date     datestr;

	if (argc == 2)
	    limit = -atoi(argv[1]);
	else if (argc != 1)
	{
	    fprintf(stderr, "Usage: cal [-<number of days>]\n");
	    exit(0);
	}
	else
	    limit = 0;
	getdate(&datestr);
	firstday = lastday = datestr.da_day;
	firstmonth = lastmonth = datestr.da_mon;
	lastday += limit;
	while (lastday > monthdays[lastmonth])
	{
	    lastday -= monthdays[lastmonth];
	    lastmonth++;
	    if (lastmonth > 12)
		lastmonth = 1;
	}

	while (!feof(fp))
	{
	    char            buff[80];
	    int             fmt;
	    fgets(buff, 80, fp);
	    if (buff[0] >= '0' && buff[0] <= '9')
	    {
		int             day, month;
		char           *msg;
		sscanf(buff, "%d/%d", &day, &month);
		msg = buff;
		while (*msg++ != ' ') ;
		if (month == firstmonth && day == firstday)
		    fmt = 1;
		else if (month == firstmonth && day == firstday + 1)
		    fmt = 2;
		else if (month == firstmonth + 1 && firstday == monthdays[firstmonth] && day == 1)
		    fmt = 2;
		else if (month == firstmonth && day > firstday)
		    fmt = 3;
		else if (firstmonth != lastmonth)
		{
		    if (month == lastmonth && day <= lastday)
			fmt = 3;
		    else if (month > firstmonth && month < lastmonth && firstmonth < lastmonth)
			fmt = 3;
		    else if ((month < lastmonth || month > firstmonth) && firstmonth > lastmonth)
			fmt = 3;
		    else
			fmt = 0;
		}
		else
		    fmt = 0;
		switch (fmt)
		{
		case 1:
		    printf("TODAY: %s", msg);
		    break;
		case 2:
		    printf("TOMORROW: %s", msg);
		    break;
		case 3:
		    printf("%d/%d: %s", day, month, msg);
		default:
		    break;
		}
	    }
	}
	fclose(fp);
    }
}
