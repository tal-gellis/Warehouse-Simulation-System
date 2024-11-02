#include "../include/Order.h"

Order::Order(int id, int customerId, int distance): id(id) , customerId(customerId) ,distance(distance) , status(OrderStatus::PENDING) , collectorId(NO_VOLUNTEER) , driverId(NO_VOLUNTEER) {};
int Order::getDistance() const {return distance;}
int Order::getId() const {return id;}
int Order::getCustomerId() const {return customerId;}
void Order::setStatus(OrderStatus status1) {status=status1;};
void Order::setCollectorId(int collectorId1) {collectorId=collectorId1;}
void Order::setDriverId(int driverId1) {driverId=driverId1;}
int Order::getCollectorId() const {return collectorId;}
int Order:: getDriverId() const {return driverId;}
OrderStatus Order:: getStatus() const {return status;}
string Order:: toStringStatus(OrderStatus status) const {
        switch (status) {
            case OrderStatus::PENDING:
                return "Pending";
            case OrderStatus::COLLECTING:
                return "Collecting";
            case OrderStatus::DELIVERING:
                return "Delivering";
            case OrderStatus::COMPLETED:
                return "Completed";
        }
        return "DEFAULT";
}
const string Order:: toString() const{
    string output = "OrderId: " + std::to_string(getId());
    output.append("\nOrderStatus: " + toStringStatus(getStatus()));
    output.append("\nCustomerID: " + std::to_string(getCustomerId()));
    if(getCollectorId()==NO_VOLUNTEER)
        output.append("\nCollector: None");
    else
        output.append("\nCollector: " + std::to_string(getCollectorId()));
    if(getDriverId()==NO_VOLUNTEER)
        output.append("\nDriver: None");
    else 
        output.append("\nDriver: " + std::to_string(getDriverId()));
    return output;
}

