#include "twitch.hpp"
#include "http_client.hpp"

#include <boost/filesystem.hpp>

#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i = 0; i < argc; i++)
  {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}


Twitch::Twitch(const std::string& nick, const std::string& pass) : IRC("irc.chat.twitch.tv", "6667", nick, pass)
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  
  if (!boost::filesystem::exists("twitch.db"))
  {
  }

  rc = sqlite3_open("twitch.db", &db);
  if(rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  else
  {
    //CREATE TABLE Persons(PersonID int,LastName varchar(255),FirstName varchar(255),Address varchar(255),City varchar(255));
    rc = sqlite3_exec(db, "select * from persons", callback, 0, &zErrMsg);
    if(rc != SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
  }
}

Twitch::~Twitch()
{

}

bool Twitch::reply(const Message& msg, const Reply code)
{
  switch(code)
  {
    case RPL_WELCOME:
    case RPL_YOURHOST:
    case RPL_CREATED:
    case RPL_MYINFO:
    case RPL_MOTDSTART:
    case RPL_MOTD:
      break;
    case RPL_ENDOFMOTD:
    {
      send("CAP REQ :twitch.tv/tags");
      send("CAP REQ :twitch.tv/membership");
      send("CAP REQ :twitch.tv/commands");
      /*json("tmi.twitch.tv", "/group/user/voyboy/chatters", [] (const boost::property_tree::ptree& res)
      {
        boost::property_tree::ptree chatter_count = res.get_child("chatter_count");
        std::cout << chatter_count.get_value<int>() << std::endl;
      });*/
      http_client::create("tmi.twitch.tv", "/group/user/mashumafi/chatters", [] (const std::stringstream& res)
      {
        std::cout << res.str();
        //boost::property_tree::ptree chatter_count = res.get_child("chatter_count");
        //std::cout << chatter_count.get_value<int>() << std::endl;
        
      });
      break;
    }
    default:
      return true;
      break;
  }
  return false;
}

void Twitch::privmsg(const Message& msg, const std::string& msgtarget, const std::string& text_to_be_sent)
{
  IRC::privmsg(msg, msgtarget, text_to_be_sent);
}
