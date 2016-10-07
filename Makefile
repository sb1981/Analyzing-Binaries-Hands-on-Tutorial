DIRS	:= $(wildcard */.)

all: 
	for d in $(DIRS); do ($(MAKE) -C $$d all ); done
	pandoc README.md --from markdown_github --to html --standalone -o index.html
	
clean :
	for d in $(DIRS); do ($(MAKE) -C $$d clean ); done

