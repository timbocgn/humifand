CC=gcc
CFLAGS=-Wall -std=c++0x

ODIR=obj

LIBS=-lbcm2835 -lm -lstdc++ -lrrd

DEPS = RPi_SHT1x.h

_OBJ = main.o RPi_SHT1x.o conffile.o logger.o ProcessValues.o fan.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

humifand: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o humifand
	
	
.PHONY: install

install: humifand
	sudo cp -u humifand.conf /etc/.
	sudo [ -d /var/local/humifand ] || mkdir /var/local/humifand
	sudo cp -u humifand.rrd /var/local/humifand/.

	sudo cp -u humifand.sh /etc/init.d/humifand
	sudo chmod a+rx /etc/init.d/humifand	
	sudo update-rc.d humifand defaults

	sudo cp -u humifand /usr/sbin/.

	sudo cp -u -R www/* /var/www/.

	sudo cp -u humifand_graph.sh /usr/sbin/.
	sudo chmod a+rx /usr/sbin/humifand_graph.sh

	sudo cp -u humifand_mailstat.sh /usr/sbin/.
	sudo chmod a+rx /usr/sbin/humifand_mailstat.sh

	sudo cp -u humifand_checkpiusv.sh /usr/sbin/.
	sudo chmod a+rx /usr/sbin/humifand_checkpiusv.sh

