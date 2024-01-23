## Description

The `chain_api_plugin` exposes functionality from the [`chain_plugin`](../chain_plugin/index.md) to the RPC API interface managed by the [`http_plugin`](../http_plugin/index.md).

## Usage

```console
# config.ini
plugin = upcx::chain_api_plugin
```

```sh
# command-line
nodupcx ... --plugin upcx::chain_api_plugin
```

## Options

None

## Dependencies

- [`chain_plugin`](../chain_plugin/index.md)
- [`http_plugin`](../http_plugin/index.md)

### Load Dependency Examples

```console
# config.ini
plugin = upcx::chain_plugin
[options]
plugin = upcx::http_plugin
[options]
```

```sh
# command-line
nodupcx ... --plugin upcx::chain_plugin [operations] [options]  \
           --plugin upcx::http_plugin [options]
```
