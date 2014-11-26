#!/bin/bash

FN=/var/local/humifand/humifand.rrd
DST=/var/www

#FN=./humifand.rrd
#DST=.

rrdtool graph $DST/temp.png -w 500 -h 200 \
	--start -86400 --end now \
	--title "HumiFan Temperature Graph (day)" \
	--watermark "`date`" \
	--vertical-label "°C" \
	--x-grid MINUTE:10:HOUR:1:HOUR:6:0:%X \
	DEF:itemp=$FN:itemp:AVERAGE \
	DEF:otemp=$FN:otemp:AVERAGE \
	DEF:irh=$FN:irh:AVERAGE \
	DEF:orh=$FN:orh:AVERAGE \
	DEF:idp=$FN:idp:AVERAGE \
	DEF:odp=$FN:odp:AVERAGE \
	DEF:ih=$FN:ih:AVERAGE \
	DEF:oh=$FN:oh:AVERAGE \
    LINE3:itemp#0000FF:"Inside Temp\t\t\t" \
    LINE3:otemp#FF0000:"Outside Temp\n" \
  	AREA:idp#0000FF33:"Insite Dewpoint\t\t" \
  	AREA:odp#FF000033:"Outside Dewpoint\n" \
  	GPRINT:itemp:LAST:"Inside Temp Cur\: %5.2lf\t" \
   	GPRINT:otemp:LAST:"Outside Temp Cur\: %5.2lf\n" \
	GPRINT:itemp:AVERAGE:"Inside Temp Avg\: %5.2lf\t" \
	GPRINT:otemp:AVERAGE:"Outside Temp Avg\: %5.2lf\n" \
	GPRINT:itemp:MAX:"Inside Temp Max\: %5.2lf\t" \
	GPRINT:otemp:MAX:"Outside Temp Max\: %5.2lf\n" \
	GPRINT:itemp:MIN:"Inside Temp Min\: %5.2lf\t" \
	GPRINT:otemp:MIN:"Outside Temp Min\: %5.2lf\n" \

rrdtool graph $DST/temp_m.png -w 500 -h 200 \
	--start -1m --end now \
	--title "HumiFan Temperature Graph (Month)" \
	--watermark "`date`" \
	--vertical-label "°C" \
	--x-grid DAY:1:DAY:7:DAY:7:0:%m-%d \
	DEF:itemp=$FN:itemp:AVERAGE \
	DEF:otemp=$FN:otemp:AVERAGE \
	DEF:irh=$FN:irh:AVERAGE \
	DEF:orh=$FN:orh:AVERAGE \
	DEF:idp=$FN:idp:AVERAGE \
	DEF:odp=$FN:odp:AVERAGE \
	DEF:ih=$FN:ih:AVERAGE \
	DEF:oh=$FN:oh:AVERAGE \
    LINE3:itemp#0000FF:"Inside Temp\t\t\t" \
    LINE3:otemp#FF0000:"Outside Temp\n" \
  	AREA:idp#0000FF33:"Insite Dewpoint\t\t" \
  	AREA:odp#FF000033:"Outside Dewpoint\n" \
  	GPRINT:itemp:LAST:"Inside Temp Cur\: %5.2lf\t" \
   	GPRINT:otemp:LAST:"Outside Temp Cur\: %5.2lf\n" \
	GPRINT:itemp:AVERAGE:"Inside Temp Avg\: %5.2lf\t" \
	GPRINT:otemp:AVERAGE:"Outside Temp Avg\: %5.2lf\n" \
	GPRINT:itemp:MAX:"Inside Temp Max\: %5.2lf\t" \
	GPRINT:otemp:MAX:"Outside Temp Max\: %5.2lf\n" \
	GPRINT:itemp:MIN:"Inside Temp Min\: %5.2lf\t" \
	GPRINT:otemp:MIN:"Outside Temp Min\: %5.2lf\n" \

rrdtool graph $DST/rh.png -w 500 -h 200 \
	--start -86400 --end now \
	--title "HumiFan Relative Humidity Graph (day)" \
	--watermark "`date`" \
	--vertical-label "Percent" \
	--x-grid MINUTE:10:HOUR:1:HOUR:6:0:%X \
	DEF:itemp=$FN:itemp:AVERAGE \
	DEF:otemp=$FN:otemp:AVERAGE \
	DEF:irh=$FN:irh:AVERAGE \
	DEF:orh=$FN:orh:AVERAGE \
	DEF:idp=$FN:idp:AVERAGE \
	DEF:odp=$FN:odp:AVERAGE \
	DEF:ih=$FN:ih:AVERAGE \
	DEF:oh=$FN:oh:AVERAGE \
  	LINE3:irh#0000FF:"Inside rH\t\t\t" \
  	LINE3:orh#FF0000:"Outside rH\n" \
  	GPRINT:irh:LAST:"Inside rH Cur\: %5.2lf\t" \
   	GPRINT:orh:LAST:"Outside rH Cur\: %5.2lf\n" \
	GPRINT:irh:AVERAGE:"Inside rH Avg\: %5.2lf\t" \
	GPRINT:orh:AVERAGE:"Outside rH Avg\: %5.2lf\n" \
	GPRINT:irh:MAX:"Inside rH Max\: %5.2lf\t" \
	GPRINT:orh:MAX:"Outside rH Max\: %5.2lf\n" \
	GPRINT:irh:MIN:"Inside rH Min\: %5.2lf\t" \
	GPRINT:orh:MIN:"Outside rH Min\: %5.2lf\n" \

rrdtool graph $DST/rh_m.png -w 500 -h 200 \
	--start -1m --end now \
	--title "HumiFan Relative Humidity Graph (Month)" \
	--watermark "`date`" \
	--vertical-label "Percent" \
	--x-grid DAY:1:DAY:7:DAY:7:0:%m-%d \
	DEF:itemp=$FN:itemp:AVERAGE \
	DEF:otemp=$FN:otemp:AVERAGE \
	DEF:irh=$FN:irh:AVERAGE \
	DEF:orh=$FN:orh:AVERAGE \
	DEF:idp=$FN:idp:AVERAGE \
	DEF:odp=$FN:odp:AVERAGE \
	DEF:ih=$FN:ih:AVERAGE \
	DEF:oh=$FN:oh:AVERAGE \
  	LINE3:irh#0000FF:"Inside rH\t\t\t" \
  	LINE3:orh#FF0000:"Outside rH\n" \
  	GPRINT:irh:LAST:"Inside rH Cur\: %5.2lf\t" \
   	GPRINT:orh:LAST:"Outside rH Cur\: %5.2lf\n" \
	GPRINT:irh:AVERAGE:"Inside rH Avg\: %5.2lf\t" \
	GPRINT:orh:AVERAGE:"Outside rH Avg\: %5.2lf\n" \
	GPRINT:irh:MAX:"Inside rH Max\: %5.2lf\t" \
	GPRINT:orh:MAX:"Outside rH Max\: %5.2lf\n" \
	GPRINT:irh:MIN:"Inside rH Min\: %5.2lf\t" \
	GPRINT:orh:MIN:"Outside rH Min\: %5.2lf\n" \


rrdtool graph $DST/ah.png -w 500 -h 200 \
	--start -86400 --end now \
	--title "HumiFan Absolute Humidity Graph (day)" \
	--watermark "`date`" \
	--vertical-label "g/m3" \
	--x-grid MINUTE:10:HOUR:1:HOUR:6:0:%X \
	DEF:itemp=$FN:itemp:AVERAGE \
	DEF:otemp=$FN:otemp:AVERAGE \
	DEF:irh=$FN:irh:AVERAGE \
	DEF:orh=$FN:orh:AVERAGE \
	DEF:idp=$FN:idp:AVERAGE \
	DEF:odp=$FN:odp:AVERAGE \
	DEF:ih=$FN:ih:AVERAGE \
	DEF:oh=$FN:oh:AVERAGE \
	DEF:fanr=$FN:fanr:LAST \
	CDEF:fanrb=fanr,15,* \
  	LINE3:ih#0000FF:"Inside H\t\t\t" \
  	LINE3:oh#FF0000:"Outside H\t\t\t" \
  	AREA:fanrb#00FF0044:"Fan On\n" \
  	GPRINT:ih:LAST:"Inside aH Cur\: %5.2lf\t" \
   	GPRINT:oh:LAST:"Outside aH Cur\: %5.2lf\t" \
   	GPRINT:fanr:LAST:"Fan Active Cur\: %1.0lf\n" \
	GPRINT:ih:AVERAGE:"Inside aH Avg\: %5.2lf\t" \
	GPRINT:oh:AVERAGE:"Outside aH Avg\: %5.2lf\n" \
	GPRINT:ih:MAX:"Inside aH Max\: %5.2lf\t" \
	GPRINT:oh:MAX:"Outside aH Max\: %5.2lf\n" \
	GPRINT:ih:MIN:"Inside aH Min\: %5.2lf\t" \
	GPRINT:oh:MIN:"Outside aH Min\: %5.2lf\n" \

rrdtool graph $DST/ah_m.png -w 500 -h 200 \
	--start -1m --end now \
	--title "HumiFan Absolute Humidity Graph (Month)" \
	--watermark "`date`" \
	--vertical-label "g/m3" \
	--x-grid DAY:1:DAY:7:DAY:7:0:%m-%d \
	DEF:itemp=$FN:itemp:AVERAGE \
	DEF:otemp=$FN:otemp:AVERAGE \
	DEF:irh=$FN:irh:AVERAGE \
	DEF:orh=$FN:orh:AVERAGE \
	DEF:idp=$FN:idp:AVERAGE \
	DEF:odp=$FN:odp:AVERAGE \
	DEF:ih=$FN:ih:AVERAGE \
	DEF:oh=$FN:oh:AVERAGE \
	DEF:fanr=$FN:fanr:LAST \
	CDEF:fanrb=fanr,15,* \
  	LINE3:ih#0000FF:"Inside H\t\t\t" \
  	LINE3:oh#FF0000:"Outside H\t\t\t" \
  	AREA:fanrb#00FF0044:"Fan On\n" \
  	GPRINT:ih:LAST:"Inside aH Cur\: %5.2lf\t" \
   	GPRINT:oh:LAST:"Outside aH Cur\: %5.2lf\t" \
   	GPRINT:fanr:LAST:"Fan Active Cur\: %1.0lf\n" \
	GPRINT:ih:AVERAGE:"Inside aH Avg\: %5.2lf\t" \
	GPRINT:oh:AVERAGE:"Outside aH Avg\: %5.2lf\n" \
	GPRINT:ih:MAX:"Inside aH Max\: %5.2lf\t" \
	GPRINT:oh:MAX:"Outside aH Max\: %5.2lf\n" \
	GPRINT:ih:MIN:"Inside aH Min\: %5.2lf\t" \
	GPRINT:oh:MIN:"Outside aH Min\: %5.2lf\n" \
