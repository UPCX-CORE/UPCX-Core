// Import necessary libraries
#include <upcx/upcx.hpp>
#include <upcx/transaction.hpp>

using namespace upcx;

class [[upcx::contract("cfhello")]] cfhello : public contract {
  public:
      using contract::contract;

      [[upcx::action]]
      void contextfree() {
        int cfd_index = 0;
        while(true) {
          int read_size = get_context_free_data( cfd_index, nullptr, 0 );
          if (read_size == -1) {
            if (cfd_index == 0) {
              print("No context free data found");
            }
            break;
          }

          char* context_free_data = new char[read_size];
          get_context_free_data( cfd_index, context_free_data, read_size );

          print("CFD ", std::to_string(cfd_index), ":", context_free_data, "\n");
          cfd_index++;
        }
      }

      [[upcx::action]]
      void normal( name user ) {
        print( "Hi, ", user);
      }
};
