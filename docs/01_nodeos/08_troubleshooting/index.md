---
content_title: Nodupcx Troubleshooting
---

### "Database dirty flag set (likely due to unclean shutdown): replay required"

`nodupcx` needs to be shut down cleanly. To ensure this is done, send a `SIGTERM`, `SIGQUIT` or `SIGINT` and wait for the process to shutdown. Failing to do this will result in this error. If you get this error, your only recourse is to replay by starting `nodupcx` with `--replay-blockchain`

### "Memory does not match data" Error at Restart

If you get an error such as `St9exception: content of memory does not match data expected by executable` when trying to start `nodupcx`, try restarting `nodupcx` with one of the following options (you can use `nodupcx --help` to get a full listing of these).

```
Command Line Options for upcx::chain_plugin:
    --fix-reversible-blocks               recovers reversible block database if
                                          that database is in a bad state
    --force-all-checks                    do not skip any checks that can be
                                          skipped while replaying irreversible
                                          blocks
    --replay-blockchain                   clear chain state database and replay
                                          all blocks
    --hard-replay-blockchain              clear chain state database, recover as
                                          many blocks as possible from the block
                                          log, and then replay those blocks
    --delete-all-blocks                   clear chain state database and block
                                          log
```

### "Could not grow database file to requested size." Error

Start `nodupcx` with `--shared-memory-size-mb 1024`. A 1 GB shared memory file allows approximately half a million transactions.

### What version of UPCX am I running/connecting to?

If defaults can be used, then `clupcx get info` will output a block that contains a field called `server_version`. If your `nodupcx` is not using the defaults, then you need to know the URL of the `nodupcx`. In that case, use the following with your `nodupcx` URL:

```sh
clupcx --url http://localhost:8888 get info
```

To focus only on the version line within the block:

```sh
clupcx --url http://localhost:8888 get info | grep server_version
```

### Error 3070000: WASM Exception Error

If you try to deploy the `upcx.bios` contract or `upcx.system` contract in an attempt to boot an UPCX-based blockchain and you get the following error or similar: `Publishing contract... Error 3070000: WASM Exception Error Details: env.set_proposed_producers_ex unresolveable`, it is because you have to activate the `PREACTIVATE_FEATURE` protocol first. More details about it and how to enable it can be found in the [Bios Boot Sequence Tutorial](https://developers.upcx.io/welcome/v2.1/tutorials/bios-boot-sequence/#112-set-the-upcxsystem-contract). For more information, you may also visit the [Nodupcx Upgrade Guides](https://developers.upcx.io/manuals/upcx/latest/nodupcx/upgrade-guides/).
