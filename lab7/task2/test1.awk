#! /usr/bin/awk -f

{
if (/bachelor/)
{count = count +1}
}
END {print "Number of Students: " count}