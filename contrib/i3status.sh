#!/bin/bash
# 
# Extends i3status' output with 'akd -p' output.
# Make sure to have output_format unset in i3status' config or set to none.
#
# For indication to work correctly, edit your akd command here, don't start
# another instance of akd.

layout=''
i3status=''

{ i3status & akd -p
} | while read -r line
do
    if [[ $line == [a-z][a-z] ]]; then
        layout=$line
    else
        i3status=$line
    fi
    printf "%s | %s\n" "$layout" "$i3status"
done

pkill -P $$
