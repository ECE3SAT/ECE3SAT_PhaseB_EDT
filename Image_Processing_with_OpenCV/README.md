If when running the executable, you have an error of that kind :
"error while loading shared libraries: libopencv_core.so.3.4: cannot open shared object file: No such file or directory"

Try to : 

- Include the path of your opencv's .so files in LD_LIBRARY_PATH : export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(your_directory)/lib

- Create a file /etc/ld.so.conf.d/opencv.conf and write to it the paths of folder where your opencv libraries are stored



