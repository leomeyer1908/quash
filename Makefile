quash: clean
	g++ ./quash.cpp echo.cpp cd.cpp pwd.cpp -o quash

clean:
	rm -f quash