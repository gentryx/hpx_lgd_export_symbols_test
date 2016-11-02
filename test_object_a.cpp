#include "test_component.hpp"

typedef LibGeoDecomp::HPXReceiver<std::vector<double> > ReceiverType1;

// uncomment this to make the code work:
// LIBGEODECOMP_REGISTER_HPX_COMM_TYPE(double)

void ping_a()
{
    std::string basename = "ping_a";

    int rank = hpx::get_locality_id();
    int size = hpx::get_num_localities().get();

    int leftNeighbor  = (rank - 1 + size) % size;
    int rightNeighbor = (rank + 1       ) % size;

    std::stringstream buf;
    buf << rank;
    std::string name      = basename + "/" + buf.str();

    buf.str("");
    buf << leftNeighbor;
    std::string leftName  = basename + "/" + buf.str();

    buf.str("");
    buf << rightNeighbor;
    std::string rightName = basename + "/" + buf.str();

    std::shared_ptr<ReceiverType1> receiver = ReceiverType1::make(name).get();

    hpx::id_type leftID  = ReceiverType1::find(leftName ).get();
    hpx::id_type rightID = ReceiverType1::find(rightName).get();

    std::vector<double> sendVec;
    sendVec.push_back(rank);
    hpx::apply(ReceiverType1::receiveAction(), leftID,  10, sendVec);
    hpx::apply(ReceiverType1::receiveAction(), rightID, 11, sendVec);

    hpx::unregister_with_basename(name, 0);
}
