#!/bin/bash
# converts "old" syntax used by vdr with plugin-param-patch
# to new syntax needed for vdr >= 1.7.13
# based on channel-conv.sh by wirbel
# --mini73 20100331

if [ -z "$2" ]   # expecting two args
then
  echo "Usage: `basename $0` channels.conf.old channels.conf"
  echo " "
  echo "converts existing pvrinput channels.conf (plugin-param-patch) to brand new channels.conf syntax (vdr >= 1.7.13)."
  echo " * first arg  is path to old channels.conf"
  echo " * second arg is path to new channels.conf"
  echo " "
  echo "-- MISSING ARGUMENTS. EXITING NOW. --"
  exit -1
else
  OLDFILE=$1
  NEWFILE=$2
fi

while IFS=: read name freq param source srate vpid apid tpid caid sid nid tid rid
do
  if [ -z "$name" ]; then
    echo ":"$freq"" >> $NEWFILE
    continue
  fi

  # split param at first |
  pluginname=${param%%|*}
  newparam=${param#*|}

  if [[ "$source" == "P" ]] && [[ "$pluginname" == "PVRINPUT" ]]; then
    if [[ `expr index "$newparam" "RADIO"` -eq 1 ]]; then
      newvpid=$vpid
    else
      # split vpid in vpid, pcrpid and streamtype
      if [[ `expr index "$vpid" "="` -gt 0 ]]; then
        vpcrpid=${vpid%%=*}
        vtype=${vpid#*=}
      else
        vpcrpid=$vpid
        vtype=2
      fi
      if [[ `expr index "$vpcrpid" "\+"` -gt 0 ]]; then
        realvpid=${vpcrpid%%+*}
        pcrpid=${vpcrpid#*+}
      else
        realvpid=$vpcrpid
        pcrpid=101
      fi
      newvpid=$realvpid+$pcrpid=$vtype
    fi

    # if current sid is already 1 keep old tid
    if [[ "$sid" == "1" ]]; then
      sid=$tid
    fi

    # omit PVRINPUT from parameter, set source to V, set SID to 1 and old SID to TID
    echo "$name:$freq:$newparam:V:$srate:$newvpid:$apid:$tpid:$caid:1:$nid:$sid:$rid" >> $NEWFILE
  else
    # non-pvrinput channel; copy n paste it
    echo "$name:$freq:$param:$source:$srate:$vpid:$apid:$tpid:$caid:$sid:$nid:$tid:$rid" >> $NEWFILE
  fi
done < $OLDFILE   # I/O redirection.

