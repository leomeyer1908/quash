quash: clean
	g++ -g -std=c++11 ./quash.cpp echo.cpp cd.cpp pwd.cpp jobs.cpp kill.cpp -o quash

clean:
	rm -f quash