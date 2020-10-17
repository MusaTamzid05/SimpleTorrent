#ifndef TORRENT_H
#define TORRENT_H

#include <string>
#include <atomic>
#include <chrono>
#include <libtorrent/session.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/magnet_uri.hpp>


namespace SimpleTorrent {

    class Torrent {

        public:
            Torrent();
            virtual ~Torrent();
            void download(const std::string url, const std::string dst_path);

        private:


            char const* state(lt::torrent_status::state_t s);
            void sighandler(int) { shut_down = true; }

            static lt::session_params get_session_params();

            void update();
            void init_session(const std::string url, const std::string dst_path);
            bool handle_session(std::vector<lt::alert*> alerts);

            std::atomic<bool> shut_down;
            libtorrent::session sess;
            std::chrono::steady_clock::time_point last_save_resume;
            lt::torrent_handle handle;

            std::string resume_path;



    };

};

#endif
