#!/bin/bash
evtest_and_exit() {
        local evtest_pid
        evtest /dev/input/event$1 &
        evtest_pid=$!
        sleep 1
        kill $evtest_pid
}

#event name looking for
echo "Bluefruit Keyboard added at $(date)" >>/tmp/scripts.log
EveName="Bluefruit52_Joy Keyboard"

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
		echo "Bluefruit Keyboard found" >>/tmp/scripts.log
		xboxdrv --evdev /dev/input/event$i \
		--mimic-xpad \
		--dpad-as-button \
		--evdev-keymap KEY_J=a,KEY_K=b,KEY_L=x,KEY_U=y,KEY_W=du,KEY_S=dd,KEY_A=dl,KEY_D=dr \
		--silent --quiet
	fi
	i=`expr $i + 1`
done
