#include "twitch.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 3)
  {
    std::cerr << "Usage: bot username password\n";
    return 1;
  }
  Twitch irc;
  irc.login(std::string(argv[1]), std::string(argv[2]));
  char line[64];
  while (std::cin.getline(line, 64))
  {
  }

  return 0;
}
