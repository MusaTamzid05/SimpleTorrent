#include "torrent.h"
#include <iostream>
#include "util.h"

namespace SimpleTorrent {


    Torrent::Torrent():shut_down(false),  sess(get_session_params()), resume_path(".resume_file") {
        last_save_resume =  std::chrono::steady_clock::now();
    }


    Torrent::~Torrent() {

    }



    void Torrent::init_session(const std::string url, const std::string dst_path) {

        std::vector<char> buf =  Util::load_file(resume_path.c_str());
        lt::add_torrent_params magnet = lt::parse_magnet_uri(url);

        if(buf.size()) {
            lt::add_torrent_params atp = lt::read_resume_data(buf);
            if(atp.info_hashes == magnet.info_hashes)
                magnet = std::move(atp);
        }

        magnet.save_path = dst_path;
        sess.async_add_torrent(std::move(magnet));
    }


    lt::session_params Torrent::get_session_params() {

        std::vector<char> session_params = Util::load_file(".session");
        lt::session_params params = session_params.empty() ? lt::session_params() : lt::read_session_params(session_params);
        params.settings.set_int(
                lt::settings_pack::alert_mask , lt::alert_category::error |
                lt::alert_category::storage |
                lt::alert_category::status
                );

        return params;

    }


    void Torrent::download(const std::string url, const std::string dst_path) {
        std::cout << "downloading from " << url << " to " << dst_path << "\n";
        init_session(url, dst_path);
        bool downloading = true;

        while(downloading) {
            std::vector<lt::alert*> alerts;
            sess.pop_alerts(&alerts);
            downloading  = handle_session(alerts);

            if(downloading)
                update();
        }

    }




    char const* Torrent::state(lt::torrent_status::state_t s) {

#ifndef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignore "-Wcovered-switch-default"
#endif

        switch(s) {
            case lt::torrent_status::checking_files: return "checking";
            case lt::torrent_status::downloading_metadata: return "dl_metada";
            case lt::torrent_status::downloading: return "downloading";
            case lt::torrent_status::finished: return "finished";
            case lt::torrent_status::seeding: return "seeding";
            case lt::torrent_status::allocating: return "allocating ";
            case lt::torrent_status::checking_resume_data: return "checking resume";
            default: return "<>";
        }

    }

    bool Torrent::handle_session(std::vector<lt::alert*> alerts) {

        bool downloading;

        for(lt::alert const* alert : alerts) {
            if(auto at = lt::alert_cast<lt::add_torrent_alert>(alert))
                handle = at->handle;

            if(lt::alert_cast<lt::torrent_finished_alert>(alert)) {
                handle.save_resume_data(lt::torrent_handle::save_info_dict);
                downloading = false;
            }

            if(lt::alert_cast<lt::torrent_error_alert>(alert)) {
                std::cout << alert->message() << "\n";
                handle.save_resume_data(lt::torrent_handle::save_info_dict);
                downloading = false;
            }

            if(auto rd = lt::alert_cast<lt::save_resume_data_alert>(alert)) {
                std::ofstream of(resume_path, std::ios_base::binary);
                of.unsetf(std::ios_base::skipws);
                auto const b = write_resume_data_buf(rd->params);
                of.write(b.data(), int(b.size()));
		
		if(!downloading) return false;

            }

            if(lt::alert_cast<lt::save_resume_data_failed_alert>(alert)) {
		if(!downloading) return false;
            }

            if(auto st = lt::alert_cast<lt::state_update_alert>(alert)) {
                if(st->status.empty())
                    continue;

                lt::torrent_status const& s = st->status[0];
                std::cout << state(s.state) << " ";
                std::cout << s.download_payload_rate / 1000  << " kb/s ";
                std::cout << s.total_done / 1000  << " kb (  ";
                std::cout << s.progress_ppm / 10000  << " %) download ( ";
                std::cout << s.num_peers  << " peers )\x1b[K";
                std::cout.flush();
		std::cout << "\n";

            }


        }


        downloading = true;
        return downloading;
    }

    void Torrent::update() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        sess.post_torrent_updates();

        if(std::chrono::steady_clock::now() - last_save_resume > std::chrono::seconds(30)) {
            handle.save_resume_data(lt::torrent_handle::save_info_dict);
            last_save_resume = std::chrono::steady_clock::now();
        }

    }
};
