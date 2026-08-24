#!/usr/bin/env bash
set -e

node=/dev/dri/renderD128
[ -e "$node" ] || exit 0

render_group_id=$(stat -c %g "$node")
getent group "$render_group_id" >/dev/null || groupadd -g "$render_group_id" render
group_name=$(getent group "$render_group_id" | cut -d: -f1)
usermod -aG "$group_name" "${SUDO_USER:-$(id -un)}"
