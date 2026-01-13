#!/bin/bash

# GenerateVersion.sh - Generate all version files from templates

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Change to script directory
cd "$SCRIPT_DIR"

# Call UpdateVersion.sh for each template file
./UpdateVersion.sh ../../Config/QMake/AcfVersion.pri.xtrsvn
./UpdateVersion.sh ../../Partitura/AcfVoce.arp/VersionInfo.acc.xtrsvn
./UpdateVersion.sh ../../Include/istd/AcfVersion.h.xtrsvn
