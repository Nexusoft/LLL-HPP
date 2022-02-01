#ifndef NEXUSPOOL_NEXUS_HTTP_INTERFACE_COMPONENT_HPP
#define NEXUSPOOL_NEXUS_HTTP_INTERFACE_COMPONENT_HPP

#include "common/types.hpp"
#include "LLP/block.hpp"
#include <memory>
#include <string>
#include <vector>

namespace nexuspool 
{
namespace nexus_http_interface 
{

struct Payout_recipient_data
{
    std::string m_address;
    double m_reward;
};

using Payout_recipients = std::vector<Payout_recipient_data>;

class Component 
{
public:
    using Sptr = std::shared_ptr<Component>;

    virtual ~Component() = default;

    // Get the reward data from a block identified by the block hash
    virtual bool get_block_reward_data(std::string hash, common::Block_reward_data& reward_data) = 0;

    // Get the block hash by height
    virtual bool get_block_hash(std::uint32_t height, std::string& hash) = 0;

    // Get the block by height
    virtual bool get_block(std::uint32_t height, LLP::CBlock& block) = 0;

    // Get the mining info from wallet
    virtual bool get_mining_info(common::Mining_info& mining_info) = 0;

    // Get the system info from wallet
    virtual bool get_system_info(common::System_info& system_info) = 0;

    // Checks if the given account address exists on the blockchain
    virtual bool does_account_exists(std::string const& account) = 0;

    // Payout all miners that are given to this method
    virtual bool payout(std::string account_from, std::string pin, Payout_recipients const& recipients, std::string& tx_id) = 0;

};


}
}

#endif 
