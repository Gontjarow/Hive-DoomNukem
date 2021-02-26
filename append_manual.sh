#!/bin/sh
envos=`uname -s`
binsize=`ls -la doom-nukem | awk '{print $5}'`
targetsize=11000000
difference="$(($targetsize-$binsize))"
zero=0
if [ "$difference" -gt "$zero" ]; then
  echo "Appending to binary!"
  $filemaker -n $difference padfile
  cat padfile >> doom-nukem
  rm padfile
  cat img/robo/robo_atlas.xpm >> doom-nukem
  cat img/thunder/0.xpm >> doom-nukem
  cat img/thunder/1.xpm >> doom-nukem
  cat img/thunder/2.xpm >> doom-nukem
  cat img/thunder/3.xpm >> doom-nukem
  cat img/thunder/4.xpm >> doom-nukem
  cat img/thunder/5.xpm >> doom-nukem
  cat img/thunder/6.xpm >> doom-nukem
  cat img/thunder/7.xpm >> doom-nukem
  cat img/thunder/8.xpm >> doom-nukem
  cat img/thunder/9.xpm >> doom-nukem
else
  echo "Not appending to binary! Nothing to add!"
fi
