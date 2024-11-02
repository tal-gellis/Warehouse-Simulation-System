#include "../include/Action.h"
#include <iostream>
extern WareHouse* backup;

BaseAction::BaseAction(): errorMsg("Error in action."), status(ActionStatus::ERROR) {};
BaseAction::~BaseAction() {};
ActionStatus BaseAction:: getStatus() const {return status;}
void BaseAction:: complete() {status = ActionStatus::COMPLETED;}
void BaseAction:: error(string errorMsg) {
    status = ActionStatus::ERROR;
    std::cout<<"Error: "<<errorMsg<<std::endl;
}
string BaseAction:: getErrorMsg() const {return errorMsg;}
string statusString(ActionStatus status) {
    if (status == ActionStatus::COMPLETED) {
        return "COMPLETED";
    }
    return "ERROR";
}
string typeString(CustomerType type) {
    if (type == CustomerType::Soldier) {
        return "Soldier";
    }
    return "Civilian";
}

SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps) {};
void SimulateStep:: act(WareHouse &wareHouse) {
    for (int i = 0 ; i < numOfSteps ; i++) {
        wareHouse.makeStep();
    }
    complete();
    wareHouse.addAction(this->clone());
}
string SimulateStep:: toString() const {
    return ("simulateStep " + std::to_string(numOfSteps) + " COMPLETED");
}
SimulateStep* SimulateStep:: clone() const {return new SimulateStep(*this);}

AddOrder::AddOrder(int id): customerId(id) {};
void AddOrder:: act(WareHouse& warehouse) {
    if (customerId > warehouse.getCustomerCount() || customerId < 0) {
        error("Cannot place this order.");
    }
    else {
        Customer& customer = warehouse.getCustomer(customerId);
        int orderId = warehouse.getOrderCount(); 
        if (customer.addOrder(orderId) >= 0) { 
            Order* order = new Order(orderId,customerId,customer.getCustomerDistance());
            warehouse.addOrder(order);
            this->complete();
        }
        else {
            error("Cannot place this order.");
        }
    }
    warehouse.addAction(this->clone());
}
string AddOrder::toString() const {
    return string("order " + std::to_string(customerId) +" " + statusString(getStatus()));
}
AddOrder * AddOrder:: clone() const {return new AddOrder(*this);}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
customerName(customerName),customerType(determineType(customerType)),distance(distance),maxOrders(maxOrders) {};
void AddCustomer:: act(WareHouse &wareHouse){
    
    if(customerType==CustomerType::Civilian)
    {
        CivilianCustomer* customer = new CivilianCustomer(wareHouse.getCustomerCount(),customerName,distance,maxOrders);
        wareHouse.addCustomer(customer);
    }
    else if (customerType==CustomerType::Soldier)
    {
        SoldierCustomer* customer = new SoldierCustomer(wareHouse.getCustomerCount(),customerName,distance,maxOrders);
        wareHouse.addCustomer(customer);
    }
    complete();
    wareHouse.addAction(this->clone());
    
}
string AddCustomer::toString() const {
    return string("Customer " + customerName +" " + typeString(customerType) + statusString(getStatus()));
}
AddCustomer * AddCustomer:: clone() const {return new AddCustomer(*this);}
CustomerType AddCustomer::determineType(const string &customerType) {
    if (customerType == "civilian") {
        return CustomerType::Civilian;
    }
    else return CustomerType::Soldier;
}

PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId) {};
void PrintCustomerStatus:: act(WareHouse& warehouse) {
    int lastCustomer = warehouse.getCustomerCount();
    if (customerId >= lastCustomer || customerId < 0) {
        error("Customer doesn't exist");
    }
    else{
        int customId = warehouse.getCustomer(customerId).getId();
        string toPrint = string("CustomerID: " + std::to_string(customId));
        for (int i : warehouse.getCustomer(customerId).getOrdersIds()) {
             Order order = warehouse.getOrder(i);
            toPrint.append("\nOrderID: " + std::to_string(i));
            toPrint.append("\nOrderStatus: " + order.toStringStatus(order.getStatus()));
        }
        toPrint.append("\nnumOrdersLeft: " + std::to_string(warehouse.getCustomer(customerId).numOrdersLeft()));
        std::cout<<toPrint<<std::endl;
        complete();
    }
    warehouse.addAction(this->clone());
}
PrintCustomerStatus* PrintCustomerStatus:: clone() const {return new PrintCustomerStatus(*this);};
string PrintCustomerStatus :: toString() const {
    return string("customerStatus " + std::to_string(customerId) + " " +statusString(getStatus()));
}

PrintVolunteerStatus::PrintVolunteerStatus(int id): volunteerId(id) {};
void PrintVolunteerStatus:: act(WareHouse& warehouse) {
    if (warehouse.getVolunteer(volunteerId).getId() == -1) {
        error("Volunteer doesn't exist");
    }
    else {
        string toPrint = warehouse.getVolunteer(volunteerId).toString();
        std::cout<<toPrint<<std::endl;
        complete();
    }
    warehouse.addAction(this->clone()); 
}
PrintVolunteerStatus * PrintVolunteerStatus:: clone() const {return new PrintVolunteerStatus(*this);};
string PrintVolunteerStatus:: toString() const {
    return string("volunteerStatus " + std::to_string(volunteerId) + " " + statusString(getStatus()));
}

PrintOrderStatus::PrintOrderStatus(int id):orderId(id){};
void PrintOrderStatus::act(WareHouse &wareHouse){
    Order order = wareHouse.getOrder(orderId);
    if (order.getId()==(-1)){
        error("Order doesn't exist");
    }
    else{
        std::cout << order.toString() << std::endl;
        complete();
    }
    wareHouse.addAction(this->clone());
}
PrintOrderStatus * PrintOrderStatus:: clone() const {return new PrintOrderStatus(*this);}
string PrintOrderStatus::toString() const {
    return string("Order Status " + std::to_string(orderId) +" " + statusString(getStatus()));
}

PrintActionsLog::PrintActionsLog(){};
void PrintActionsLog::act(WareHouse &wareHouse){
    vector<BaseAction*> actions = wareHouse.getActions();
    for(BaseAction* action : actions){
        std::cout <<action->toString()<< std::endl;
    }
    complete();
    wareHouse.addAction(this->clone());
}
PrintActionsLog* PrintActionsLog:: clone() const {return new PrintActionsLog(*this);};
string PrintActionsLog::toString() const {
    return ("log COMPLETED");
}

Close::Close() {};
void Close::act(WareHouse& warehouse) {
    for(Order *order : warehouse.getPendingOrders()) {
        string s("OrderID: " + std::to_string(order->getId()) + ", CustomerId: " + std::to_string(order->getCustomerId()) + ", OrderStatus: " + order->toStringStatus(order->getStatus()));
        std::cout<<s<<std::endl;
    }
    for(Order *order : warehouse.getInProcessOrders()) {
        string s("OrderID: " + std::to_string(order->getId()) + ", CustomerId: " + std::to_string(order->getCustomerId()) + ", OrderStatus: " + order->toStringStatus(order->getStatus()));
        std::cout<<s<<std::endl;
    }
    for(Order *order : warehouse.getCompletedOrders()) {
        string s("OrderID: " + std::to_string(order->getId()) + ", CustomerId: " + std::to_string(order->getCustomerId()) + ", OrderStatus: " + order->toStringStatus(order->getStatus()));
        std::cout<<s<<std::endl;
    }

    complete();
    warehouse.close();
}
Close * Close:: clone() const {return new Close(*this);};
string Close:: toString() const {
    return string("close COMPLETED");
}


BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse:: act(WareHouse& warehouse) {
    if (!backup) {
        backup = new WareHouse(warehouse);
    }
    else{
        *backup = warehouse;
    }
    complete();
    warehouse.addAction(this->clone());
}
BackupWareHouse* BackupWareHouse:: clone() const {return new BackupWareHouse(*this);};
string BackupWareHouse:: toString() const {
    return string("backup " + statusString(getStatus()));
}


RestoreWareHouse::RestoreWareHouse() {};
void RestoreWareHouse:: act(WareHouse &warehouse) {
    if (!backup) {
        error("No backup available");
    }
    else {
        warehouse = *backup;
    }
    complete();
    warehouse.addAction(this->clone());
}
RestoreWareHouse* RestoreWareHouse:: clone() const {return new RestoreWareHouse(*this);}
string RestoreWareHouse:: toString() const {
    return string("restore " + statusString(getStatus()));
}


SimulateStep::~SimulateStep(){};
AddOrder::~AddOrder(){};
AddCustomer::~AddCustomer(){};
PrintOrderStatus::~PrintOrderStatus(){};
PrintCustomerStatus::~PrintCustomerStatus(){};
PrintVolunteerStatus::~PrintVolunteerStatus(){};
PrintActionsLog::~PrintActionsLog(){};
Close::~Close(){};
BackupWareHouse::~BackupWareHouse(){};
RestoreWareHouse::~RestoreWareHouse(){};