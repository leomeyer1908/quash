clean:
	rm -f quash

quash: clean
	g++ ./quash.cpp echo.cpp cd.cpp pwd.cpp -o quash