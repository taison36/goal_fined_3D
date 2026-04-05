//
// Created by Tykhon Korol on 10.01.26.
//

#include "WebSocketClient.h"
#include "WebSocketClient.h"

#include <iostream>
#include "server_ws.hpp"
#include "client_ws.hpp"

using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

static ws::WebSocketClient::OnOpen defaultOnOpen() {
    return []() {
        std::cout << "[WS] Connected\n";
    };
}


static ws::WebSocketClient::OnError defaultOnError() {
    return [](const std::string &err) {
        std::cerr << "[WS] Client: Error: " << err << "\n";
    };
}

static ws::WebSocketClient::OnError defaultOnMessage() {
    return [](const std::string &msg) {
        //std::cout << "[WS] Message received: " << msg << "\n";
    };
}


class ws::WebSocketClient::Impl {
    WsClient client;
    std::shared_ptr<WsClient::Connection> shared_connection = nullptr;
    OnOpen openCb;
    OnMessage msgCb;
    OnError errCb;

    std::mutex msg_mtx;
    std::condition_variable msg_cv;
    std::string last_message;
    bool got_message = false;

public:
    Impl(const std::string &url,
         OnOpen openCb,
         OnMessage msgCb,
         OnError errCb);

    ~Impl() = default;

    void start();

    void stop();

    [[nodiscard]] bool isConnected() const;

    void send(const std::string &) const;

    std::string waitResponse();
};

void ws::WebSocketClient::Impl::start() {
    client.start();
}

void ws::WebSocketClient::Impl::stop() {
    client.stop();
    shared_connection = nullptr;
}

void ws::WebSocketClient::Impl::send(const std::string &message) const {
    if (isConnected()) {
        shared_connection->send(message,
                                [](const SimpleWeb::error_code &ec) {
                                    if (ec) std::cerr << "Send error: " << ec.message() << "\n";
                                });
    }
}

bool ws::WebSocketClient::Impl::isConnected() const {
    return shared_connection ? true : false;
}

std::string ws::WebSocketClient::Impl::waitResponse() {
    std::unique_lock<std::mutex> lock(msg_mtx);
    msg_cv.wait(lock, [this]() { return got_message; });
    got_message = false;
    return last_message;
}

ws::WebSocketClient::Impl::Impl(const std::string &url,
                                OnOpen openCb = nullptr,
                                OnMessage msgCb = nullptr,
                                OnError errCb = nullptr): client(WsClient(url)) {
    if (!openCb) openCb = defaultOnOpen();
    if (!msgCb) msgCb = defaultOnMessage();
    if (!errCb) errCb = defaultOnError();

    this->openCb = openCb;
    this->msgCb = msgCb;
    this->errCb = errCb;


    client.on_message = [this](std::shared_ptr<WsClient::Connection> connection,
                               std::shared_ptr<WsClient::InMessage> in_message) {
        {
            std::lock_guard<std::mutex> lock(msg_mtx);
            this->last_message = in_message->string();
            got_message = true;
        }
        msg_cv.notify_one();

        this->msgCb(last_message);
    };

    client.on_open = [this](std::shared_ptr<WsClient::Connection> connection) {
        shared_connection = connection;
        this->openCb();
    };

    client.on_close = [this](std::shared_ptr<WsClient::Connection> /*connection*/, int status,
                             const std::string &reason) {
        std::cout << "Client: Closed connection with status code " << status << std::endl;
        shared_connection = nullptr;
    };

    client.on_error = [this](std::shared_ptr<WsClient::Connection>, const SimpleWeb::error_code &ec) {
        this->errCb(ec.to_string());
    };
}

ws::WebSocketClient::WebSocketClient(const std::string &url,
                                     const OnOpen &onOpen,
                                     const OnMessage &onMessage,
                                     const OnError &onError) : ws_impl(
    std::make_unique<Impl>(url, onOpen, onMessage, onError)) {
}

std::string ws::WebSocketClient::waitResponse() const {
    return ws_impl->waitResponse();
}

ws::WebSocketClient::~WebSocketClient() = default;

void ws::WebSocketClient::start() const {
    ws_impl->start();
}

void ws::WebSocketClient::send(std::string const &msg) const {
    ws_impl->send(msg);
}

void ws::WebSocketClient::stop() const {
    ws_impl->stop();
}

bool ws::WebSocketClient::isConnected() const {
    return ws_impl->isConnected();
}
