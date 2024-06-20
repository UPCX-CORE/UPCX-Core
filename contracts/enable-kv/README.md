## Summary

This README illustrates the steps for usage of enable-kv.sh script with kv_map as an example. Set your environment variables as follows before beginning the rest of the steps.

1. export UPCX_2_1_0=[upcx 2.1.0 directory]
1. export UPCX_CDT_1_8_0=[upcx.cdt 1.8.0 directory]
1. export PATH=$UPCX_2_1_0/build/bin:$PATH

## Steps

Bring up nodupcx in a different terminal

1. export PATH=$UPCX_2_1_0/build/bin:$PATH
1. nodupcx -e -p upcx --plugin upcx::producer*plugin --plugin upcx::producer_api_plugin --plugin upcx::chain_api_plugin --plugin upcx::http_plugin --plugin upcx::history_plugin --plugin upcx::history_api_plugin --filter-on=* --access-control-allow-origin=\_ --contracts-console --http-validate-host=false --verbose-http-errors --max-transaction-time=1000 --backing-store chainbase --data-dir=datadir

In the first terminal

1. $UPCX_2_1_0/contracts/enable-kv/enable-kv.sh -c $UPCX_2_1_0/build/contracts/contracts/
1. cd $UPCX_CDT_1_8_0/examples/kv_map
1. mkdir build
1. cd build
1. cmake .. -DCMAKE_PREFIX_PATH=$UPCX_2_1_0/build
1. make
1. clupcx create account upcx jane UPCX6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
1. clupcx set contract jane $UPCX_CDT_1_8_0/examples/kv_map/build/kv_map -p jane@active
1. clupcx push action jane upsert '[1, "jane.acct" "jane", "doe", "1 main st", "new york", "NY", "USA", "123"]' -p jane@active
1. clupcx push action jane get '[1]' -p jane@active
