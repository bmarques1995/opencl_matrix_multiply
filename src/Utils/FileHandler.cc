#include "FileHandler.hh"
#include <fstream>

bool FileHandler::ReadTextFile(std::string_view filepath, std::string* content)
{
	std::ifstream fileStream;
	fileStream.open(filepath.data(), std::ios::binary);
	if (!fileStream)
		return false;
	auto start = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	uint64_t fsize = fileStream.tellg() - start;
	fileStream.seekg(0, std::ios::beg);
	char* buffer = new char[fsize + 1];
	buffer[fsize] = '\0';
	fileStream.read(buffer, fsize);
	fileStream.close();
	*content = buffer;
	delete[] buffer;
	return true;
}

bool FileHandler::WriteTextFile(std::string_view filepath, std::string content)
{
	bool stored = false;
	std::ofstream fileStream;
	fileStream.open(filepath.data(), std::ios::binary);
	if (!fileStream)
		return false;
	fileStream << content;
	fileStream.close();
	return true;
}
