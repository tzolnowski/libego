#!/bin/sh
SCRIPT_DIR="$(readlink -f $(dirname "$0"))"

"${SCRIPT_DIR}"/engine \
    "param.other seed 123" \
    "param.other genmove_playouts 10000" \
    \
    "param.tree use 1" \
    "param.tree max_moves 400" \
    \
    "param.other local_use 0" \
    \
    "param.other logger_is_active 0" \
    "param.other logger_directory logdir" \
    \
    gtp
