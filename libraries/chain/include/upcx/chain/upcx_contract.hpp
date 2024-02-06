#pragma once

#include <upcx/chain/types.hpp>
#include <upcx/chain/contract_types.hpp>

namespace upcx { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_upcx_newaccount(apply_context&);
   void apply_upcx_updateauth(apply_context&);
   void apply_upcx_deleteauth(apply_context&);
   void apply_upcx_linkauth(apply_context&);
   void apply_upcx_unlinkauth(apply_context&);

   /*
   void apply_upcx_postrecovery(apply_context&);
   void apply_upcx_passrecovery(apply_context&);
   void apply_upcx_vetorecovery(apply_context&);
   */

   void apply_upcx_setcode(apply_context&);
   void apply_upcx_setabi(apply_context&);

   void apply_upcx_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace upcx::chain
