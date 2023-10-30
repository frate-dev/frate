rm -rf ./_deps ./CMakeCache.txt ./CMakeFiles
make clean
rm -rf ./Makefile
sudo xargs rm -rf < install_manifest.txt
