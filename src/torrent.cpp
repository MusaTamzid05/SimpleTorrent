#include "torrent.h"
#include <iostream>

namespace SimpleTorrent {
    
    Torrent::Torrent() {

    }


    Torrent::~Torrent() {

    }


    void Torrent::download(const std::string url, const std::string dst_path) const {
        std::cout << "downloading from " << url << " to " << dst_path << "\n";
    }
};
