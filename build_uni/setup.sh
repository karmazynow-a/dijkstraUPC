#!/bin/bash

# Check if script is sourced. If not, print proper message and exit.
if [[ "$(basename -- "$0")" == "setup.sh" ]] 
then
    echo "Don't run $0, source it:" >&2
    echo "source setup.sh" >&2
    exit 1
fi

echo "Running source_bupc.sh"
source /opt/nfs/config/source_bupc.sh
echo "Creating nodes file"
/opt/nfs/config/station_name_list.sh 201 216 > nodes

