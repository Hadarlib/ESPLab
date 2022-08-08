#! /usr/bin/awk -f

{
if (/English/)
{count = count +1}
}
END {print "Count: " count}