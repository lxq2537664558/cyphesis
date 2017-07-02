// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2009 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA


#ifdef HAVE_CONFIG_H
#endif // HAVE_CONFIG_H

#include "common/AtlasStreamClient.h"
#include "common/ClientTask.h"

#include "common/debug.h"

#include <Atlas/Codec.h>
#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Net/Stream.h>

#ifdef _WIN32
#undef DATADIR
#endif // _WIN32

#include <iostream>

using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;
using Atlas::Objects::Root;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Operation::Create;
using Atlas::Objects::Operation::Login;
using Atlas::Objects::Operation::RootOperation;

using namespace boost::asio;

static const bool debug_flag = false;


StreamClientSocketBase::StreamClientSocketBase(boost::asio::io_service& io_service, std::function<void()>& dispatcher)
: m_io_service(io_service), mDispatcher(dispatcher), m_ios(&mBuffer), m_codec(nullptr), m_encoder(nullptr), m_is_connected(false)
{
}

StreamClientSocketBase::~StreamClientSocketBase()
{
    delete m_encoder;
    delete m_codec;
}

std::iostream& StreamClientSocketBase::getIos()
{
    return m_ios;
}

int StreamClientSocketBase::negotiate(Atlas::Objects::ObjectsDecoder& decoder)
{

    Atlas::Net::StreamConnect conn("cyphesis_client", m_ios, m_ios);

    while (conn.getState() == Atlas::Net::StreamConnect::IN_PROGRESS) {
      write();
      auto dataReceived = read_blocking();
      conn.poll(dataReceived > 0);
    }

    if (conn.getState() == Atlas::Net::StreamConnect::FAILED) {
        std::cerr << "Failed to negotiate" << std::endl;
        return -1;
    }

    m_codec = conn.getCodec(decoder);

    if (!m_codec) {
        return -1;
    }

    m_encoder = new Atlas::Objects::ObjectsEncoder(*m_codec);

    m_codec->streamBegin();

    do_read();

    return 0;
}

Atlas::Codec& StreamClientSocketBase::getCodec()
{
    return *m_codec;
}

Atlas::Objects::ObjectsEncoder& StreamClientSocketBase::getEncoder()
{
    return *m_encoder;
}

int StreamClientSocketBase::poll(const boost::posix_time::time_duration& duration) {
    return poll(duration, []()->bool{return false;});
}

int StreamClientSocketBase::poll(const boost::posix_time::time_duration& duration, const std::function<bool()> exitCheckerFn)
{
    bool hasExpired = false;
    bool isCancelled = false;
    deadline_timer timer(m_io_service);
    timer.expires_from_now(duration);
    timer.async_wait([&](boost::system::error_code ec){
        if (!ec) {
            hasExpired = true;
        } else {
            isCancelled = true;
        }
    });

    //We'll try to only run one handler each polling. Either our timer gets called, or one of the network handlers.
    //The reason for this loop is that when we cancel the timer we need to poll run handlers until the timer handler
    //has been run, since it references locally scoped variables.
    while (!hasExpired && !isCancelled && !exitCheckerFn()) {
        m_io_service.run_one();
        //Check if we didn't run the timer handler; if so we should cancel it and then keep on polling until
        //it's been run.
        if (!hasExpired && !isCancelled) {
            timer.cancel();
        }
    }
    if (!m_is_connected) {
        return -1;
    }
    if (hasExpired) {
        return 1;
    }
    return 0;
}




TcpStreamClientSocket::TcpStreamClientSocket(boost::asio::io_service& io_service, std::function<void()>& dispatcher, boost::asio::ip::tcp::endpoint endpoint)
: StreamClientSocketBase(io_service, dispatcher), m_socket(io_service)
{
    m_socket.connect(endpoint);
    m_is_connected = true;
}

void TcpStreamClientSocket::do_read()
{
    m_socket.async_read_some(mReadBuffer.prepare(read_buffer_size),
            [this](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    mReadBuffer.commit(length);
                    this->m_ios.rdbuf(&mReadBuffer);
                    m_codec->poll();
                    this->m_ios.rdbuf(&mBuffer);
                    mDispatcher();
                    this->do_read();
                } else {
                    m_is_connected = false;
                }
            });
}

size_t TcpStreamClientSocket::read_blocking()
{
    if (!m_socket.is_open()) {
        return 0;
    }
    if (m_socket.available() > 0) {
        auto received = m_socket.read_some(mBuffer.prepare(m_socket.available()));
        if (received > 0) {
            mBuffer.commit(received);
        }
        return received;
    }
    return 0;
}

size_t TcpStreamClientSocket::write()
{
    if (!m_socket.is_open()) {
        return 0;
    }
    if (mBuffer.size() == 0) {
        return 0;
    }

    auto size = boost::asio::write(m_socket, mBuffer.data(),
              boost::asio::transfer_all());
    mBuffer.consume(size);
    return size;
}



LocalStreamClientSocket::LocalStreamClientSocket(boost::asio::io_service& io_service, std::function<void()>& dispatcher, boost::asio::local::stream_protocol::endpoint endpoint)
: StreamClientSocketBase(io_service, dispatcher), m_socket(io_service)
{
    m_socket.connect(endpoint);
    m_is_connected = true;
}

void LocalStreamClientSocket::do_read()
{
    m_socket.async_read_some(mReadBuffer.prepare(read_buffer_size),
            [this](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    mReadBuffer.commit(length);
                    this->m_ios.rdbuf(&mReadBuffer);
                    m_codec->poll();
                    this->m_ios.rdbuf(&mBuffer);
                    mDispatcher();
                    this->do_read();
                } else {
                    m_is_connected = false;
                }
            });
}

size_t LocalStreamClientSocket::read_blocking()
{
    if (!m_socket.is_open()) {
        return 0;
    }
    if (m_socket.available() > 0) {
        size_t received = m_socket.read_some(mBuffer.prepare(m_socket.available()));
        if (received > 0) {
            mBuffer.commit(received);
        }
        return received;
    }
    return 0;
}

size_t LocalStreamClientSocket::write()
{
    if (!m_socket.is_open()) {
        return 0;
    }
    if (mBuffer.size() == 0) {
        return 0;
    }

    auto size = boost::asio::write(m_socket, mBuffer.data(),
              boost::asio::transfer_all());
    mBuffer.consume(size);
    return size;
}



void AtlasStreamClient::output(const Element & item, size_t depth) const
{
    output_element(std::cout, item, depth);
}

void AtlasStreamClient::output(const Root & ent) const
{
    MapType entmap = ent->asMessage();
    MapType::const_iterator Iend = entmap.end();
    for (MapType::const_iterator I = entmap.begin(); I != Iend; ++I) {
        const Element & item = I->second;
        std::cout << std::string(spacing(), ' ') << I->first << ": ";
        output(item, 1);
        std::cout << std::endl;
    }
}

/// \brief Function call from the base class when an object arrives from the
/// server
///
/// @param obj Object that has arrived from the server
void AtlasStreamClient::objectArrived(const Root & obj)
{
    RootOperation op = Atlas::Objects::smart_dynamic_cast<RootOperation>(obj);
    if (!op.isValid()) {
        std::cerr << "ERROR: Non op object received from server"
                  << std::endl << std::flush;;
        if (!obj->isDefaultParents() && !obj->getParents().empty()) {
            std::cerr << "NOTICE: Unexpected object has parent "
                      << obj->getParents().front()
                      << std::endl << std::flush;
        }
        if (!obj->isDefaultObjtype()) {
            std::cerr << "NOTICE: Unexpected object has objtype "
                      << obj->getObjtype()
                      << std::endl << std::flush;
        }
        return;
    }

    mOps.push_back(op);
}

void AtlasStreamClient::dispatch()
{
    for (auto& op : mOps) {
        operation(op);
    }
    mOps.clear();
}


void AtlasStreamClient::operation(const RootOperation & op)
{
    if (debug_flag) {
        debug_print("Received:");
        debug_dump(op, std::cout);
    }
    if (m_currentTask != 0) {
        OpVector res;
        m_currentTask->operation(op, res);
        OpVector::const_iterator Iend = res.end();
        for (OpVector::const_iterator I = res.begin(); I != Iend; ++I) {
            send(*I);
        }

        if (m_currentTask->isComplete()) {
            delete m_currentTask;
            m_currentTask = 0;
        }
    }

    switch (op->getClassNo()) {
        case Atlas::Objects::Operation::APPEARANCE_NO:
            appearanceArrived(op);
            break;
        case Atlas::Objects::Operation::DISAPPEARANCE_NO:
            disappearanceArrived(op);
            break;
        case Atlas::Objects::Operation::INFO_NO:
            infoArrived(op);
            break;
        case Atlas::Objects::Operation::ERROR_NO:
            errorArrived(op);
            break;
        case Atlas::Objects::Operation::SIGHT_NO:
            sightArrived(op);
            break;
        case Atlas::Objects::Operation::SOUND_NO:
            soundArrived(op);
            break;
        default:
            break;
    }
}

void AtlasStreamClient::infoArrived(const RootOperation & op)
{
    reply_flag = true;
    if (!op->isDefaultFrom()) {
        return;
    }
    if (op->isDefaultArgs() || op->getArgs().empty()) {
        std::cerr << "WARNING: Malformed account from server" << std::endl << std::flush;
        return;
    }
    if (op->isDefaultRefno()) {
        return;
    }
    if (op->getRefno() != serialNo) {
        return;
    }
    m_infoReply = op->getArgs().front();
}

void AtlasStreamClient::appearanceArrived(const RootOperation & op)
{
}

void AtlasStreamClient::disappearanceArrived(const RootOperation & op)
{
}

void AtlasStreamClient::sightArrived(const RootOperation & op)
{
}

void AtlasStreamClient::soundArrived(const RootOperation & op)
{
}

void AtlasStreamClient::loginSuccess(const Atlas::Objects::Root & arg)
{
}

/// \brief Called when an Error operation arrives
///
/// @param op Operation to be processed
void AtlasStreamClient::errorArrived(const RootOperation & op)
{
    reply_flag = true;
    error_flag = true;
    const std::vector<Root> & args = op->getArgs();
    if (args.empty()) {
        return;
    }
    const Root & arg = args.front();
    Element message_attr;
    if (arg->copyAttr("message", message_attr) == 0 && message_attr.isString()) {
        m_errorMessage = message_attr.String();
    }
}

AtlasStreamClient::AtlasStreamClient() : m_io_work(m_io_service), reply_flag(false), error_flag(false),
                                         serialNo(512), m_socket(nullptr),
                                         m_currentTask(0), m_spacing(2)
{
}

AtlasStreamClient::~AtlasStreamClient()
{
    delete m_socket;
}

void AtlasStreamClient::send(const RootOperation & op)
{
    if (m_socket == 0) {
        return;
    }

    if (debug_flag) {
        debug_print("Sending:");
        debug_dump(op, std::cout);
    }

    reply_flag = false;
    error_flag = false;
    m_socket->getEncoder().streamObjectsMessage(op);
    m_socket->write();
}

int AtlasStreamClient::connect(const std::string & host, unsigned short port)
{
    delete m_socket;
    m_socket = nullptr;
    try {
        std::function<void()> dispatcher = [&]{this->dispatch();};
        m_socket = new TcpStreamClientSocket(m_io_service, dispatcher, ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port));
    } catch (const std::exception& e) {
        return -1;
    }
    return m_socket->negotiate(*this);

}

int AtlasStreamClient::connectLocal(const std::string & filename)
{
    delete m_socket;
    m_socket = nullptr;
    try {
        std::function<void()> dispatcher = [&]{this->dispatch();};
        m_socket = new LocalStreamClientSocket(m_io_service, dispatcher, local::stream_protocol::endpoint(filename));
    } catch (const std::exception& e) {
        return -1;
    }
    return m_socket->negotiate(*this);
}

int AtlasStreamClient::cleanDisconnect()
{
    delete m_socket;
    m_socket = nullptr;
//    // Shutting down our write side will cause the server to get a HUP once
//    // it has consumed all we have left for it
//    m_ios->shutdown(true);
//    // The server will then close the socket once we have all the responses
//    while (this->poll(20, 0) == 0);
    return 0;
}


int AtlasStreamClient::login(const std::string & username,
                             const std::string & password)
{
    m_username = username;

    Login l;
    Anonymous account;
 
    account->setAttr("username", username);
    account->setAttr("password", password);
 
    l->setArgs1(account);
    l->setSerialno(newSerialNo());
 
    send(l);

    return waitForLoginResponse();
}

int AtlasStreamClient::create(const std::string & type,
                              const std::string & username,
                              const std::string & password)
{
    m_username = username;

    Create c;
    Anonymous account;

    account->setAttr("username", username);
    account->setAttr("password", password);
    account->setParents(std::list<std::string>(1, type));

    c->setArgs1(account);
    c->setSerialno(newSerialNo());

    send(c);

    return waitForLoginResponse();
}

int AtlasStreamClient::waitForLoginResponse()
{
    while (poll(boost::posix_time::seconds(10)) == 0) {
        if (reply_flag && !error_flag) {
            if (m_infoReply->isDefaultId()) {
               std::cerr << "Malformed reply" << std::endl << std::flush;
            } else {
                m_accountId = m_infoReply->getId();
                if (!m_infoReply->getParents().empty()) {
                    m_accountType = m_infoReply->getParents().front();
                }
                loginSuccess(m_infoReply);
                return 0;
            }
            reply_flag = false;
        }
    }

    return -1;
}

int AtlasStreamClient::pollOne(const boost::posix_time::time_duration& duration)
{
    if (!m_socket) {
        return -1;
    }
    int result = m_socket->poll(duration, [&]()->bool{return !mOps.empty();});
    if (result == -1) {
        std::cerr << "Server disconnected" << std::endl << std::flush;
    }
    return result;
}


int AtlasStreamClient::poll(const boost::posix_time::time_duration& duration)
{
    if (!m_socket) {
        return -1;
    }
    int result = m_socket->poll(duration, [](){return false;});
    if (result == -1) {
        std::cerr << "Server disconnected" << std::endl << std::flush;
    }
    return result;
}

int AtlasStreamClient::poll(int seconds, int microseconds)
{
    return poll(boost::posix_time::seconds(seconds) + boost::posix_time::microseconds(microseconds));
}

int AtlasStreamClient::runTask(ClientTask * task, const std::string & arg)
{
    assert(task != 0);

    if (m_currentTask != 0) {
        std::cout << "Busy" << std::endl << std::flush;
        return -1;
    }

    m_currentTask = task;

    OpVector res;

    m_currentTask->setup(arg, res);

    if (m_currentTask->isComplete()) {
        delete task;
        m_currentTask = 0;
        return -1;
    }

    OpVector::const_iterator Iend = res.end();
    for (OpVector::const_iterator I = res.begin(); I != Iend; ++I) {
        send(*I);
    }
    return 0;
}

int AtlasStreamClient::endTask()
{
    if (m_currentTask == 0) {
        return -1;
    }
    delete m_currentTask;
    m_currentTask = 0;
    return 0;
}

bool AtlasStreamClient::hasTask() const {
    return m_currentTask != nullptr;
}

int AtlasStreamClient::pollUntilTaskComplete()
{
    if (m_currentTask == nullptr) {
        return 1;
    }
    while (m_currentTask != nullptr) {
        if (poll(boost::posix_time::milliseconds(100)) == -1) {
            return -1;
        }
    }
    return 0;
}


