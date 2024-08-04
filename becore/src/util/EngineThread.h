#include <thread>
#include <atomic>
#include "src/nucleo/engine.hpp"
#include "src/interface/ui.hpp"

namespace Bubble
{
    namespace Util
    {
        class EngineThread {
        public:
            EngineThread() : running(false) {}

            void start(Bubble::Nucleo::Engine* engine) {
                running = true;
                thread = std::thread(&EngineThread::atualizar, this, engine);
            }

            void stop() {
                running = false;
                if (thread.joinable()) {
                    thread.join();
                }
            }

            bool isRunning() const {
                return running;
            }

        private:
            std::thread thread;
            std::atomic<bool> running;

            void atualizar(Bubble::Nucleo::Engine* engine) {
                while (running) {
                    engine->atualizar();  // Atualize a engine aqui
                }
            }
        };
        class UIThread
        {
        public:
            UIThread() : running(false) {}

            void start(Bubble::Interface::UI* ui) {
                running = true;
                thread = std::thread(&UIThread::atualizar, this, ui);
            }

            void stop() {
                running = false;
                if (thread.joinable()) {
                    thread.join();
                }
            }

            bool isRunning() const {
                return running;
            }

        private:
            std::thread thread;
            std::atomic<bool> running;

            void atualizar(Bubble::Interface::UI* ui) {
                while (running) {
                    ui->pollevents();
                    ui->renderizar();
                }
            }
        };
    }
}