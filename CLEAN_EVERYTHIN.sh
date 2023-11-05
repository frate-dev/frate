rm -rf ./_deps ./CMakeCache.txt ./CMakeFiles  build/*
make clean
rm -rf ./Makefile
sudo xargs rm -rf < install_manifest.txt
