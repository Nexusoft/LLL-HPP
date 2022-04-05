#ifndef NEXUSPOOL_CHRONO_TIMER_IMPL_HPP
#define NEXUSPOOL_CHRONO_TIMER_IMPL_HPP

#include "chrono/timer.hpp"
#include "asio/basic_waitable_timer.hpp"
#include "asio/io_service.hpp"
#include <functional>
#include <memory>
#include <mutex>

namespace nexuspool {
namespace chrono {

class Timer_impl : public Timer
{
public:

    // According to asio documentation -> if a running timer gets deleted, asio implicitly calls cancel() on that timer
    explicit Timer_impl(std::shared_ptr<asio::io_context> io_context)
        : m_io_context{std::move(io_context)}, m_timer{*m_io_context }
    {
    }

    void start(Milliseconds expires_in, Handler handler) override
    {
        start_int(expires_in, std::move(handler));
    }

    void start(Seconds expires_in, Handler handler) override
    {
        start_int(expires_in, std::move(handler));
    }

    void stop() override 
    {
        {
            std::scoped_lock lock(m_timer_mutex);
            // destroy current active timer invocation
            m_active.reset();

            m_timer.cancel();
        }
    }

private:
    std::shared_ptr<asio::io_context> m_io_context;
    std::mutex m_timer_mutex;
    asio::basic_waitable_timer<std::chrono::steady_clock> m_timer;
    std::shared_ptr<bool> m_active{}; // keep current active timer invocation alive

    template<typename T>
    void start_int(T expires_in, Handler&& handler)
    {
        std::scoped_lock lock(m_timer_mutex);

        // create new current active timer invocation / invalidate old
        m_active = std::make_shared<bool>();

        // new call to expires_from_now() also cancels the current active expirations
        m_timer.expires_after(expires_in);
        m_timer.async_wait([lambda_handler = std::move(handler), weak_active = std::weak_ptr<bool>(m_active)](::asio::error_code const& error) 
        {
            auto const active = weak_active.lock();
            if (active) 
            {
                if (!error) 
                {
                    lambda_handler();
                }
            }
        });
    }
};

}
}

#endif