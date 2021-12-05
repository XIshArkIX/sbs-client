#include "sbs-client.hpp"

/* FORMATS */
const enum formatEnum { _JSON_FORMAT, _TEXT_FORMAT };

const std::string _jsonFormat{"json"};
const std::string _textFormat{"text"};

std::map<std::string, int> formatMappings{
    {_jsonFormat, _JSON_FORMAT},
    {_textFormat, _TEXT_FORMAT},
};

void QueryBuilder::setFormat(std::string format) {
  switch (formatMappings[format]) {
    case _JSON_FORMAT:
    case _TEXT_FORMAT:
      this->format = format;
      break;
    default:
      break;
  }
}

/* METHODS */
const enum methodEnum { _GET_METHOD, _POST_METHOD };

const std::string _getMethod{"GET"};
const std::string _postMethod{"POST"};

std::map<std::string, int> methodMappings{
    {_getMethod, _GET_METHOD},
    {_postMethod, _POST_METHOD},
};

void QueryBuilder::setMethod(std::string method) {
  switch (formatMappings[method]) {
    case _GET_METHOD:
    case _POST_METHOD:
      this->method = method;
      break;
    default:
      break;
  }
}

/* ROUTES */
// TODO: add dynamic route check on start up
void QueryBuilder::setRoute(std::string route) { this->route = route; }

/* TOKEN */
void QueryBuilder::setToken(std::string token) { this->token = token; }

/* DATA */
void QueryBuilder::setData(std::string data) { this->data = data; }
void QueryBuilder::setData(Json::Value data) {
  Json::StreamWriterBuilder jsonStreamWriter;
  jsonStreamWriter["indentation"] = "";

  this->data = Json::writeString(jsonStreamWriter, data);
}

/* ARGS */
void QueryBuilder::setArgs(std::string args) { this->args = args; }

/* PATHS */
void QueryBuilder::setArgs(std::string path) { this->path = path; }

std::string QueryBuilder::result() {
  if (this->token.empty()) {
    throw "Token not set. Use setToken() method.";
  }

  if (this->route.empty()) {
    throw "Route not set. Use setRoute() method.";
  }

  if (this->method.empty() ||
      methodMappings.find(this->method) == methodMappings.end()) {
    throw "Method not set or not supported. Use setMethod() method.";
  }

  if (this->format.empty() ||
      formatMappings.find(this->format) == formatMappings.end()) {
    throw "Format not set or not supported. Use setFormat() method.";
  }

  std::stringstream ss;

  std::string upperCasedMethod{this->method};
  boost::to_upper(upperCasedMethod);

  ss << "format=" << this->format << ";"
     << "method=" << upperCasedMethod << ";"
     << "token=" << this->token << ";"
     << "route=" << this->route << ";";

  if (!this->data.empty()) {
    ss << "data=" << this->data << ";";
  }

  if (!this->args.empty()) {
    ss << "args=" << this->args << ";";
  }

  if (!this->path.empty()) {
    ss << "path=" << this->path << ";";
  }

  return std::string(ss.str());
}