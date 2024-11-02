#include "../include/Volunteer.h"
Volunteer::Volunteer(int id, const string &name):completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(id),name(name){};
Volunteer::~Volunteer(){};
const string& Volunteer::getName() const {return name;}
int Volunteer::getId() const {return id;}
int Volunteer::getActiveOrderId() const {return activeOrderId;}
int Volunteer:: getCompletedOrderId() const {return completedOrderId;}
bool Volunteer::isBusy() const {return (getActiveOrderId() != NO_ORDER);}

// ----------------------------- Collector section
CollectorVolunteer::CollectorVolunteer(int id,const string& name, int coolDown): Volunteer(id,name), coolDown(coolDown),timeLeft(0) {};
CollectorVolunteer * CollectorVolunteer:: clone() const {return new CollectorVolunteer(*this);}
void CollectorVolunteer::step() {
    if (timeLeft > 0) {
        timeLeft-- ;
        if (timeLeft == 0) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    } 
}
int CollectorVolunteer::getCoolDown() const {return coolDown;}
int CollectorVolunteer::getTimeLeft() const {return timeLeft;}
bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft>0) {
        timeLeft--;
    }
    if (timeLeft == 0) {return true;}
    return false;
}
bool CollectorVolunteer::hasOrdersLeft() const {return true;}
bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    OrderStatus stat = order.getStatus();
    return((!isBusy()) && stat==OrderStatus::PENDING && hasOrdersLeft());
}
void CollectorVolunteer::acceptOrder(const Order &order) {
    timeLeft = coolDown;
    activeOrderId = order.getId();
} 
string CollectorVolunteer::toString() const {
    string output = "VolunteerID: " + std::to_string(getId());
    if (isBusy()) {
        output.append("\nisBusy: True ");
        output.append("\nOrderID: " + std::to_string(getActiveOrderId()));
        output.append("\nTimeLeft: " + std::to_string(getTimeLeft()));
        }
    else {
        output.append("\nisBusy: False \n");
        output.append("OrderID: None");
        output.append("\nTimeLeft: None");
        }
    output.append("\nOrders Left: No Limit");
    return output;
}

//---- Limited Collector section
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string& name, int coolDown ,int maxOrders): CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders) {};
LimitedCollectorVolunteer * LimitedCollectorVolunteer::clone() const {return new LimitedCollectorVolunteer(*this);}
bool LimitedCollectorVolunteer:: hasOrdersLeft() const {return (ordersLeft != 0) ;}
bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const {
    OrderStatus stat = order.getStatus();
    return((!isBusy()) && stat==OrderStatus::PENDING && hasOrdersLeft());
}
void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
} 
int LimitedCollectorVolunteer:: getMaxOrders() const {return maxOrders;}
int LimitedCollectorVolunteer:: getNumOrdersLeft() const {return ordersLeft;}
string LimitedCollectorVolunteer:: toString() const {
    string output = "VolunteerID: " + std::to_string(getId());
    if (isBusy()) {
        output.append("\nisBusy: True ");
        output.append("\nOrderID: " + std::to_string(getActiveOrderId()));
        output.append("\nTimeLeft: " + std::to_string(getTimeLeft()));
        }
    else {
        output.append("\nisBusy: False \n");
        output.append("OrderID: None");
        output.append("\nTimeLeft: None");
        }
    output.append("\nOrders Left: " + std::to_string(ordersLeft));
    return output;
}

// ----------------------------- Driver Section
DriverVolunteer::DriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep):Volunteer(id,name),maxDistance(maxDistance), distancePerStep(distancePerStep),distanceLeft(0){};
DriverVolunteer * DriverVolunteer:: clone() const {return new DriverVolunteer(*this);}
int DriverVolunteer::getDistanceLeft() const {return distanceLeft;}
int DriverVolunteer::getMaxDistance() const {return maxDistance;} 
int DriverVolunteer::getDistancePerStep() const {return distancePerStep;} 
bool DriverVolunteer:: decreaseDistanceLeft() {
    distanceLeft=(distanceLeft-distancePerStep);
    if(distanceLeft<=0){
        distanceLeft=0;
        return true;
    }
    else
        return false;
}
bool DriverVolunteer:: hasOrdersLeft() const {return true;};
bool DriverVolunteer::canTakeOrder(const Order &order) const {
    OrderStatus stat = order.getStatus();
    return((!isBusy()) && stat==OrderStatus::COLLECTING &&  hasOrdersLeft() && (order.getDistance() <= maxDistance));
    }
void DriverVolunteer::acceptOrder(const Order &order) {
   distanceLeft = order.getDistance();
   activeOrderId = order.getId();
   }
void DriverVolunteer::step() {
    if(isBusy()){
        if(decreaseDistanceLeft()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}
string DriverVolunteer::toString() const {
    string output = "VolunteerID: " + std::to_string(getId());
    if (isBusy()) {
        output.append("\nisBusy: True");
        output.append("\nOrderID: " + std::to_string(getActiveOrderId()));
        output.append("\nDistanceLeft: " + std::to_string(getDistanceLeft()));
        }
    else {
        output.append("\nisBusy: False \n");
        output.append("OrderID: None");
        output.append("\nDistanceLeft: None");
        }
    output.append("\nOrders Left: No Limit");
    return output;
}

//---- Limited Driver section
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders) , ordersLeft(maxOrders){};
LimitedDriverVolunteer* LimitedDriverVolunteer:: clone() const {return new LimitedDriverVolunteer(*this);};
int LimitedDriverVolunteer:: getMaxOrders() const {return maxOrders;};
int LimitedDriverVolunteer::getNumOrdersLeft() const {return ordersLeft;};
bool LimitedDriverVolunteer:: hasOrdersLeft() const {return (ordersLeft != 0) ;}
bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const {
     OrderStatus stat = order.getStatus();
    return((!isBusy()) && stat==OrderStatus::COLLECTING &&  hasOrdersLeft() && (order.getDistance() <= getMaxDistance()));
    }
void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
   }
string LimitedDriverVolunteer::toString() const {
    string output = "VolunteerID: " + std::to_string(getId());
    if (isBusy()) {
        output.append("\nisBusy: True");
        output.append("\nOrderID: " + std::to_string(getActiveOrderId()));
        output.append("\nDistanceLeft: " + std::to_string(getDistanceLeft()));
        }
    else {
        output.append("\nisBusy: False \n");
        output.append("OrderID: None");
        output.append("\nDistanceLeft: None");
        }
    output.append("\nOrders Left: " + std::to_string(ordersLeft));
    return output;
}

LimitedDriverVolunteer::~LimitedDriverVolunteer(){};
LimitedCollectorVolunteer::~LimitedCollectorVolunteer(){};
CollectorVolunteer::~CollectorVolunteer(){};
DriverVolunteer::~DriverVolunteer(){};