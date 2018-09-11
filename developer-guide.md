# Developer Guide

## Set Proxy

```bash
export http_proxy = http://10.203.0.1:1234
export https_proxy = $http_proxy
export ftp_proxy = $http_proxy
export rsync_proxy = $http_proxy
export no_proxy = "localhost, 127.0.0.1, localaddress, .localmain.com"
```

## Set Develop Environment

```bash
pacman -S base-devel msys2-devel mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-eigen3
pacman -S mingw-w64-x86_64-fmt
pacman -S mingw-w64-x86_64-matio
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-git-lfs
pacman -S mingw-w64-x86_64-boost
pacman -Syu
```

## Appendix

### Basic Command

### Protocol Proxy

[Simple example][set_proxy]

---
[set_proxy]: https://wiki.archlinux.org/index.php/proxy_settings