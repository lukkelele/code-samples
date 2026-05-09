# tls_http_server

---

The `CERTS` variable below assumes you run curl from the repo root.
```sh
CERTS=samples/cpp/advanced/net/tls_http_server/certs
```

## Routes

```
GET    /users
POST   /users        body: {"name":"...","email":"..."}
GET    /users/:id
DELETE /users/:id
```

## Plain HTTP
> USE_TLS=false (in main.cpp)

```sh
curl http://localhost:8080/users

curl -X POST http://localhost:8080/users \
    -H 'Content-Type: application/json' \
    -d '{"name":"Carol","email":"carol@example.com"}'

curl http://localhost:8080/users/1

curl -X DELETE http://localhost:8080/users/2 -i
```

## HTTPS
> USE_TLS=true (in main.cpp)

### Trusted via the test CA:
```sh
curl --cacert "${CERTS}/ca.crt" https://localhost:8080/users

curl --cacert "${CERTS}/ca.crt" -X POST https://localhost:8080/users \
    -H 'Content-Type: application/json' \
    -d '{"name":"Carol","email":"carol@example.com"}'

curl --cacert "${CERTS}/ca.crt" https://localhost:8080/users/1
curl --cacert "${CERTS}/ca.crt" -X DELETE https://localhost:8080/users/2 -i
```

### Skip verification
```sh
curl -k https://localhost:8080/users
```

### Inspect the handshake/negotiated cipher/ALPN:
```sh
curl --cacert "${CERTS}/ca.crt" -v https://localhost:8080/users 2>&1 | grep -E 'SSL|ALPN|TLS|subject|issuer'

openssl s_client -connect localhost:8080 -servername localhost \
    -CAfile "${CERTS}/ca.crt" -alpn http/1.1 </dev/null
```

---

## Mutual TLS (require_client_cert = true)
> require_client_cert=true (in main.cpp)

Set `require_client_cert = true` in `main.cpp` and rebuild.
```sh
curl --cacert "${CERTS}/ca.crt" \
     --cert   "${CERTS}/client.crt" \
     --key    "${CERTS}/client.key" \
     https://localhost:8080/users
```

Without the client cert the handshake should be rejected:
```sh
curl --cacert "${CERTS}/ca.crt" https://localhost:8080/users
```
