#pragma once

#include <upcxio/chain/types.hpp>
#include <upcxio/chain/contract_types.hpp>

namespace upcxio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_upcxio_newaccount(apply_context&);
   void apply_upcxio_updateauth(apply_context&);
   void apply_upcxio_deleteauth(apply_context&);
   void apply_upcxio_linkauth(apply_context&);
   void apply_upcxio_unlinkauth(apply_context&);

   /*
   void apply_upcxio_postrecovery(apply_context&);
   void apply_upcxio_passrecovery(apply_context&);
   void apply_upcxio_vetorecovery(apply_context&);
   */

   void apply_upcxio_setcode(apply_context&);
   void apply_upcxio_setabi(apply_context&);

   void apply_upcxio_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace upcxio::chain
