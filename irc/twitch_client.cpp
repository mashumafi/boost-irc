#include "twitch_client.hpp"
#include "json_client.hpp"

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


twitch_client::twitch_client(const std::string& nick, const std::string& pass) : irc_client("irc.chat.twitch.tv", "6667", nick, pass)
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

twitch_client::~twitch_client()
{

}

bool twitch_client::reply(const Message& msg, const Reply code)
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
      json_client::create("tmi.twitch.tv", "/group/user/mushisgosu/chatters", [] (const boost::property_tree::ptree& res)
      {
        boost::property_tree::ptree chatter_count = res.get_child("chatter_count");
        std::cout << chatter_count.get_value<int>() << std::endl;
      });
      break;
    }
    default:
      return true;
      break;
  }
  return false;
}

void twitch_client::privmsg(const Message& msg, const std::string& msgtarget, const std::string& text_to_be_sent)
{
  irc_client::privmsg(msg, msgtarget, text_to_be_sent);
}
