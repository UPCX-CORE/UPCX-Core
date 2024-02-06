---
content_title: Kupcxd Troubleshooting
---

## How to solve the error "Failed to lock access to wallet directory; is another `kupcxd` running"?

Since `clupcx` may auto-launch an instance of `kupcxd`, it is possible to end up with multiple instances of `kupcxd` running. That can cause unexpected behavior or the error message above.

To fix this issue, you can terminate all running `kupcxd` instances and restart `kupcxd`. The following command will find and terminate all instances of `kupcxd` running on the system:

```sh
pkill kupcxd
```
