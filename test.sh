
#!/bin/bash

make && clear

files=$(ls samples/*.fasml)

RED='\033[0;31m'
GREEN='\033[0;32m'
LIGHT_BLUE='\033[1;34m'
ORANGE='\033[0;33m'
NC='\033[0m' # No Color

for file in $files
do
    echo -e "${LIGHT_BLUE}Testing $file${NC}"

    echo -ne "${ORANGE}Enter the arguments: ${NC}"
    read args

    ./fasml $file $args

    if [ $? -eq 0 ]
    then
        echo -e "${GREEN}Test $file passed${NC}\n"
    else
        echo -e "${RED}Test $file failed${NC}\n"
    fi
done
