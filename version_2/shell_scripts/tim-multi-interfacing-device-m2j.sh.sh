#!/bin/bash
evtest_and_exit() {
        local evtest_pid
        evtest /dev/input/event$1 &
        evtest_pid=$!
        sleep 1
        kill $evtest_pid
}

#event name looking for
echo "Tim Pi Mouse added at $(date)" >>/tmp/scripts.log
EveName="Tim_Multiinterfacing_Device"

#find number of events
NumOfEve=$(ls /dev/input | grep -c event*)
echo "Number of events: $NumOfEve" >>/tmp/scripts.log

#launch xboxdrv event mapping
i=0
while [ $i -lt $NumOfEve ]
do
	echo "Testing event$i" >>/tmp/scripts.log
	OccurrencesOfName=$(evtest_and_exit $i | grep -c "$EveName")
	if [ $OccurrencesOfName -gt 0 ]
	then
		echo "Tim Pi Mouse found" >>/tmp/scripts.log
		xboxdrv --evdev /dev/input/event$i \
		--mimic-xpad \
		--dpad-as-button \
		--ui-axismap REL_Y=du,-REL_Y=dd,-REL_X=dl,REL_X=dr
		--evdev-keymap BTN_LEFT=a,BTN_RIGHT=b, \
		--silent --quiet
	fi
	i=`expr $i + 1`
done
