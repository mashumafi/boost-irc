#include "json_client.hpp"

#include <boost/property_tree/json_parser.hpp>

json_client* json_client::create(const std::string& server, const std::string& path, const boost::function<void (const boost::property_tree::ptree& pt)>& json_cb)
{
  return new json_client(server, path, json_cb);
}

json_client::json_client(const std::string& server, const std::string& path, const boost::function<void (const boost::property_tree::ptree& pt)>& json_cb)
{
  http_client::create(server, path, [=] (std::stringstream& res)
  {
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(res, pt);
    json_cb(pt);
  });
}
