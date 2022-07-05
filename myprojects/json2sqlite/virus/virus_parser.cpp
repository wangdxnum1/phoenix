#include "virus_parser.h"

#include "nlohmann/json.hpp"

#include "log/mylog.h"

#include "virus.h"

namespace json2sqlite {
	bool VirusParser::ParseVirus(const std::string& content, std::vector<VirusPtr> &virus) {
		nlohmann::json root = nlohmann::json::parse(content);
		if (root.is_discarded()) return false;
		if (!root.is_object()) return false;

		if (!(root.contains("data") && root.at("data").is_array())) return false;

		auto json_data = root.at("data");

		virus.reserve(json_data.size());

		for (auto item : json_data.items()) {
			auto value = item.value();
			if (!value.is_object()) continue;

			VirusPtr v = std::make_shared<Virus>();

			if (value.contains("sha256_hash") && value.at("sha256_hash").is_string()) {
				v->sha256_hash_ = value.at("sha256_hash");
			}

			if (value.contains("sha3_384_hash") && value.at("sha3_384_hash").is_string()) {
				v->sha3_384_hash_ = value.at("sha3_384_hash");
			}

			if (value.contains("sha1_hash") && value.at("sha1_hash").is_string()) {
				v->sha1_hash_ = value.at("sha1_hash");
			}
			if (value.contains("md5_hash") && value.at("md5_hash").is_string()) {
				v->md5_hash_ = value.at("md5_hash");
			}
			if (value.contains("first_seen") && value.at("first_seen").is_string()) {
				v->first_seen_ = value.at("first_seen");
			}

			if (value.contains("last_seen") && value.at("last_seen").is_string()) {
				v->last_seen_ = value.at("last_seen");
			}

			if (value.contains("last_seen") && value.at("last_seen").is_string()) {
				v->last_seen_ = value.at("last_seen");
			}
			if (value.contains("file_name") && value.at("file_name").is_string()) {
				v->file_name_ = value.at("file_name");
			}
			if (value.contains("file_type_mime") && value.at("file_type_mime").is_string()) {
				v->file_type_mime_  = value.at("file_type_mime");
			}
			if (value.contains("file_type") && value.at("file_type").is_string()) {
				v->file_type_ = value.at("file_type");
			}
			if (value.contains("file_size") && value.at("file_size").is_number_integer()) {
				v->file_size_ = value.at("file_size");
			}

			virus.push_back(v);
		}

		return true;
	}

	VirusParser::VirusParser() {

	}

	VirusParser::~VirusParser() {

	}
}