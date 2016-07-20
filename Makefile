FLAGS = -std=c++11 -L~/workspace/boost_1_61_0/lib -lboost_system -lboost_thread -lpthread -lboost_regex

%.o: %.cpp %.hpp
	g++ -c $< -o $@ $(FLAGS)

%.o: %.cpp
	g++ -c $< -o $@ $(FLAGS)

bot: bot.o irc.o twitch.o
	g++ $^ -o $@ $(FLAGS)
