MARKDOWN = pandoc --from markdown_github --to html --standalone

all: ex1 ex2 ex3 $(patsubst %.md,%.html,$(wildcard *.md)) Makefile
	mv README.html common.html
	python ../generateHTML.py

ex1: ex1.c
	gcc -m64 -shared -o ex1_1.so ex1.c
	gcc -m32 -shared -o ex1_2.so ex1.c
	i686-w64-mingw32-gcc -shared -o ex1_3.so ex1.c
	x86_64-w64-mingw32-gcc -shared -o ex1_4.so ex1.c
	zip -r ex1.zip ex1_*.so 
	rm *.so

ex2: ex2.c
	gcc -m32 -o ex2 ex2.c
	zip -r ex2.zip ex2 
	rm ex2
	
ex3: ex3.c
	gcc -m32 -o ex3 ex3.c
	zip -r ex3.zip ex3 
	rm ex3

clean:
	rm *.zip
	rm -f $(patsubst %.md,%.html,$(wildcard *.md))
	rm -f *.bak *~

%.html: %.md
	$(MARKDOWN) $< --output $@
