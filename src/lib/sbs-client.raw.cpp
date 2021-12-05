#include "sbs-client.hpp"

using boost::asio::buffer;
using boost::asio::const_buffer;
using boost::asio::ip::tcp;
using boost::system::error_code;

sbs::raw::SocketClient(std::string endpoint, std::string token)
    : endpoint(endpoint), token(token){};
sbs::raw::~SocketClient(){};

std::string sbs::raw::rawQuery(std::string query) {
  // resolver of I/O context
  tcp::resolver resolver(this->io_context);
  // auto-generated endpoints for [server, port/service-name]
  auto endpoints = resolver.resolve(this->endpoint, "7827");

  // create socket object
  tcp::socket socket(io_context);
  // connect to the server
  boost::asio::connect(socket, endpoints);

  // define buffer and prepare some blocks of data
  boost::asio::streambuf buff;
  buff.prepare(0xFFFF);

  // define error placeholder
  boost::system::error_code error;

  boost::asio::write(socket, buffer(query.data(), query.size()), error);

  boost::asio::read(socket, buff, error);

  if (error && error != boost::asio::error::misc_errors::eof) {
    return std::string("socket error");
  }

  std::string result{boost::asio::buffers_begin(buff.data()),
                     boost::asio::buffers_end(buff.data())};

  return result;
}