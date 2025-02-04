

all: generate_images

grids.pdf: grids.tex
	pdflatex grids.tex

architecture.pdf: architecture.tex
	pdflatex architecture.tex

generate_images: grids.pdf architecture.pdf
	pdftocairo -png -r 200 -cropbox grids.pdf
	pdftocairo -png -r 200 -cropbox architecture.pdf
	./crop-images.sh
