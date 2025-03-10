# Humifand intelligent cellar air conditioning system

Further instructions can be found here:

http://www.way2.net/blog/humifand/

## Installation

### BASE SYSTEM

The following documentation ist based on a fresh Raspberry PI based on
Raspbian 2014-06-20-wheezy-raspbian. 

GCC and git are preinstalled, so nothing to do here.

### PREREQUISITES

We need to install the rrdtool and its development environment first

```
sudo apt-get install rrdtool
sudo apt-get install librrd-dev
sudo apt-get install apache2
```

The last one will take a while as many dependencies are fetched.

If you want automatic emails:

```
sudo apt-get install ssmtp
sudo apt-get install mailutils
sudo apt-get install sharutils
```

Edit the `/etc/ssmtp/ssmtp.conf` and change user/pass and mailhost according to
your accounts:

```
AuthUser=youruserid@gmail.com
AuthPass=userpass
FromLineOverride=YES
mailhub=smtp.gmail.com:587
UseSTARTTLS=YES
```

To use the PHP enables AJAX version of the UI:

```
sudo apt-get install php5
sudo apt-get install php5-dev (this will take a while and fetch several other packages)
sudo apt-get install php-pear
sudo pecl install rrd (this will take a while as it compiles the extension on the raspi)
```

put the line `extension=rrd.so` into `/etc/php5/apache2/php.ini`

### GPIO Lib

Use the provided bcm2835-1.37 lib for accessing the gpio ports. It was
difficult to find the latest version as the links were not updated on the
web site.

http://www.airspayce.com/mikem/bcm2835/

* download the latest version of the library, say bcm2835-1.xx.tar.gz, then:

```
tar zxvf bcm2835-1.xx.tar.gz
cd bcm2835-1.xx
./configure
make
sudo make check
sudo make install
```

### Creating the initial rrd file

The empty RRD file can be created using the `create.sh` script. By intention
this is not included in the make file to avoid accidentially overwriting
existing RRD files.

Please invoke the script prio to the make (next step) run. The rrd file is
created in the local directory.

## Build humifand itself

To build and install the daemon please use:

```
make clean
make
make install
```

## Hardware

The basic wiring of the components is described in the connections.png file
contained in this archive.

Note that the Q1 / R1 circuit is necessary to convert
the 3.3V based output of the Raspi to the relais cards needs. Please check the
relais card documentation if this open collector circuit is compatible!


### Using PI USV

It is recommend to use the so called PI USV, an add on module which gives you a
secondary power source and shutdowns the raspi in case of a power failure. As your
system is about to run forever (ideally), this is a good idea.

You can get it here: http://www.piusv.de

To use it, follow the instructions to install the respective piusvmonitor daemon and
change the 'config.piusv' by replaing the values below:

```
// Path to the script that is executed EVERY TIME PRI_POW changes from 0 to 1.
PRI_POW=/usr/sbin/humifand_checkpiusv.sh PRI_POW

// Path to the script that is executed EVERY TIME SEC_POW changes from 0 to 1.
SEC_POW=/usr/sbin/humifand_checkpiusv.sh SEC_POW

// Path to the script that is executed EVERY TIME BAT_LOW changes from 0 to 1.
BAT_LOW=/usr/sbin/humifand_checkpiusv.sh BAT_LOW
````

This will generate a mail when something happens.

### cron jobs

To enable the regular update of the graphics for the apache and mail, add the
following to your /etc/contrab:

```
*/5 *   * * *   root    /usr/sbin/humifand_graph.sh >/dev/null
````

To enable a daily message with all graphs, add the following to your
/etc/contrab:

```
01 00    * * *   root    /usr/sbin/humifand_mailstat.sh >/dev/null
````

Don't forget to restart cron:

```
sudo /etc/init.d/cron restart
```