# 'Makefile'
MARKDOWN = pandoc --from markdown_github --to html --standalone
all: $(patsubst %.md,%.html,$(wildcard *.md)) Makefile
	mv README.html common.html
	python ../generateHTML.py
	
clean:
	rm -f $(patsubst %.md,%.html,$(wildcard *.md))
	rm -f *.bak *~

%.html: %.md
	$(MARKDOWN) $< --output $@
	