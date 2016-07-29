#include "twitch_client.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 3)
  {
    std::cerr << "Usage: bot username password\n";
    return 1;
  }
  twitch_client irc(argv[1], argv[2]);
  char line[64];
  while (std::cin.getline(line, 64))
  {
    irc.send(line);
  }

  return 0;
}
