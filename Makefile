clean:
	rm -f quash

quash: clean
	g++ ./quash.cpp echo.cpp -o quash