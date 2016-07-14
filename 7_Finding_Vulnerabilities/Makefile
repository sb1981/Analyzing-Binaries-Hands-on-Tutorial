MARKDOWN = pandoc --from markdown_github --to html --standalone

all: ex1 ex2 $(patsubst %.md,%.html,$(wildcard *.md)) Makefile
	mv README.html common.html
	python ../generateHTML.py
	
ex1: ex1.c
	gcc -m32 -o ex1 ex1.c
	i686-w64-mingw32-gcc -o ex1.exe ex1.c
	zip -r ex1.zip ex1 ex1.exe
	rm ex1
	rm ex1.exe

ex2: ex2.c
	gcc -m32 -o ex2 ex2.c
	i686-w64-mingw32-gcc -o ex2.exe ex2.c
	zip -r ex2.zip ex2 ex2.exe
	rm ex2
	rm ex2.exe

clean:
	rm *.zip
	rm -f $(patsubst %.md,%.html,$(wildcard *.md))
	rm -f *.bak *~

%.html: %.md
	$(MARKDOWN) $< --output $@