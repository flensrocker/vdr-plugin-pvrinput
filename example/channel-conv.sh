#!/bin/bash
# new syntax needs vdr with iptv patch!
# have phun, --wirbel 20090526

if [ -z "$2" ]   # expecting two args
then
  echo "Usage: `basename $0` channels.conf.old channels.conf"
  echo " "
  echo "converts existing pvrinput channels.conf to new channels.conf syntax."
  echo " * first arg  is path to old channels.conf"
  echo " * second arg is path to new channels.conf"
  echo " "
  echo "-- MISSING ARGUMENTS. EXITING NOW. --"
  exit -1
else
  OLDFILE=$1
  NEWFILE=$2
fi


NUM=0
nsid=0
need_ext_inputs=0

while IFS=: read name freq param source srate vpid apid tpid caid sid nid tid rid
do
  if [ -z "$name" ]; then
    echo ":"$freq"" >> $NEWFILE
    continue
  fi

  if [[ "$caid" == "A1" ]]; then

    if (( freq < 10000 )); then
      # ext input; *impossible* to convert. skip them now and add new(!) entries at the end of conf.
      need_ext_inputs=1
      continue
    fi

    let "nsid=(500 + freq*16)/1000"
    echo -n "$name:$freq:PVRINPUT|" >> $NEWFILE
    if (( freq < 87500 )) || (( freq > 108000 )); then
       echo -n "TV" >> $NEWFILE
       norm="PAL"
       vtest=$(echo $param | grep "I0")
       if [[ ! -z "$vtest" ]]; then
         norm="NTSC"
       fi
       vtest=$(echo $param | grep "I1")
       if [[ ! -z "$vtest" ]]; then
         norm="SECAM"
       fi
       echo -n "|$norm:P:0:301:300:305:0:" >> $NEWFILE
    else
       echo -n "RADIO:P:0:0:300:0:0:" >> $NEWFILE
    fi
    echo "$nsid:$nid:$tid:$rid" >> $NEWFILE

  else
    # non-pvrinput channel; copy n paste it
    echo "$name:$freq:$param:$source:$srate:$vpid:$apid:$tpid:$caid:$sid:$nid:$tid:$rid" >> $NEWFILE
  fi

  NUM=$NUM+1
done < $OLDFILE   # I/O redirection.


# create ext inputs if needed; no scanning - just a dumb adding all of them.
if (( $need_ext_inputs == 1 )); then
  ext_sid=9000
  echo -n ":ext analog inputs" >> $NEWFILE && echo "" >> $NEWFILE
  for (( i=0; i<=4; i++ )); do
     echo "Composite $i;pvrinput:1:PVRINPUT|COMPOSITE$i:P:0:301:300:0:0:$ext_sid:0:0:0" >> $NEWFILE
     let "ext_sid=ext_sid+1"
  done
  for (( i=0; i<=3; i++ )); do
     echo "S-Video $i;pvrinput:1:PVRINPUT|SVIDEO$i:P:0:301:300:0:0:$ext_sid:0:0:0" >> $NEWFILE
     let "ext_sid=ext_sid+1"
  done
fi
 
