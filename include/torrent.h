#ifndef TORRENT_H
#define TORRENT_H

#include <string>

namespace SimpleTorrent {

    class Torrent {

        public:
            Torrent();
            virtual ~Torrent();
            void download(const std::string url, const std::string dst_path) const;

    };

};

#endif
