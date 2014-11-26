#!/bin/bash

rsync -av -e ssh ~/Documents/Dev/humifand pi@fanraspi:/home/pi

ssh pi@fanraspi 'cd humifand;make'


