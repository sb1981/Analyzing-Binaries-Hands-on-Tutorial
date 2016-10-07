import os
import glob

def addBody(file, w):
	write = False
	with open(file, 'r') as f:
		while True:
			line=f.readline()
			if not line: break
			if "<body>" in line:
				write=True
			if "</body>" in line:
				f.close()
				return
			if write:
				w.write(line)
	f.close()

# delete index.html
if os.path.exists('index.html'):
	os.remove('index.html')

# get html-files
htmlfiles = glob.glob('./*.html')

# create index.html
f = open('index.html', 'w')

# header
f.write('<!DOCTYPE html>\n<html lang="en">\n<head>\n<title>')
f.write(os.path.relpath(".","..").replace("_"," "))
f.write('</title>\n<meta charset="utf-8">\n<link rel="stylesheet" href="../tabs.css">\n</head>')

#body
f.write('<body><h1>')
f.write(os.path.relpath(".","..").replace("_"," "))
f.write('</h1><a href="../index.html">Back to index</a><br><br>')

f.write('<div id="container">')
# content
ctr=1
for file in htmlfiles:
	f.write('<input id="tab-' + str(ctr) + '" type="radio" name="tab-group" ')
	if ctr==1 :
		f.write('checked="checked" ')
	f.write ('/>\n<label for="tab-' + str(ctr) + '">' + file[2:len(file)-5] + '</label>')
	ctr+=1
	
f.write('<div id="content">')

ctr=1
for file in htmlfiles:
	f.write('<div id="content-'+ str(ctr) +'">')
#	f.write('<iframe src="' + file + '"></iframe>')
	addBody(file, f)
	f.write('</div>')
	ctr+=1
	
f.write('</div>')
f.write('</div>')
f.write('</body>')


f.close()