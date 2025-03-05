#include <systemc>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
// #include <ctime>
#include <chrono>
#include <cstdint>

using namespace sc_core;

std::mutex cout_mutex;

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

class ThreadSafeEventIf : public sc_interface {
    virtual void notify(sc_time delay = SC_ZERO_TIME) = 0;
    virtual const sc_event &default_event(void) const = 0;
protected:
    virtual void update(void) = 0;
};

class ThreadSafeEvent : public sc_prim_channel, public ThreadSafeEventIf {
public:
    ThreadSafeEvent(const char *name = "TheEvent"): event(name) {}

    void notify(sc_time delay = SC_ZERO_TIME) {
        this->delay = delay;
        {
            const std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "ThreadSafeNotify called\n";
        }
        
        async_request_update();
    }

    const sc_event &default_event(void) const {
        return event;
    }
protected:
    virtual void update(void) {
        event.notify(delay);
        const std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "ThreadSafeUpdate called\n";
    }
    sc_event event;
    sc_time delay;
};

SC_MODULE(MainModule)
{
public:
    SC_CTOR(MainModule)
    {
        SC_THREAD(main);
        sensitive << event;
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(eventTriggered);
        sensitive << event;
        dont_initialize();
    }
private:
    // TODO: Use this to pace number of external event notifies, to synchronise with systemC time.
    void main() {
        std::cout << "MainModule sc_thread started\n";
        while(1)
        {
            wait();
            // std::cout << "Got event in main() @" << sc_time_stamp() << std::endl;
        }
    }

    void eventTriggered() {
        const std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Got event in sc_method  @" << sc_time_stamp().to_seconds() << " s" << std::endl;
    }

public:
    ThreadSafeEvent event;
    sc_in<bool> clk;
};

void *externalHostThread(void *arg) {
    std::cout << "External host thread started" << std::endl;
    MainModule* theModule = (MainModule*)(arg);
    // send events periodically
    for(unsigned i=0; i<1000000; i++) {
        if(i%10000 == 0) {
            {
                const std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Event " << i/10000 << " notified from an external host thread @ " << timeSinceEpochMillisec() << std::endl;
            }
            
            theModule->event.notify();
        }
    }
    return 0;
}

int sc_main(int argc, char *argv[])
{
    sc_clock clk("clk", 1, SC_MS, 0.5);
    MainModule theModule("theModule");
    theModule.clk(clk);

    pthread_t thread;
    pthread_create(&thread, NULL, externalHostThread, &theModule);

    sc_time sim_time(1000000,SC_SEC);
    std::cout << "Before sc_start @" << sc_time_stamp() << std::endl;
    sc_start(sim_time);
    std::cout << "Before end @" << sc_time_stamp() << std::endl;
    return 0;
}