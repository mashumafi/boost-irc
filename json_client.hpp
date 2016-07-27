#ifndef JSON_CLIENT_H
#define JSON_CLIENT_H

#include "http_client.hpp"

#include <boost/property_tree/ptree.hpp>

class json_client
{
public:
  static json_client* create(const std::string& server, const std::string& path, const boost::function<void (const boost::property_tree::ptree& pt)>& json_cb);
private:
  json_client(const std::string& server, const std::string& path, const boost::function<void (const boost::property_tree::ptree& pt)>& json_cb);
};

#endif
