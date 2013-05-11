SOURCE = connectfour
CFLAGS= -W -Wall -Werror -std=c99 -O3

SOURCE_TEX = probing
DELAY = 80
DENSITY = 300
WIDTH = 512

make:
	gcc $(SOURCE).c $(CFLAGS) -o $(SOURCE)
	make clean

make png:
	pdflatex -shell-escape $(SOURCE_TEX).tex -output-format=pdf
	pdf2svg $(SOURCE_TEX).pdf $(SOURCE_TEX).svg
	# Necessary, as pdf2svg does not always create valid svgs:
	inkscape $(SOURCE_TEX).svg --export-plain-svg=$(SOURCE_TEX).svg
	inkscape $(SOURCE_TEX).svg -w $(WIDTH) --export-png=$(SOURCE).png
	make clean

clean:
	rm -rf *.class *.html *.log *.aux *.svg *.aux *.log *.gnuplot
