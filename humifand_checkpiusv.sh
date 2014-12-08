#!/bin/bash

MAIL_ADR=tim@way2.net

case "$1" in
    PRI_POW)
            MAIL_TEXT="Humifand Primary Power restored. Everything allright!"
            MAIL_SUBJECT="Humifand: Primary Power restored"
            ;;

    SEC_POW)
            MAIL_TEXT="Humifand Primary Power failure. We're about to shutdown. Dry air is in danger!"
            MAIL_SUBJECT="Humifand: Primary Power failure"
            ;;

    BAT_LOW)
            MAIL_TEXT="Humifand secondary battery is low. Please replace as soon as as possible!"
            MAIL_SUBJECT="Humifand: Secondary Power failure"
            ;;
    *)
            echo $"Usage: $0 {PRI_POW,SEC_POW,BAT_LOW}"
            exit 1
esac

echo $MAIL_TEXT | mail -s "$MAIL_SUBJECT" $MAIL_ADR

