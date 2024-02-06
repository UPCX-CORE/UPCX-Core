## Description

Retrieves the ABI for an account

## Positional Parameters

- `name` _TEXT_ - The name of the account whose abi should be retrieved

## Options

- `-f,--file` _TEXT_ - The name of the file to save the contract .abi to instead of writing to console

## Examples

Retrieve and save abi for upcx.token contract

```sh
clupcx get abi upcx.token -f upcx.token.abi
```

```console
saving abi to upcx.token.abi
```
