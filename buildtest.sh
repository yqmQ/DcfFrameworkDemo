gcc ./src/dcf_test_main.c  -L ./lib -ldcf  -I ./include -g -o ./build/DcfBuildtest
cd build&&./DcfBuildtest