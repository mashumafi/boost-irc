// Based on https://tools.ietf.org/html/rfc2812

#ifndef IRC_H
#define IRC_H

#include <cstring>
#include <vector>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using boost::asio::ip::tcp;

enum Reply
{
  NONE,
  PRIVMSG,
  PING,
  JOIN,
  RPL_WELCOME = 1,
  RPL_YOURHOST = 2,
  RPL_CREATED = 3,
  RPL_MYINFO = 4,
  RPL_BOUNCE = 5,
  RPL_TRACELINK = 200,
  RPL_TRACECONNECTING = 201,
  RPL_TRACEHANDSHAKE = 202,
  RPL_TRACEUNKNOWN = 203,
  RPL_TRACEOPERATOR = 204,
  RPL_TRACEUSER = 205,
  RPL_TRACESERVER = 206,
  RPL_TRACESERVICE = 207,
  RPL_TRACENEWTYPE = 208,
  RPL_TRACECLASS = 209,
  RPL_TRACERECONNECT = 210,
  RPL_STATSLINKINFO = 211,
  RPL_STATSCOMMANDS = 212,
  RPL_STATSCLINE = 213,
  RPL_STATSNLINE = 214,
  RPL_STATSILINE = 215,
  RPL_STATSKLINE = 216,
  RPL_STATSQLINE = 217,
  RPL_STATSYLINE = 218,
  RPL_ENDOFSTATS = 219,
  RPL_UMODEIS = 221,
  RPL_SERVICEINFO = 231,
  RPL_ENDOFSERVICES = 232,
  RPL_SERVICE = 233,
  RPL_SERVLIST = 234,
  RPL_SERVLISTEND = 235,
  RPL_STATSVLINE = 240,
  RPL_STATSLLINE = 241,
  RPL_STATSUPTIME = 242,
  RPL_STATSOLINE = 243,
  RPL_STATSHLINE = 244,
  RPL_STATSSLINE = 244,
  RPL_STATSPING = 246,
  RPL_STATSBLINE = 247,
  RPL_STATSDLINE = 250,
  RPL_LUSERCLIENT = 251,
  RPL_LUSEROP = 252,
  RPL_LUSERUNKNOWN = 253,
  RPL_LUSERCHANNELS = 254,
  RPL_LUSERME = 255,
  RPL_ADMINME = 256,
  RPL_ADMINLOC1 = 257,
  RPL_ADMINLOC2 = 258,
  RPL_ADMINEMAIL = 259,
  RPL_TRACELOG = 261,
  RPL_TRACEEND = 262,
  RPL_TRYAGAIN = 263,
  RPL_NONE = 300,
  RPL_AWAY = 301,
  RPL_USERHOST = 302,
  RPL_ISON = 303,
  RPL_UNAWAY = 305,
  RPL_NOWAWAY = 306,
  RPL_WHOISUSER = 311,
  RPL_WHOISSERVER = 312,
  RPL_WHOISOPERATOR = 313,
  RPL_WHOWASUSER = 314,
  RPL_ENDOFWHO = 315,
  RPL_WHOISCHANOP = 316,
  RPL_WHOISIDLE = 317,
  RPL_ENDOFWHOIS = 318,
  RPL_WHOISCHANNELS = 319,
  RPL_LISTSTART = 321,
  RPL_LIST = 322,
  RPL_LISTEND = 323,
  RPL_CHANNELMODEIS = 324,
  RPL_UNIQOPIS = 325,
  RPL_NOTOPIC = 331,
  RPL_TOPIC = 332,
  RPL_INVITING = 341,
  RPL_SUMMONING = 342,
  RPL_INVITELIST = 346,
  RPL_ENDOFINVITELIST = 347,
  RPL_EXCEPTLIST = 348,
  RPL_ENDOFEXCEPTLIST = 349,
  RPL_VERSION = 351,
  RPL_WHOREPLY = 352,
  RPL_NAMREPLY = 353,
  RPL_KILLDONE = 361,
  RPL_CLOSING = 362,
  RPL_CLOSEEND = 363,
  RPL_LINKS = 364,
  RPL_ENDOFLINKS = 365,
  RPL_ENDOFNAMES = 366,
  RPL_BANLIST = 367,
  RPL_ENDOFBANLIST = 368,
  RPL_ENDOFWHOWAS = 369,
  RPL_INFO = 371,
  RPL_MOTD = 372,
  RPL_INFOSTART = 373,
  RPL_ENDOFINFO = 374,
  RPL_MOTDSTART = 375,
  RPL_ENDOFMOTD = 376,
  RPL_YOUREOPER = 381,
  RPL_REHASHING = 382,
  RPL_YOURESERVICE = 383,
  RPL_MYPORTIS = 384,
  RPL_TIME = 391,
  RPL_USERSSTART = 392,
  RPL_USERS = 393,
  RPL_ENDOFUSERS = 394,
  RPL_NOUSERS = 395,
  ERR_NOSUCHNICK = 401,
  ERR_NOSUCHSERVER = 402,
  ERR_NOSUCHCHANNEL = 403,
  ERR_CANNOTSENDTOCHAN = 404,
  ERR_TOOMANYCHANNELS = 405,
  ERR_WASNOSUCHNICK = 406,
  ERR_TOOMANYTARGETS = 407,
  ERR_NOSUCHSERVICE = 408,
  ERR_NOORIGIN = 409,
  ERR_NORECIPIENT = 411,
  ERR_NOTEXTTOSEND = 412,
  ERR_NOTOPLEVEL = 413,
  ERR_WILDTOPLEVEL = 414,
  ERR_BADMASK = 415,
  ERR_UNKNOWNCOMMAND = 421,
  ERR_NOMOTD = 422,
  ERR_NOADMININFO = 423,
  ERR_FILEERROR = 424,
  ERR_NONICKNAMEGIVEN = 431,
  ERR_ERRONEUSNICKNAME = 432,
  ERR_NICKNAMEINUSE = 433,
  ERR_NICKCOLLISION = 436,
  ERR_UNAVAILRESOURCE = 437,
  ERR_USERNOTINCHANNEL = 441,
  ERR_NOTONCHANNEL = 442,
  ERR_USERONCHANNEL = 443,
  ERR_NOLOGIN = 444,
  ERR_SUMMONDISABLED = 445,
  ERR_USERSDISABLED = 446,
  ERR_NOTREGISTERED = 451,
  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTRED = 462,
  ERR_NOPERMFORHOST = 463,
  ERR_PASSWDMISMATCH = 464,
  ERR_YOUREBANNEDCREEP = 465,
  ERR_YOUWILLBEBANNED = 466,
  ERR_KEYSET = 467,
  ERR_CHANNELISFULL = 471,
  ERR_UNKNOWNMODE = 472,
  ERR_INVITEONLYCHAN = 473,
  ERR_BANNEDFROMCHAN = 474,
  ERR_BADCHANNELKEY = 475,
  ERR_BADCHANMASK = 476,
  ERR_NOCHANMODES = 477,
  ERR_BANLISTFULL = 478,
  ERR_NOPRIVILEGES = 481,
  ERR_CHANOPRIVSNEEDED = 482,
  ERR_CANTKILLSERVER = 483,
  ERR_RESTRICTED = 484,
  ERR_UNIQOPPRIVSNEEDED = 485,
  ERR_NOOPERHOST = 491,
  ERR_NOSERVICEHOST = 492,
  ERR_UMODEUNKNOWNFLAG = 501,
  ERR_USERSDONTMATCH = 502
};

extern Reply hashit(const std::string& inString);

// https://gist.github.com/datagrok/380449c30fd0c5cf2f30

class Message
{
public:
  Message(std::string raw)
  {
    const std::string regex_str = "^"                 // We'll match the whole line. Start.
                                  "(?:@(.*?)\\ )?"    // Match Tags
                                                      // Optional prefix and the space that separates it
                                                      // from the next thing. Prefix can be a servername,
                                                      // or nick[[!user]@host]
                                  "(?::("             // This whole set is optional but if it's
                                                      // here it begins with : and ends with space
                                  "([^@!\\ ]*)"       // nick
                                  "(?:"               // then, optionally user/host
                                  "(?:"               // but user is optional if host is given
                                  "!([^@]*)"          // !user
                                  ")?"                // (user was optional)
                                  "@([^\\ ]*)"        // @host
                                  ")?"                // (host was optional)
                                  ")\\ )?"            // ":nick!user@host " ends
                                  "([^\\ ]+)"         // IRC command (required)
                                                      // Optional args, max 15, space separated. Last arg is
                                                      // the only one that may contain inner spaces. More than
                                                      // 15 words means remainder of words are part of 15th arg.
                                                      // Last arg may be indicated by a colon prefix instead.
                                                      // Pull the leading and last args out separately; we have
                                                      // to split the former on spaces.
                                  "("
                                  "(?:"
                                  "\\ [^:\\ ][^\\ ]*" // space, no colon, non-space characters
                                  "){0,14}"           // repeated up to 14 times
                                  ")"                 // captured in one reference
                                  "(?:\\ :?(.*))?"    // the rest, does not capture colon.
                                  "$";                // EOL
    boost::regex expr{regex_str};
    boost::smatch what;
    if (boost::regex_search(raw, what, expr))
    {
      this->m_raw = raw;
      std::string tags_str(what[1]);
      std::vector<std::string> tags;
      boost::split(tags, tags_str, boost::is_any_of(";"));
      for(uint i = 0; i < tags.size(); i++)
      {
        std::vector<std::string> tag;
        boost::split(tag, tags[i], boost::is_any_of("="));
        if(tag[0].length() > 0)
        {
          if(tag.size() == 2)
          {
            m_tags.insert(std::pair<std::string, std::string>(tag[0], tag[1]));
          }
          else if(tag.size() == 1)
          {
            m_tags.insert(std::pair<std::string, std::string>(tag[0], ""));
          }
          else
          {
            std::cout << tags[i] << std::endl;
          }
        }
      }
      this->prefix = what[2];
      this->nickname = what[3];
      this->user = what[4];
      this->host = what[5];
      this->command = what[6];
      std::string params = std::string(what[7]);
      boost::split(this->params, params, boost::is_any_of(" "));
      this->params.erase(this->params.begin());
      if(std::string(what[8]).length() > 0); this->params.push_back(what[8]);
      //std::cout << msg.prefix << " " << msg.nickname << " " << msg.user << " " << msg.command << " " << msg.params.size() << std::endl;
    }
  }
  
  std::string toString() const
  {
    std::string tags = "    \"tags\": [\r\n";
    for(auto elem : m_tags)
    {
      tags += str(boost::format("        \"%1%\": \"%2%\",\r\n") % elem.first % elem.second);
    }
    tags += "    ]\r\n";
    return str(boost::format("{\r\n    \"nickname\": \"%1%\",\r\n    \"user\": \"%2%\",\r\n    \"host\": \"%3%\",\r\n    \"command\": \"%4%\",\r\n    \"params\": \"%5%\",\r\n%6%}") % nickname % user % host % command % boost::algorithm::join(params, ":") % tags);
  }
  const std::string& raw() const
  {
    return m_raw;
  }
  const std::map<std::string, std::string>& tags() const
  {
    return m_tags;
  }
private:
  std::string m_raw;
  std::map<std::string, std::string> m_tags;
public:
  std::string prefix;
  std::string nickname;
  std::string user;
  std::string host;
  std::string command;
  std::vector<std::string> params;
};

class IRC
{
public:
  IRC(const std::string& host, const std::string& port, const std::string& nick, const std::string& pass);
  virtual ~IRC();
  
  void close();
  
  virtual void send(const std::string& command, const std::vector<std::string>& params);
  virtual void send(const std::string& command);
  
  // 3.1.0 Connection Registration
  // 3.1.1 Password message
  virtual void pass(const std::string& pass);
  // 3.1.2 Nick message
  virtual void nick(const std::string& nick);
  // 3.1.3 User message
  
  // 3.1.4 Oper message
  
  // 3.1.5 User mode message
  
  // 3.1.6 Service message
  
  // 3.1.7 Quit
  virtual void quit(const std::string& msg = "");
  // 3.1.8 Squit
  
  // 3.2.0 Channel operations
  // 3.2.1 Join message
  virtual void join0();
  virtual void join(const std::vector<std::string>&, const std::vector<std::string>& keys=std::vector<std::string>());
  virtual void join(const std::string&, const std::string& keys = "");
  // 3.2.2 Part message
  virtual void part(const std::vector<std::string>&, const std::string& msg = "");
  virtual void part(const std::string&, const std::string& msg="");
  // 3.2.3 Channel mode message
  virtual void mode(const std::string& channel, const std::string&, const std::string& modes, const std::string& modeparams);
  // 3.2.4 Topic message
  virtual void topic(const std::string& channel, const std::string& topic);
  // 3.2.5 Names message
  virtual void names(const std::vector<std::string>& channels, const std::string& target = "");
  virtual void names(const std::string& channel, const std::string& target = "");
  // 3.2.6 List message
  virtual void list(const std::vector<std::string>& channels, const std::string& target = "");
  virtual void list(const std::string& channel, const std::string& target = "");
  // 3.2.7 Invite message
  virtual void invite(const std::string& nickname, const std::string& channel);
  // 3.2.8 Kick command
  virtual void kick(const std::vector<std::string>& channels, const std::vector<std::string>& users, const std::string& comment = "");
  
  // 3.3.0 Sending messages
  // 3.3.1 Private messages
  virtual void privmsg(const std::string&, const std::string&);
  // 3.3.2 Notice
  virtual void notice(const std::string& msgtarget, const std::string& text);
  
  // 3.4.0 Server queries and commands
  // 3.4.1 Motd message
  virtual void motd(const std::string& target);
  // 3.4.2 Lusers message
  virtual void lusers(const std::string& mask = "", const std::string& target = "");
  // 3.4.3 Version message
  virtual void version(const std::string& target = "");
  // 3.4.4 Stats message
  virtual void stats(const std::string& query = "", const std::string& target = "");
  // 3.4.5 Links message
  virtual void links(const std::string& remote_server = "", const std::string& server_mask = "");
  // 3.4.6 Time message
  virtual void time(const std::string& target = "");
  // 3.4.7 Connect message
  virtual void connect(const std::string& target_server = "", const std::string& port = "", const std::string& remote_server = "");
  // 3.4.8 Trace message
  virtual void trace(const std::string& target = "");
  // 3.4.9 Admin command
  virtual void admin(const std::string& target = "");
  // 3.4.10 Info command
  virtual void info(const std::string& target = "");
  
  // 3.5.0 Service Query and Commands
  
  
  // 3.6.0 User based queries
  
  
  // 3.7.0 Miscellaneous messages
  // 3.7.1  Kill message
  virtual void kill();
  // 3.7.2  Ping message
  virtual void ping(const std::string& server, const std::string& server2 = "");
  // 3.7.3  Pong message
  virtual void pong(const std::string& server, const std::string& server2 = "");
  // 3.7.3  Error message
  virtual void error(const std::string& error_message);
protected:
  virtual void reply(const Message&, const Reply code);
  
  virtual void join(const Message&, const std::vector<std::string>& channel, const std::vector<std::string>& keys);
  virtual void ping(const Message& msg, const std::string& server, const std::string& server2 = "");
  virtual void privmsg(const Message&, const std::string&, const std::string&);
  virtual void part(const Message&);
  virtual void connected(void);
  virtual void read(const std::string& msg);
  virtual void read(const Message& msg);
  
private:
  void connect(tcp::resolver::iterator endpoint_iterator);
  void read();
  tcp::socket* s;
  boost::asio::io_service io_service;
  boost::asio::streambuf response;
  std::string m_nick;
  std::string m_pass;
};

#endif
