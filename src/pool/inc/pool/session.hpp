#ifndef NEXUSPOOL_SESSION_HPP
#define NEXUSPOOL_SESSION_HPP

#include <memory>
#include <string>
#include <chrono>
#include "pool/types.hpp"
#include "persistance/types.hpp"
#include "LLP/block.hpp"

namespace nexuspool
{
class Miner_connection;
namespace LLP { class CBlock; }

struct Session_user
{
	bool m_logged_in{ false };
	bool m_new_account{ true };
	persistance::Account_data m_account;
	std::chrono::steady_clock::time_point m_login_time;
};

// Holds relevant user data and miner_connection
class Session
{
public:

	virtual ~Session() = default;

	virtual void update_connection(std::shared_ptr<Miner_connection> miner_connection) = 0;
	virtual std::weak_ptr<Miner_connection> get_connection() = 0;
	virtual Session_user get_user_data() const = 0;
	virtual void update_user_data(Session_user const& user_data) = 0;
	virtual std::chrono::steady_clock::time_point get_update_time() const = 0;
	virtual void set_update_time(std::chrono::steady_clock::time_point update_time) = 0;
	virtual bool add_share() = 0;
	virtual void reset_shares() = 0;
	virtual void update_hashrate(double hashrate, std::uint32_t pool_nbits, std::uint32_t network_nbits) = 0;
	virtual void set_block(LLP::CBlock const& block) = 0;
	virtual std::unique_ptr<LLP::CBlock> get_block() = 0;
	virtual bool is_inactive() const = 0;
	virtual void set_inactive() = 0;
	virtual bool is_need_work() const = 0;
	virtual void needs_work(bool need_work) = 0;

	virtual bool create_account() = 0;
	virtual void login() = 0;
};

// Manages all sessions
class Session_registry
{
public:
	using Sptr = std::shared_ptr<Session_registry>;

	virtual ~Session_registry() = default;

	virtual void stop() = 0;

	// Managment methods
	virtual Session_key create_session() = 0;
	virtual std::shared_ptr<Session> get_session(Session_key key) = 0;
	virtual std::shared_ptr<Session> get_session_with_no_work() = 0;	// returns a session which needs work
	virtual void reset_work_status_of_sessions() = 0;
	virtual void clear_unused_sessions() = 0;
	virtual void end_round() = 0;
	virtual std::size_t get_sessions_size() = 0;

	// sends get_hashrate message to active sessions
	virtual void get_hashrate() = 0;
	// sends pool notification message to active sessions
	virtual void send_notification(std::string message) = 0;

	virtual bool valid_nxs_address(std::string const& nxs_address) = 0;
	virtual bool does_account_exists(std::string account) = 0;

};

}

#endif
