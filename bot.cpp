#include "twitch.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 3)
  {
    std::cerr << "Usage: bot username password\n";
    return 1;
  }
  Twitch irc;
  irc.pass(std::string(argv[1]));
  irc.nick(std::string(argv[2]));
  char line[64];
  while (std::cin.getline(line, 64))
  {
    irc.send(line);
  }

  return 0;
}
