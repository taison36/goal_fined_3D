//
// Created by Tykhon Korol on 10.01.26.
//

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <vector>

namespace ws {
    class WebSocketClient {
        class Impl;
        std::unique_ptr<Impl> ws_impl;



    public:
        using OnOpen = std::function<void()>;
        using OnMessage = std::function<void(std::string const &)>;
        using OnError = std::function<void(std::string const &)>;

        explicit WebSocketClient(const std::string &url,
                                 const OnOpen &onOpen = nullptr,
                                 const OnMessage &onMessage = nullptr,
                                 const OnError &onError = nullptr);

        ~WebSocketClient();

        void start() const;

        void send(const std::string &msg) const;

        void stop() const;

        std::string waitResponse() const;

        [[nodiscard]] bool isConnected() const;
    };
}
#endif //WEBSOCKET_H
