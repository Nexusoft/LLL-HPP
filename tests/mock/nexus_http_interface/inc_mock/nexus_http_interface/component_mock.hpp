#ifndef NEXUSPOOL_NEXUS_HTTP_INTERFACE_COMPONENT_MOCK_HPP
#define NEXUSPOOL_NEXUS_HTTP_INTERFACE_COMPONENT_MOCK_HPP

#include <gmock/gmock.h>
#include <string>
#include "nexus_http_interface/component.hpp"

namespace nexuspool {
namespace nexus_http_interface {

class Component_mock : public Component
{
public:
    MOCK_METHOD(bool, get_block_reward_data, (std::string hash, common::Block_reward_data& reward_data), (override));
    MOCK_METHOD(bool, get_block_hash, (std::uint32_t height, std::string& hash), (override));
    MOCK_METHOD(bool, get_block, (std::uint32_t height, LLP::CBlock& block), (override));
    MOCK_METHOD(bool, get_mining_info, (common::Mining_info& mining_info), (override));
    MOCK_METHOD(bool, get_system_info, (common::System_info& system_info), (override));
    MOCK_METHOD(bool, does_account_exists, (std::string const& account), (override));
    MOCK_METHOD(bool, payout, (std::string account_from, std::string pin, Payout_recipients const& recipients, std::string& tx_id), (override));

};


}
}

#endif 
