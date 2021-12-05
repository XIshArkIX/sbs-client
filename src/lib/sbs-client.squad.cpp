#include "sbs-client.hpp"

sbs::squad::SquadClient(std::string endpoint, std::string token)
    : sbs::raw(endpoint, token){};
sbs::squad::~SquadClient(){};

std::string sbs::squad::getAll() {
  return this->rawQuery(
      std::string("\
    route=players/all\
    method=get\
    token=\
  "));
}

std::string sbs::squad::getById(std::string id) {
  return this->rawQuery(std::string("get squad ") + id);
}

std::string sbs::squad::set(std::string id, std::string data) {
  std::string query{data};

  boost::replace_all(query, sbs::constants::REPLACED_PLACEHODLER,
                     sbs::constants::TO_REPLACE_WITH_PLACEHOLDER);

  return this->rawQuery(std::string("set squad ") + id + " " + query);
}

std::string sbs::squad::getByUser(std::string userId) {
  return this->rawQuery(std::string("get squad user-") + userId);
}