#!/bin/bash

usage()
{
    echo "usage: enable-kv.sh -c <contract directory > [-u <URL to nodoes>]" 
}

while [ "$1" != "" ]; do
    case $1 in
        -c | --contracts-dir )  shift
                       contracts_dir=$1
                       ;;
        -u | --url )   shift
                       url=$1
                       ;;
        * )            usage
                       exit 1
    esac
    shift
done

if [ "$contracts_dir" = "" ]; then
   usage
   exit 1
fi

if [ "$url" = "" ]; then
    url=http://127.0.0.1:8888
fi

curl -X POST $url/v1/producer/schedule_protocol_feature_activations -d '{"protocol_features_to_activate": ["0ec7e080177b2c02b278d5088611686b49d739925a92d9bfcacd7fc6b74053bd"]}'
sleep 1s
clupcx set contract upcx $contracts_dir/upcx.boot -p upcx@active
sleep 1s
sleep 1s
sleep 1s
clupcx push action upcx activate '["299dcb6af692324b899b39f16d5a530a33062804e41f09dc97e9f156b4476707"]' -p upcx@active
sleep 1s
sleep 1s

# KV_DATABASE
clupcx push action upcx activate '["825ee6288fb1373eab1b5187ec2f04f6eacb39cb3a97f356a07c91622dd61d16"]' -p upcx@active
# ACTION_RETURN_VALUE
clupcx push action upcx activate '["c3a6138c5061cf291310887c0b5c71fcaffeab90d5deb50d3b9e687cead45071"]' -p upcx@active
# RAM_RESTRICTIONS
clupcx push action upcx activate '["4e7bf348da00a945489b2a681749eb56f5de00b900014e137ddae39f48f69d67"]' -p upcx@active
# GET_SENDER
clupcx push action upcx activate '["f0af56d2c5a48d60a4a5b5c903edfb7db3a736a94ed589d0b797df33ff9d3e1d"]' -p upcx@active
# FORWARD_SETCODE
clupcx push action upcx activate '["2652f5f96006294109b3dd0bbde63693f55324af452b799ee137a81a905eed25"]' -p upcx@active
# ONLY_BILL_FIRST_AUTHORIZER
clupcx push action upcx activate '["8ba52fe7a3956c5cd3a656a3174b931d3bb2abb45578befc59f283ecd816a405"]' -p upcx@active
# RESTRICT_ACTION_TO_SELF
clupcx push action upcx activate '["ad9e3d8f650687709fd68f4b90b41f7d825a365b02c23a636cef88ac2ac00c43"]' -p upcx@active
# DISALLOW_EMPTY_PRODUCER_SCHEDULE
clupcx push action upcx activate '["68dcaa34c0517d19666e6b33add67351d8c5f69e999ca1e37931bc410a297428"]' -p upcx@active
# FIX_LINKAUTH_RESTRICTION
clupcx push action upcx activate '["e0fb64b1085cc5538970158d05a009c24e276fb94e1a0bf6a528b48fbc4ff526"]' -p upcx@active
# REPLACE_DEFERRED
clupcx push action upcx activate '["ef43112c6543b88db2283a2e077278c315ae2c84719a8b25f25cc88565fbea99"]' -p upcx@active
# NO_DUPLICATE_DEFERRED_ID
clupcx push action upcx activate '["4a90c00d55454dc5b059055ca213579c6ea856967712a56017487886a4d4cc0f"]' -p upcx@active
# ONLY_LINK_TO_EXISTING_PERMISSION
clupcx push action upcx activate '["1a99a59d87e06e09ec5b028a9cbb7749b4a5ad8819004365d02dc4379a8b7241"]' -p upcx@active
# CONFIGURABLE_WASM_LIMITS
clupcx push action upcx activate '["bf61537fd21c61a60e542a5d66c3f6a78da0589336868307f94a82bccea84e88"]' -p upcx@active
# BLOCKCHAIN_PARAMETERS
clupcx push action upcx activate '["5443fcf88330c586bc0e5f3dee10e7f63c76c00249c87fe4fbf7f38c082006b4"]' -p upcx@active

sleep 1s
clupcx set contract upcx $contracts_dir/upcx.bios -p upcx@active
sleep 1s
clupcx push action upcx setkvparams '[{"max_key_size":64, "max_value_size":1024, "max_iterators":128}]' -p upcx@active
