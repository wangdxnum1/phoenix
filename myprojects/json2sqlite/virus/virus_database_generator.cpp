#include "virus_database_generator.h"

#include "extension/tools/tool.h"

#include "log/mylog.h"

#include "fmt/format.h"
#include "SQLiteCpp/SQLiteCpp.h"

#include "virus/virus.h"

namespace {
	const std::string kVirusTableName = "virus";
	std::string CreateVirusTableSql() {
		return fmt::format("CREATE TABLE[{}]( \
								[virus_id] VARCHAR(64), \
								[sha256_hash] VARCHAR(64), \
								[sha3_384_hash] VARCHAR(96), \
								[sha1_hash] VARCHAR(64),\
								[md5_hash] VARCHAR(64), \
								[first_seen] VARCHAR(32), \
								[last_seen] VARCHAR(32), \
								[file_name] VARCHAR(512), \
								[file_size] BIGINT, \
								[file_type_mime] VARCHAR(64), \
								[file_type] VARCHAR(64), \
								[create_time] BIGINT, \
								primary key(virus_id));", kVirusTableName);
	}

	std::string InsertVirus(json2sqlite::VirusPtr virus) {
		return fmt::format("REPLACE INTO `{}` \
			(`virus_id`,`sha256_hash`,`sha3_384_hash`,`sha1_hash`,`md5_hash`,`first_seen`,`last_seen`, \
			`file_name`,`file_size`,`file_type_mime`,`file_type`,`create_time`) \
			values('{}','{}','{}','{}','{}','{}','{}','{}', {},'{}','{}', {})", \
			kVirusTableName, \
			NS_EXTENSION::GenerateGUID(), \
			virus->sha256_hash_,
			virus->sha3_384_hash_,
			virus->sha1_hash_,
			virus->md5_hash_,
			virus->first_seen_,
			virus->last_seen_,
			virus->file_name_,
			virus->file_size_,
			virus->file_type_mime_,
			virus->file_type_,
			NS_EXTENSION::GetCurrentTimestampMS());
	}
}

namespace json2sqlite {
	bool VirusDatabaseGenerator::GenerateVirusDatabase(const std::string &db_name, const std::vector<VirusPtr> &virus) {
		bool result = false;
		try
		{
			// Open a database file
			SQLite::Database db(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

			// Begin transaction
			SQLite::Transaction transaction(db);
			if (!db.tableExists(kVirusTableName)) {
				db.exec(CreateVirusTableSql());
			}
			

			// insert data
			for (auto v : virus) {
				std::string sql = InsertVirus(v);
				db.exec(sql);
			}

			// Commit transaction
			transaction.commit();

			result = true;
		}
		catch (std::exception& e)
		{
			LOG_ERROR("{}",e.what());
			result = false;
		}

		return result;
	}

	VirusDatabaseGenerator::VirusDatabaseGenerator() {

	}

	VirusDatabaseGenerator::~VirusDatabaseGenerator() {

	}
}