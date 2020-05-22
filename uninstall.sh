#!/bin/bash

INSTALLPATH=/usr/local/bin

rm $INSTALLPATH/tictactoe

if [[ $? -ne 0 ]]; then
    echo "Failed to uninstall"
    exit 1
fi

echo "Successfully uninstalled"

exit 0
