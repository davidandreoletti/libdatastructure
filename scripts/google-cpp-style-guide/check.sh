# !/bin/sh

echo "================================================================================"
echo "Checking directory: $1"

for fileExtension in h cpp;
do
    find "$1" -name "*.$fileExtension" -exec python cpplint.py {} \;
done;

