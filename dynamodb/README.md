
dynamodb

* sudo apt-get install libcurl4-openssl-dev
* cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC_LINKING=1 -DBUILD_ONLY="core;config;dynamodb" ..
* sudo make install

