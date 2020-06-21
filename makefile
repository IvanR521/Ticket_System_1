main : Users.cpp \
		Trains.cpp \
		Orders.cpp \
		Connector.cpp \
		Main.cpp 
	g++ Users.cpp \
		Trains.cpp \
		Orders.cpp \
		Connector.cpp \
		Main.cpp \
		-o code \
		-std=c++11 -O2