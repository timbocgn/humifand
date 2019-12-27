#!/bin/bash

rsync -av -e ssh ~/Documents/Dev/humifand pi@192.168.0.29:/home/pi

ssh pi@<ip> 'cd humifand;make'


