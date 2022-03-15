#include "config/config_impl.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace nexuspool
{
namespace config
{
	Config_impl::Config_impl()
		: m_wallet_ip{ "127.0.0.1" }
		, m_wallet_port{9325}
		, m_local_port{ 0 }
		, m_public_ip{ "127.0.0.1" }
		, m_miner_listen_port{ 0 }
		, m_local_ip{"127.0.0.1"}
		, m_mining_mode{ common::Mining_mode::HASH}
		, m_pool_config{}
		, m_persistance_config{}
		, m_logfile{""}		// no logfile usage, default
		, m_log_level{ 2 }	// info level
		, m_connection_retry_interval{5}
		, m_get_height_interval{2}
		, m_session_expiry_time{5}
		, m_update_block_hashes_interval{600}
		, m_hashrate_interval{300}
		, m_miner_notifications{true}
		, m_legacy_mode{false}
	{
	}

	bool Config_impl::read_config(std::string const& pool_config_file)
	{
		std::cout << "Reading POOL config file " << pool_config_file << std::endl;

		std::ifstream config_file(pool_config_file);
		if (!config_file.is_open())
		{
			std::cerr << "Unable to read POOL config file " << pool_config_file << std::endl;
			return false;
		}

		try
		{
			json j = json::parse(config_file);
			j.at("wallet_ip").get_to(m_wallet_ip);
			j.at("wallet_port").get_to(m_wallet_port);
			if (j.count("local_ip") != 0)
			{
				j.at("local_ip").get_to(m_local_ip);
			}
			if (j.count("local_port") != 0)
			{
				j.at("local_port").get_to(m_local_port);
			}
			if (j.count("public_ip") != 0)
			{
				j.at("public_ip").get_to(m_public_ip);
			}
			if (j.count("miner_listen_port") != 0)
			{
				j.at("miner_listen_port").get_to(m_miner_listen_port);
			}

			std::string mining_mode = j["mining_mode"];
			std::for_each(mining_mode.begin(), mining_mode.end(), [](char& c) 
			{
				c = ::tolower(c);
			});

			if (mining_mode == "prime")
			{
				m_mining_mode = common::Mining_mode::PRIME;
			}
			else
			{
				m_mining_mode = common::Mining_mode::HASH;
			}

			auto pool_json = j.at("pool");

			m_pool_config.m_account = pool_json.at("account");
			m_pool_config.m_pin = pool_json.at("pin");
			m_pool_config.m_fee = pool_json.at("fee");
			m_pool_config.m_difficulty_divider = pool_json.at("difficulty_divider");
			m_pool_config.m_round_duration_hours = pool_json.at("round_duration_hours");
			m_pool_config.m_nxs_api_user = pool_json.at("nxs_api_user");
			m_pool_config.m_nxs_api_pw = pool_json.at("nxs_api_pw");

			//optional
			if (pool_json.contains("fee_address"))
			{
				m_pool_config.m_fee_address = pool_json.at("fee_address");
			}

			auto persistance_type = j.at("persistance")["type"];
			m_persistance_config.m_file = j.at("persistance")["file"];

			if (persistance_type == "database")
			{
				m_persistance_config.m_type = Persistance_type::database;
			}
			else
			{
				m_persistance_config.m_type = Persistance_type::sqlite;
			}

			// advanced configs
			if (j.count("connection_retry_interval") != 0)
			{
				j.at("connection_retry_interval").get_to(m_connection_retry_interval);
			}
			if (j.count("get_height_interval") != 0)
			{
				j.at("get_height_interval").get_to(m_get_height_interval);
			}
			if (j.count("session_expiry_time") != 0)
			{
				j.at("session_expiry_time").get_to(m_session_expiry_time);
			}
			if (j.count("update_block_hashes_interval") != 0)
			{
				j.at("update_block_hashes_interval").get_to(m_update_block_hashes_interval);
			}
			if (j.count("get_hashrate_interval") != 0)
			{
				j.at("get_hashrate_interval").get_to(m_hashrate_interval);
			}
			if (j.count("miner_notifications") != 0)
			{
				j.at("miner_notifications").get_to(m_miner_notifications);
			}
			if (j.count("legacy_mode") != 0)
			{
				j.at("legacy_mode").get_to(m_legacy_mode);
			}

			if (j.count("logfile") != 0)
			{
				j.at("logfile").get_to(m_logfile);
			}

			if (j.count("log_level") != 0)
			{
				j.at("log_level").get_to(m_log_level);
			}

			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Failed to parse POOL config file. Exception: " << e.what() << std::endl;
			return false;
		}
	}
}
}