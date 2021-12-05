#include "sbs-client.hpp"

sbs::user::UserClient(std::string endpoint, std::string token)
    : sbs::raw(endpoint, token){};
sbs::user::~UserClient(){};

std::string sbs::user::getAll() {
  return this->rawQuery(std::string("get user all"));
}

std::string sbs::user::getById(std::string id) {
  return this->rawQuery(std::string("get user ") + id);
}

std::string sbs::user::set(std::string id, std::string data) {
  std::string query{data};

  boost::replace_all(query, sbs::constants::REPLACED_PLACEHODLER,
                     sbs::constants::TO_REPLACE_WITH_PLACEHOLDER);

  return this->rawQuery(std::string("set user") + id + " " + query);
}