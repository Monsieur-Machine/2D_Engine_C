#!/usr/bin/bash

echo "Creating \"~/.tiled\" directory"
mkdir -p $HOME/.tiled/

echo "Copying extension to tiled directory"
cp "json_export_tool.py" "$HOME/.tiled/"

echo ""
echo "Don't forget to activate python in tiled !"