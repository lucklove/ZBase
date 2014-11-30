MAKE = make
CD = cd

all:
	$(CD) c && $(MAKE)
	$(CD) cc && $(MAKE)

clean:
	$(CD) c && $(MAKE) clean
	$(CD) cc && $(MAKE) clean
