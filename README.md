# Servy

A bare-metal HTTP server built from scratch in C++ using raw sockets.

## Why?

No frameworks. No abstractions. Just pure socket programming to understand how the web actually works under the hood.

## Goals

- Learn TCP/IP fundamentals hands-on
- Understand HTTP protocol at the byte level
- Master low-level systems programming on Windows (Winsock)

## Build

```bash
g++ -o servy main.cpp http_server.cpp -lws2_32
```

## Status

Work in progress.