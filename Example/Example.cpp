/*#include <iostream>
#include <vector>
#include <chrono>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>


std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(boost::asio::ip::tcp::socket& socket) {

	socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length) {
			if (!ec) {
				std::cout << "\n\nRead " << length << "bytes\n\n";
				for (int i = 0; i < length; i++) {
					std::cout << vBuffer[i];

					GrabSomeData(socket);
				}
			}
		});
}

int main() {


	boost::system::error_code errors;

	boost::asio::io_context context;

	boost::asio::io_context::work idleWork(context);

	std::thread thrContext = std::thread([&]() { context.run(); });

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("93.184.216.34", errors), 80);

	boost::asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, errors);

	if (!errors) {
		std::cout << "connected" << std::endl;
	}
	else {
		std::cout << "failed to connect tyo address: \n" << errors.message() << std::endl;
	}

	if (socket.is_open()) {

		GrabSomeData(socket);

		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"connection: close\r\n\r\n";

		socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()), errors);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2000ms);

		return 0;

	}
}

*/