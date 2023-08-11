

all: generate_images

grids.pdf: grids.tex
	pdflatex grids.tex

generate_images: grids.pdf
	pdftocairo -png -r 200 -cropbox grids.pdf
	./crop-images.sh
