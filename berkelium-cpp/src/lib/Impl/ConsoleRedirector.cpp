#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/API/Runtime.hpp>

#include <sstream>

namespace Berkelium {

namespace impl {

ConsoleRedirector::ConsoleRedirector(RuntimeRef runtime, LogType type) :
	runtime(runtime),
	type(type) {
	TRACE_OBJECT_NEW("ConsoleRedirector");
}

ConsoleRedirector::~ConsoleRedirector() {
	TRACE_OBJECT_DELETE("ConsoleRedirector");
}

void ConsoleRedirector::update(std::string& buffer) {
	// split into lines
	std::string line;
	std::istringstream is(buffer);

	if(buffer[buffer.length() - 1] == 10) {
		// buffer end == line end
		while(std::getline(is, line)) {
			log(line);
		}
		buffer = "";
	} else {
		// buffer end != line end
		std::list<std::string> lines;
		while(std::getline(is, line)) {
			lines.push_back(line);
		}
		if(lines.empty()) {
			return;
		}
		// store incomplete line
		buffer = lines.back();
		lines.pop_back();
		while(!lines.empty()) {
			log(lines.front());
			lines.pop_front();
		}
	}
}

void ConsoleRedirector::log(std::string line) {
	runtime->log(LogSource::Host, type, type == LogType::StdOut ? "STDOUT" : "STDERR", "", line);
}

} // namespace impl

} // namespace Berkelium
