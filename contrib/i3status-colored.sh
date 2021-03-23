#!/bin/bash
#
# Extends i3status' output with 'akd -p' output. Supports colors.
# Make sure to have output_format set to i3bar in i3status config's general
# section.
#
# For indication to work correctly, edit your akd command here, don't start
# another instance of akd.

FORMAT=',[{"full_text":"%s"},%s\n'
layout=''
i3status=''

{ i3status & sleep 0.5; akd -p
} | while read -r line
do
    case "$line" in
         ,\[{*) i3status=$line; printf "$FORMAT" "$layout" "${i3status#,\[}" ;;
    [a-z][a-z]) layout=$line;   printf "$FORMAT" "$layout" "${i3status#,\[}" ;;
          \[{*) i3status=,$line ;&
             *) printf "%s\n" "$line" ;;
    esac
done

pkill -P $$
