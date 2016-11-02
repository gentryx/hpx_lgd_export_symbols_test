#include <hpx/include/components.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/lcos/broadcast.hpp>
#include <hpx/lcos/local/receive_buffer.hpp>
#include <hpx/runtime/get_ptr.hpp>
#include <vector>

#define LIBGEODECOMP_REGISTER_HPX_COMM_TYPE(CARGO)                      \
    typedef LibGeoDecomp::HPXReceiver<std::vector<CARGO> >::receiveAction DummyReceiver_vector_ ## CARGO ## _ReceiveAction; \
    HPX_REGISTER_ACTION_DECLARATION(DummyReceiver_vector_ ## CARGO ## _ReceiveAction); \
    HPX_REGISTER_ACTION(DummyReceiver_vector_ ## CARGO ## _ReceiveAction);     \
    HPX_REGISTER_BROADCAST_APPLY_ACTION_DECLARATION(DummyReceiver_vector_ ## CARGO ## _ReceiveAction); \
    HPX_REGISTER_BROADCAST_APPLY_ACTION(DummyReceiver_vector_ ## CARGO ## _ReceiveAction); \
    typedef hpx::components::managed_component<LibGeoDecomp::HPXReceiver<std::vector<CARGO>> > receiver_type_vector_ ## CARGO; \
    HPX_REGISTER_COMPONENT(receiver_type_vector_ ## CARGO , DummyReceiver_vector_ ## CARGO);

namespace LibGeoDecomp {

template<typename CARGO, typename BUFFER=hpx::lcos::local::receive_buffer<CARGO> >
class HPXReceiver : public hpx::components::managed_component_base<HPXReceiver<CARGO> >
{
public:
    static hpx::future<std::shared_ptr<HPXReceiver> > make(const std::string& name, std::size_t rank = 0)
    {
        return hpx::new_<HPXReceiver>(hpx::find_here()).then(
            [name, rank](hpx::future<hpx::id_type> idFuture)
            {
                hpx::id_type id = idFuture.get();
                hpx::future<bool> f = hpx::register_with_basename(name, id, rank);
                return f.then(
                    [id](hpx::future<bool>)
                    {
                        return hpx::get_ptr<HPXReceiver>(id);
                    });
            });
    }

    static hpx::future<hpx::id_type> find(const std::string& name)
    {
        std::vector<hpx::future<hpx::id_type> > ids = hpx::find_all_from_basename(name, 1);
        if (ids.size() != 1) {
            throw std::logic_error("Unexpected amount of HPXReceivers found in AGAS, expected exactly 1");
        }

        return std::move(ids[0]);
    }

    void receive(std::size_t step, CARGO val)
    {
        // buffer.store_received(step, std::move(val));
        std::cout << "got step " << step << " and cargo size " << val.size() << "\n";
    }
    HPX_DEFINE_COMPONENT_DIRECT_ACTION(HPXReceiver, receive, receiveAction);
};


}
