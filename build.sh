gcc ./src/dcf_test_main.c  -L ./lib -ldcf  -I ./include -g -o ./build/DcfFrameWorkDemo
export LD_LIBRARY_PATH=/home/yeqm/dis1/lib:$LD_LIBRARY_PATH
cd build&&./DcfFrameWorkDemo
