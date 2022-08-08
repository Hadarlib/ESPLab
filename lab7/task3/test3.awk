#! /usr/bin/awk -f

BEGIN { FS = ","}

{
if ($2 ~/happy/)
print $2 " --- " $3 " --- " $4
}