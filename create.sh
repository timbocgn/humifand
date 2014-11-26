#!/bin/bash

#
# This create the following database:
#
# 	itemp	Inside temperature (°C)
# 	otemp	Outside temperature (°C)
# 	irh		Inside relative humidity (in %RH)
# 	orh		Outside relative humidity (in %RH)
# 	ih		Inside absolute humidity (in g/m³)
# 	oh		Outside absolute humidity (in g/m³)
# 	idp		Inside dew point (°C)
# 	odp		Outside dew point (°C)
# 	fanr	Is 1 if the fan runs and 0 if not
#
# 	The step is 30 seconds, which is the probe time of humifan
#   7 * 24 * 60 * 60 / 30 	=> 20.160 PDP's give us one week of 30 sec average
#	365 * 24 * 60 * 60 / 600 => 52.560 PDP's give us one week of 10 min average
# 	365*24*5 => 43.800 gives us a 5 year archive when aggregating 60*60 / 30 => 120
#


rrdtool create humifand.rrd --step 30 \
  DS:itemp:GAUGE:60:-50:100 \
  DS:otemp:GAUGE:60:-50:100 \
  DS:irh:GAUGE:60:0:100 \
  DS:orh:GAUGE:60:0:100 \
  DS:ih:GAUGE:60:0:600 \
  DS:oh:GAUGE:60:0:600 \
  DS:idp:GAUGE:60:-50:100 \
  DS:odp:GAUGE:60:-50:100 \
  DS:fanr:GAUGE:60:0:1 \
  RRA:AVERAGE:0.5:1:20160 \
  RRA:AVERAGE:0.5:20:52560 \
  RRA:AVERAGE:0.5:120:43800 \
  RRA:LAST:0.5:1:20160 \
  RRA:LAST:0.5:20:52560 \
  RRA:LAST:0.5:120:43800 \
