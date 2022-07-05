#ifndef VIRUS_H_
#define VIRUS_H_

#include <string>
#include <memory>

namespace json2sqlite {
	class Virus;
	using VirusPtr = std::shared_ptr<Virus>;

	class Virus {
	public:
		int type_;
		std::string sha256_hash_;
		std::string sha3_384_hash_;
		std::string sha1_hash_;
		std::string md5_hash_;
		std::string first_seen_;
		std::string last_seen_;
		std::string file_name_;
		unsigned long file_size_;
		std::string file_type_mime_;
		std::string file_type_;
	public:
		Virus();
		~Virus();
	};
}

#endif