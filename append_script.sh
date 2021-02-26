#!/bin/sh
envos=`uname -s`
binsize=`ls -la doom-nukem | awk '{print $5}'`
targetsize=11000000
difference="$(($targetsize-$binsize))"
zero=0
filemaker=xfs_mkfile
if [ "$envos" = "Darwin" ]; then
  filemaker=mkfile
fi
if [ "$difference" -gt "$zero" ]; then
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
  cat wav/steam/0.wav >> doom-nukem
  cat wav/thunder.wav >> doom-nukem
  cat wav/sword.wav >> doom-nukem
  cat wav/plop.wav >> doom-nukem
fi