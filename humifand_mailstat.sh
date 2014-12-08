#!/bin/bash

MAIL_ADR=tim@way2.net
WWW_ROOT=/var/www

(uuencode $WWW_ROOT/temp.png temp.png; uuencode $WWW_ROOT/temp_m.png temp_m.png;uuencode $WWW_ROOT/rh.png rh.png; uuencode $WWW_ROOT/rh_m.png rh_m.png;uuencode $WWW_ROOT/ah.png ah.png; uuencode $WWW_ROOT/ah_m.png ah_m.png;) | mail -s "Humifand: Daily Statistic" $MAIL_ADR

