#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

CA_DAYS=${CA_DAYS:-3650}
LEAF_DAYS=${LEAF_DAYS:-825}

echo "[gen_certs] generating CA"
openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:4096 -out ca.key
openssl req -x509 -new -key ca.key -days "$CA_DAYS" -out ca.crt -config openssl-ca.cnf

echo "[gen_certs] generating server cert"
openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -out server.key
openssl req -new -key server.key -out server.csr -config openssl-server.cnf
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
    -out server.crt -days "$LEAF_DAYS" -sha256 \
    -extfile openssl-server.cnf -extensions v3_req

echo "[gen_certs] generating client cert"
openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -out client.key
openssl req -new -key client.key -out client.csr -config openssl-client.cnf
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
    -out client.crt -days "$LEAF_DAYS" -sha256 \
    -extfile openssl-client.cnf -extensions v3_req

rm -f server.csr client.csr ca.srl
echo "[gen_certs] done"
