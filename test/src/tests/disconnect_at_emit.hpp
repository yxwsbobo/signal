#ifndef TESTS_DISCONNECT_AT_EMIT_HPP
#define TESTS_DISCONNECT_AT_EMIT_HPP

//Check that disconnecting from signal during emit() call doesn't lead to
//segfault.

#include <fgl/signal.hpp>

namespace tests::disconnect_at_emit
{

using signal = fgl::signal<void()>;

struct receiver
{
    private:
        struct slot
        {
            void operator()()
            {
                self.on_event();
            }

            receiver& self;
        };

    public:
        receiver(signal& sig):
            sig_(sig),
            connection_(sig.connect(slot{*this}))
        {
        }

    private:
        void on_event()
        {
            ++call_count_;

            if(call_count_ == 1)
            {
                connection_.close();
                sig_.emit();
            }
        }

    private:
        signal& sig_;
        unsigned int call_count_ = 0;
        signal::owning_connection<slot> connection_;
};

bool test()
{
    signal sig;
    receiver receiver0{sig};
    auto conn = sig.connect([](){});

    sig.emit();

    return true;
}

} //namespace

#endif
