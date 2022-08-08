#! /usr/bin/awk -f

BEGIN {FS = ","
}

{
if ($4 == "standard")
{
countMath  += $6
countReading += $7
countWriting += $8
num = num + 1
}
}

END {print "Math average: " countMath/num "\n" "Reading average: " countReading/num "\n" "writing average: " countWriting/num}