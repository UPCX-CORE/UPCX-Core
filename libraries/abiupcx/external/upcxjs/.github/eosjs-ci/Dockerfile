### Test:
#### docker build --tag upcxjs-ci:test ./.github/upcxjs-ci
#### docker run --publish 8888:8888 upcxjs-ci:test

ARG UPCXBRANCH=develop
ARG CDTBRANCH=develop
FROM upcx/upcx.cdt:${CDTBRANCH} as contracts
ARG CDTBRANCH
WORKDIR /root
RUN apt-get update \
 && apt-get install -yq curl git

RUN if [ "$CDTBRANCH" = "release_1.7.x" ]; then \
 git clone https://github.com/upcx/tropical-example-web-app \
 && mkdir -p /root/contracts/upcx.bios_v1.8.3 && mv ./tropical-example-web-app/upcx/contracts/upcx.bios-v1.8.3/* /root/contracts/upcx.bios_v1.8.3/ \

 && git clone https://github.com/upcx/upcx \
 && cd upcx \
 && git checkout v2.0.12 \
 && mkdir -p /root/contracts/upcx.bios/ && mv ./unittests/contracts/upcx.bios/* /root/contracts/upcx.bios/ \
 && mkdir -p /root/contracts/upcx.system/ && mv ./unittests/contracts/upcx.system/* /root/contracts/upcx.system/ \
 && mkdir -p /root/contracts/upcx.msig/ && mv ./unittests/contracts/upcx.msig/* /root/contracts/upcx.msig/ \
 && mkdir -p /root/contracts/upcx.token/ && mv ./unittests/contracts/upcx.token/* /root/contracts/upcx.token/ \
 ; fi

RUN if [ "$CDTBRANCH" = "release_1.8.x" ] || [ "$CDTBRANCH" = "develop" ]; then \
 git clone https://github.com/upcx/upcx \
 && cd upcx \
 && git checkout develop \
 && mkdir -p /root/contracts/upcx.bios/ && mv ./contracts/contracts/upcx.bios/bin/* /root/contracts/upcx.bios/ \
 && mkdir -p /root/contracts/upcx.boot/ && mv ./contracts/contracts/upcx.boot/bin/* /root/contracts/upcx.boot/ \
 && mkdir -p /root/contracts/upcx.system/ && mv ./unittests/contracts/upcx.system/* /root/contracts/upcx.system/ \
 && mkdir -p /root/contracts/upcx.msig/ && mv ./unittests/contracts/upcx.msig/* /root/contracts/upcx.msig/ \
 && mkdir -p /root/contracts/upcx.token/ && mv ./unittests/contracts/upcx.token/* /root/contracts/upcx.token/ \
 && mkdir -p /root/contracts/nested_container_kv/ && mv ./unittests/test-contracts/nested_container_kv/* /root/contracts/nested_container_kv \
 && mkdir -p /root/contracts/nested_container_multi_index/ && mv ./unittests/test-contracts/nested_container_multi_index/* /root/contracts/nested_container_multi_index \
 ; fi

RUN if [ "$CDTBRANCH" = "release_1.8.x" ] || [ "$CDTBRANCH" = "develop" ]; then \
 git clone https://github.com/upcx/upcx.cdt \
 && cd upcx.cdt \
 && git checkout develop \
 && mkdir -p build/read_only_query_tests/ \
 && upcx-cpp -abigen ./tests/unit/test_contracts/read_only_query_tests.cpp -o ./build/read_only_query_tests/read_only_query_tests.wasm \
 && mkdir -p /root/contracts/read_only_query_tests/ && mv ./build/read_only_query_tests/* /root/contracts/read_only_query_tests/ \
 ; fi

RUN if [ "$CDTBRANCH" = "release_1.8.x" ] || [ "$CDTBRANCH" = "develop" ]; then \
 git clone https://github.com/upcx/key-value-example-app.git \
 && cd key-value-example-app \
 && upcx-cpp -abigen ./contracts/kv_todo/src/kv_todo.cpp -o ./contracts/kv_todo/build/kv_todo.wasm -R ./contracts/kv_todo/ricardian/ -I ./contracts/kv_todo/include/ \
 && mkdir -p /root/contracts/kv_todo/ && mv ./contracts/kv_todo/build/* /root/contracts/kv_todo/ \
 ; fi

RUN if [ "$CDTBRANCH" = "release_1.8.x" ] || [ "$CDTBRANCH" = "develop" ]; then \
 git clone https://github.com/upcx/return-values-example-app.git \
 && cd return-values-example-app \
 && upcx-cpp -abigen ./contracts/action_return_value/src/action_return_value.cpp -o ./contracts/action_return_value/build/action_return_value.wasm -R ./contracts/action_return_value/ricardian/ \
 && mkdir -p /root/contracts/action_return_value/ && mv ./contracts/action_return_value/build/* /root/contracts/action_return_value/ \
 ; fi

RUN mkdir cfhello
COPY ./contracts/cfhello.cpp /root/cfhello
RUN cd cfhello \
 && mkdir build \
 && upcx-cpp -abigen ./cfhello.cpp -o ./build/cfhello.wasm \
 && mkdir -p /root/contracts/cfhello/ && mv ./build/* /root/contracts/cfhello/

FROM upcx/upcx:${UPCXBRANCH}
ARG UPCXBRANCH
ENTRYPOINT ["nodupcx", "--data-dir", "/root/.local/share", "-e", "-p", "upcx", "--replay-blockchain", "--plugin", "upcx::producer_plugin", "--plugin", "upcx::producer_api_plugin", "--plugin", "upcx::chain_api_plugin", "--plugin", "upcx::trace_api_plugin", "--trace-no-abis", "--plugin", "upcx::db_size_api_plugin", "--plugin", "upcx::http_plugin", "--http-server-address=0.0.0.0:8888", "--access-control-allow-origin=*", "--contracts-console", "--http-validate-host=false", "--enable-account-queries=true", "--verbose-http-errors", "--max-transaction-time=100"]
WORKDIR /root
RUN mkdir -p "/opt/upcx/bin/contracts"
COPY --from=contracts /root/contracts /opt/upcx/bin/contracts/
COPY ./ /opt/upcx/bin/

RUN mkdir -p "/opt/upcx/bin/config-dir"
RUN /bin/bash /opt/upcx/bin/scripts/deploy_contracts.sh "$UPCXBRANCH"
