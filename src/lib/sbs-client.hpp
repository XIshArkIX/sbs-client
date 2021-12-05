#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iterator>

class QueryBuilder {
 private:
  std::string format;
  std::string method;
  std::string route;
  std::string token;
  std::string data;
  std::string args;
  std::string path;

 public:
  void setFormat(std::string);
  void setMethod(std::string);
  void setRoute(std::string);
  void setToken(std::string);
  void setData(std::string);
  void setData(Json::Value);
  void setArgs(std::string);
  void setPath(std::string);
  std::string result();

  QueryBuilder(){};
  ~QueryBuilder(){};
};

class SocketClient {
 protected:
  std::string rawQuery(std::string);
  boost::asio::io_context io_context;
  std::string endpoint;
  std::string token;
  std::string queryBuilder();

 public:
  explicit SocketClient(std::string, std::string);
  ~SocketClient();
};

class UserClient : SocketClient {
  // constructor inheritance
  using SocketClient::SocketClient;

 public:
  std::string getAll();
  std::string getById(std::string);
  std::string set(std::string, std::string);

  UserClient(std::string, std::string);
  ~UserClient();
};

class SquadClient : SocketClient {
  // constructor inheritance
  using SocketClient::SocketClient;

 public:
  std::string getAll();
  std::string getById(std::string);
  std::string getByUser(std::string);
  std::string set(std::string, std::string);

  SquadClient(std::string, std::string);
  ~SquadClient();
};

namespace sbs {
namespace constants {
const std::string REPLACED_PLACEHODLER(" ");
const std::string TO_REPLACE_WITH_PLACEHOLDER("!");
}  // namespace constants
using user = UserClient;
using squad = SquadClient;
using raw = SocketClient;
}  // namespace sbs