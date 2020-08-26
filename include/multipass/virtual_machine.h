/*
 * Copyright (C) 2017-2019 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MULTIPASS_VIRTUAL_MACHINE_H
#define MULTIPASS_VIRTUAL_MACHINE_H

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace multipass
{
class SSHKeyProvider;

class VirtualMachine
{
public:
    enum class State
    {
        off,
        stopped,
        starting,
        restarting,
        running,
        delayed_shutdown,
        suspending,
        suspended,
        unknown
    };

    using UPtr = std::unique_ptr<VirtualMachine>;
    using ShPtr = std::shared_ptr<VirtualMachine>;

    virtual ~VirtualMachine() = default;
    virtual void stop() = 0;
    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual void suspend() = 0;
    virtual State current_state() = 0;
    virtual int ssh_port() = 0;
    virtual std::string ssh_hostname() = 0;
    virtual std::string ssh_username() = 0;
    virtual std::vector<std::string> ipv4() = 0;
    virtual std::vector<std::string> ipv6() = 0;
    virtual void wait_until_ssh_up(std::chrono::milliseconds timeout) = 0;
    virtual void ensure_vm_is_running() = 0;
    virtual void update_state() = 0;

    VirtualMachine::State state;
    const std::string vm_name;
    std::condition_variable state_wait;
    std::mutex state_mutex;

protected:
    VirtualMachine(VirtualMachine::State state, const std::string& vm_name) : state{state}, vm_name{vm_name} {};
    VirtualMachine(const std::string& vm_name) : VirtualMachine(State::off, vm_name){};
    VirtualMachine(const VirtualMachine&) = delete;
    VirtualMachine& operator=(const VirtualMachine&) = delete;
};
} // namespace multipass
#endif // MULTIPASS_VIRTUAL_MACHINE_H
