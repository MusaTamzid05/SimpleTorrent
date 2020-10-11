#include "torrent.h"
#include <iostream>

int main(int argc, char** argv) {


    if(argc != 3) {
        std::cerr << "Usage : url dst_dir " ;
        return -1;
    }

    SimpleTorrent::Torrent torrent;

    std::string url = argv[1];
    std::string dst_path = argv[2];

    torrent.download(url, dst_path);
    return 0;
}
