template<typename T>
inline MjpegStreamingServer<T>::MjpegStreamingServer(FrameSnapshotCyclicBuffer<T>& buffer, std::string boundary, std::string host, int port) : 
	buffer(buffer), 
	converter(boundary), 
	host(host), 
	port(port) 
{};

template<typename T>
inline void MjpegStreamingServer<T>::Start() {
	if (this->running.exchange(true)) return;

	this->server = std::thread(&MjpegStreamingServer::Run, this);
};

template<typename T>
inline void MjpegStreamingServer<T>::Stop() {
	if (!this->running.exchange(false)) return;

	if (this->server.joinable()) this->server.join();
};

template<typename T>
inline void MjpegStreamingServer<T>::Run() {
	httplib::Server server;

	std::cout << ">>> Run() entered\n";

	server.Get("/ping", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("pong", "text/plain");
	});

	server.Get(std::string(Config::STREAM_ROUTE), [&](const httplib::Request& req, httplib::Response& res) {
		res.set_chunked_content_provider(
			"multipart/x-mixed-replace; boundary=" + this->converter.GetBoundary(),
			[&](size_t, httplib::DataSink& sink) {
				while (this->running.load()) {
					auto snapshot = this->buffer.Snapshot();

					if (snapshot.begin() != snapshot.end()) {
						const Frame* last = nullptr;

						for (const auto& frame : snapshot) {
							last = &frame;
						}

						if (last) {
							std::ostringstream stream;
							this->converter.ConvertFrame(*last, stream);

							const std::string& data = stream.str();

							if (!sink.write(data.data(), data.size())) {
								return false;
							}
						}
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(33));
				}

				return true;
			}
		);
	});

	server.listen(this->host.c_str(), this->port);
};

template<typename T>
inline MjpegStreamingServer<T>::~MjpegStreamingServer() {
	this->Stop();
}
