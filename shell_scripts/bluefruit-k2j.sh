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
EveName="Bluefruit52 Keyboard"

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
		--evdev-keymap KEY_U=a,KEY_I=b,KEY_J=x,KEY_K=y,KEY_O=rt,KEY_Y=lt,KEY_L=rb,KEY_H=lb \
		--mimic-xpad --silent --quiet
	fi
	i=`expr $i + 1`
done
