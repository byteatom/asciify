#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>

void usage()
{
	std::cout <<
#include "README.md"
		;
}

std::error_code err;

void asciify(const fs::path &file)
{
	std::fstream stream(file, std::ios::in | std::ios::binary);
	uintmax_t  size = fs::file_size(file);
	char* buf = new char[(size_t)size];
	stream.read(buf, size);
	stream.close();
	stream.open(file, std::ios::out | std::ios::binary | std::ios::trunc);
	for (uintmax_t i = 0; i < size; ++i) {
		if (buf[i] == 9 || buf[i] == 10 || buf[i] == 13 || (32 <= buf[i] && buf[i] <= 126))
			stream.put(buf[i]);
	}
	delete buf;
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		usage();
		return EXIT_SUCCESS;
	}

	fs::path src{ argv[1] };

	if (!fs::exists(src, err)) {
		std::cout << "Source not exist" << std::endl;
		return EXIT_FAILURE;
	}

	if (fs::is_regular_file(src, err))
		asciify(src);
	else if (fs::is_directory(src, err)) {
		for (auto& entry : fs::recursive_directory_iterator(src)) {
			if (fs::is_regular_file(entry, err))
				asciify(entry);
		}
	}

	return EXIT_SUCCESS;
}

