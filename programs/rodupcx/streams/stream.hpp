#pragma once
#include <upcx/abi.hpp>
#include <fc/log/logger.hpp>

namespace b1 {

class stream_handler {
 public:
   virtual ~stream_handler() {}
   virtual const std::vector<upcx::name>& get_routes() const = 0;
   virtual void publish(const char* data, uint64_t data_size, const upcx::name& routing_key) = 0;

   bool check_route(const upcx::name& stream_route) {
      if (get_routes().size() == 0) {
         return true;
      }

      for (const auto& name : get_routes()) {
         if (name == stream_route) {
            return true;
         }
      }

      return false;
   }
};

inline std::vector<upcx::name> extract_routes(const std::string& routes_str) {
   std::vector<upcx::name> streaming_routes{};
   bool star = false;
   std::string routings = routes_str;
   while (routings.size() > 0) {
      size_t      pos          = routings.find(",");
      size_t      route_length = pos == std::string::npos ? routings.length() : pos;
      std::string route        = routings.substr(0, pos);
      ilog("extracting route ${route}", ("route", route));
      if (route != "*") {
         streaming_routes.emplace_back(upcx::name(route));
      } else {
         star = true;
      }
      routings.erase(0, route_length + 1);
   }
   if (star && !streaming_routes.empty()) {
      throw std::runtime_error(std::string("Invalid routes '") + routes_str + "'");
   }
   return streaming_routes;
}

} // namespace b1
