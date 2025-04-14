CC   = pio

compile: 
		${CC} run -e esp32wroom32

documentation:
		cd docs && doxygen && cd ..

clean:
		rm -rf docs/html docs/latex
		rm -rf .pio/

.PHONY: all compile documentation clean
all: compile documentation