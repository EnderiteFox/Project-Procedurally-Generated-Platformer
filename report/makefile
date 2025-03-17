all : document clean

document : main.dvi
	pdflatex main.tex main.lof main.toc main.aux > /dev/null

main.dvi : main.tex
	pdflatex main.tex

clean :
	rm main.aux main.lof main.log main.toc 2>/dev/null
	 
start : all
	evince main.pdf
