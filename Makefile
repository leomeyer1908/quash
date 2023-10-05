clean:
	rm -f quash

quash: clean
	g++ ./quash.cpp -o quash