#include "twitch_client.hpp"
#include <server_http.hpp>

#include <iostream>
#include <server.hpp>

//Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>

using namespace std;
//Added for the json-example:
using namespace boost::property_tree;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

void default_resource_send(const HttpServer &server, shared_ptr<HttpServer::Response> response,
                           shared_ptr<ifstream> ifs, shared_ptr<vector<char> > buffer);

int main(int argc, char* argv[]) {
  if (argc != 4)
  {
    std::cerr << "Usage: bot username password doc_root\n";
    return 1;
  }
  twitch_client irc(argv[1], argv[2]);
  
  HttpServer server(8080, 1);
  server.default_resource["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    const auto web_root_path=boost::filesystem::canonical("../root");
    boost::filesystem::path path=web_root_path;
    path/=request->path;
    if(boost::filesystem::exists(path))
    {
      path=boost::filesystem::canonical(path);
      //Check if path is within web_root_path
      if(distance(web_root_path.begin(), web_root_path.end())<=distance(path.begin(), path.end()) &&
         equal(web_root_path.begin(), web_root_path.end(), path.begin()))
        {
        if(boost::filesystem::is_directory(path))
          path/="index.html";
        if(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path))
        {
          auto ifs=make_shared<ifstream>();
          ifs->open(path.string(), ifstream::in | ios::binary);
          
          if(*ifs)
          {
            //read and send 128 KB at a time
            streamsize buffer_size=131072;
            auto buffer=make_shared<vector<char> >(buffer_size);
            
            ifs->seekg(0, ios::end);
            auto length=ifs->tellg();
            
            ifs->seekg(0, ios::beg);
            
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
            default_resource_send(server, response, ifs, buffer);
            return;
          }
        }
      }
    }
    string content="Could not open path "+request->path;
    *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
  };
  server.start();


  return 0;
}

void default_resource_send(const HttpServer &server, shared_ptr<HttpServer::Response> response,
                           shared_ptr<ifstream> ifs, shared_ptr<vector<char> > buffer) {
    streamsize read_length;
    if((read_length=ifs->read(&(*buffer)[0], buffer->size()).gcount())>0) {
        response->write(&(*buffer)[0], read_length);
        if(read_length==static_cast<streamsize>(buffer->size())) {
            server.send(response, [&server, response, ifs, buffer](const boost::system::error_code &ec) {
                if(!ec)
                    default_resource_send(server, response, ifs, buffer);
                else
                    cerr << "Connection interrupted" << endl;
            });
        }
    }
}
