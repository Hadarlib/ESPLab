#! /usr/bin/awk -f

BEGIN { FS = ","}

{
if ($0 ~/\<f[au]n\>/){
count = count +1
}
}

END {print "The number of poems that have fan or fun is: " count}