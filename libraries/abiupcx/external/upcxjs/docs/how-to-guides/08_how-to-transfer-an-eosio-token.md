To transfer an upcx token, [submit a transaction](01_how-to-submit-a-transaction.md) to the [`transfer`](https://github.com/upcx/upcx.contracts/blob/52fbd4ac7e6c38c558302c48d00469a4bed35f7c/contracts/upcx.token/include/upcx.token/upcx.token.hpp#L83) action of the account storing the token you wish to transfer.

In the example shown below `useraaaaaaaa` transfers **1.0000 UPCX** token stored in the `upcx.token` account from `useraaaaaaaa` to `userbbbbbbbb`.

```javascript
(async () => {
  await api.transact(
    {
      actions: [
        {
          account: "upcx.token",
          name: "transfer",
          authorization: [
            {
              actor: "useraaaaaaaa",
              permission: "active",
            },
          ],
          data: {
            from: "useraaaaaaaa",
            to: "userbbbbbbbb",
            quantity: "1.0000 UPCX",
            memo: "some memo",
          },
        },
      ],
    },
    {
      blocksBehind: 3,
      expireSeconds: 30,
    }
  );
})();
```
