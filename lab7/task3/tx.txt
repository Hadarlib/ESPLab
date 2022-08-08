#! /usr/bin/awk -f

BEGIN { FS = ","}

{
n = split($2, array, " ")
if (n <3)
{
num = num +1
}
}

END {print num}