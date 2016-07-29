#include "twitch_client.hpp"

#include <iostream>
#include <server.hpp>

int main(int argc, char* argv[]) {
  if (argc != 4)
  {
    std::cerr << "Usage: bot username password doc_root\n";
    return 1;
  }
  twitch_client irc(argv[1], argv[2]);
  
  /*char line[64];
  while (std::cin.getline(line, 64))
  {
    irc.send(line);
  }*/
  
  // Initialise the server.
  // <address> <port> <doc_root>
  http::server::server s("0.0.0.0", "8080", argv[3]);

  // Run the server until stopped.
  s.run();

  return 0;
}
