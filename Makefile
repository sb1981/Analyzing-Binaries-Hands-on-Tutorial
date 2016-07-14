DIRS	:= $(wildcard */.)

all: 
	for d in $(DIRS); do ($(MAKE) -C $$d all ); done
	
clean :
	for d in $(DIRS); do ($(MAKE) -C $$d clean ); done

