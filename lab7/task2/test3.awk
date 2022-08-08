#! /usr/bin/awk -f

BEGIN { FS = ","
print "==========="
print "Success Student List"
print "===========\n"
}

{
if ($6 >=80 && $7>=80 && $8>=80)
{
print $0
num = num +1
}
}


END {print "The number of students: " num}