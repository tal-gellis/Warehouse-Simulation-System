#include "../include/WareHouse.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

//--- RULE OF 5
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if(this != &other){
        isOpen = other.isOpen;
        orderCounter = other.orderCounter;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();

        for (Order* order : pendingOrders) {
            delete order;
        }
        pendingOrders.clear();

        for (Order* order : inProcessOrders) {
            delete order;
        }
        inProcessOrders.clear();

        for (Order* order : completedOrders) {
            delete order;
        }
        completedOrders.clear();

        for (Customer* customer : customers) {
            delete customer;
        }
        customers.clear();
        // Copy actionsLog
        for (const BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        // Copy volunteers
        for (const Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }

        // Copy pendingOrders
        for (const Order* order : other.pendingOrders) {
            pendingOrders.push_back(new Order(*order));
        }

        // Copy inProcessOrders
        for (const Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*order));
        }

        // Copy completedOrders
        for (const Order* order : other.completedOrders) {
            completedOrders.push_back(new Order(*order));
        }

        // Copy customers
        for (const Customer* customer : other.customers) {
            customers.push_back(customer->clone());
        }
    }

    return *this;
}
WareHouse::WareHouse(const WareHouse& other):
isOpen(other.isOpen),actionsLog(), volunteers(), pendingOrders(),inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter) {
        // Copy actionsLog
        for (const BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        // Copy volunteers
        for (const Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }

        // Copy pendingOrders
        for (const Order* order : other.pendingOrders) {
            pendingOrders.push_back(new Order(*order));
        }

        // Copy inProcessOrders
        for (const Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*order));
        }

        // Copy completedOrders
        for (const Order* order : other.completedOrders) {
            completedOrders.push_back(new Order(*order));
        }

        // Copy customers
        for (const Customer* customer : other.customers) {
            customers.push_back(customer->clone());
        }
}
WareHouse::~WareHouse() {
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();

        for (Order* order : pendingOrders) {
            delete order;
        }
        pendingOrders.clear();

        for (Order* order : inProcessOrders) {
            delete order;
        }
        inProcessOrders.clear();

        for (Order* order : completedOrders) {
            delete order;
        }
        completedOrders.clear();

        for (Customer* customer : customers) {
            delete customer;
        }
        customers.clear();
        delete NoCustomer;
        delete NoVolunteer;
        delete NoOrder;
        NoCustomer = nullptr;
        NoVolunteer = nullptr;
        NoOrder = nullptr;
}
WareHouse::WareHouse(WareHouse&& other):
    isOpen(move(other.isOpen)),
    actionsLog(move(other.actionsLog)), volunteers(move(other.volunteers)),
    pendingOrders(move(other.pendingOrders)),inProcessOrders(move(other.inProcessOrders)),completedOrders(move(other.completedOrders)),
    customers(move(other.customers)),
    customerCounter(move(other.customerCounter)),volunteerCounter(move(other.volunteerCounter)),orderCounter(move(other.orderCounter)) {}
WareHouse& WareHouse::operator=(WareHouse&& other){
    if(this != &other) {
        isOpen=move(other.isOpen);
        actionsLog=move(other.actionsLog);
        volunteers=move(other.volunteers);
        pendingOrders=move(other.pendingOrders);
        inProcessOrders=move(other.inProcessOrders);
        completedOrders=move(other.completedOrders);
        customers=move(other.customers);

        customerCounter=move(other.customerCounter);
        volunteerCounter=move(other.volunteerCounter);
        orderCounter=move(other.orderCounter);        

        other.isOpen = false;
        other.orderCounter = 0;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        // Clear vectors or perform appropriate clearing for your data members
        other.volunteers.clear();
        other.customers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.actionsLog.clear();
    }
    return *this;
}

//--- RULE OF 5 END

//--constractor

WareHouse::WareHouse(const string &configFilePath) :
 isOpen(false),actionsLog(), volunteers(), pendingOrders(),inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0) {
    ifstream configFile(configFilePath);
    
    
    if (!configFile.is_open()) {
        cerr << "Error: Unable to open configuration file: " << configFilePath << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(configFile, line)) {
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "customer") {
            string name, customerType;
            int distance, maxOrders;
            iss >> name >> customerType >> distance >> maxOrders;
            if(customerType == "soldier") {
                customers.push_back(new SoldierCustomer(customerCounter, name, distance, maxOrders));
                customerCounter++;
            }
            else if(customerType == "civilian") {
                customers.push_back(new CivilianCustomer(customerCounter, name, distance, maxOrders));
                customerCounter++;
            }
        } else if (type == "volunteer") {
            string name, volunteerRole;
            iss >> name >> volunteerRole;

            if (volunteerRole == "collector") {
                int cooldown;
                iss >> cooldown;
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, cooldown));
                volunteerCounter++;
            } else if (volunteerRole == "limited_collector") {
                int cooldown, maxOrders;
                iss >> cooldown >>maxOrders;
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, cooldown, maxOrders));
                volunteerCounter++;
            } else if (volunteerRole == "driver") {
                int maxDistance, distancePerStep;
                iss >> maxDistance >> distancePerStep;
                volunteers.push_back(new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep));
                volunteerCounter++;
            } else if (volunteerRole == "limited_driver") {
                int maxDistance, distancePerStep, maxOrders;
                iss >> maxDistance >> distancePerStep >> maxOrders;
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders));
                volunteerCounter++;
            } 
        }
    }

    configFile.close();
}
//--Implemention
void WareHouse::start() {
    isOpen=true;
    std::cout << "Warehouse is open!"<<std::endl;
    while (isOpen) {
        
        string input;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        string orderType;
        iss>>orderType;

        if(orderType=="step"){
            int numOfSteps;
            iss >> numOfSteps;
            SimulateStep action = SimulateStep(numOfSteps);
            action.act(*this);
        }
        if(orderType=="order"){
            int orderId;
            iss >> orderId;
            AddOrder action = AddOrder(orderId);
            action.act(*this);
        }
        if(orderType=="customer"){
            string name, customerType;
            int distance, maxOrders;
            iss >> name >> customerType >> distance >> maxOrders;
            AddCustomer action = AddCustomer(name,customerType,distance,maxOrders);
            action.act(*this);
        }
        if(orderType=="orderStatus"){
            int orderId;
            iss >> orderId;
            PrintOrderStatus action = PrintOrderStatus(orderId);
            action.act(*this);
        }
        if(orderType=="customerStatus"){
            int customerId;
            iss >> customerId;
            PrintCustomerStatus action = PrintCustomerStatus(customerId);
            action.act(*this);
        }
        if(orderType=="volunteerStatus"){
            int Id;
            iss >> Id;
            PrintVolunteerStatus action = PrintVolunteerStatus(Id);
            action.act(*this);
        }
        if(orderType=="log"){
            PrintActionsLog action = PrintActionsLog();
            action.act(*this);
        }
        if(orderType=="close"){
            Close action = Close();
            action.act(*this);
        }
        if(orderType=="backup"){
            BackupWareHouse action = BackupWareHouse();
            action.act(*this);
        }
        if(orderType=="restore"){
            RestoreWareHouse action = RestoreWareHouse();
            action.act(*this);
        }
    }
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
    orderCounter++;
    }
void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}

Customer& WareHouse:: getCustomer(int customerId) const {
    for(Customer* Customer: customers){
        if((*Customer).getId()==customerId)
            return(*Customer);
    }
    
    return(*NoCustomer);
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const{
      for(Volunteer* volunteer: volunteers){
            if(volunteer->getId()==volunteerId) {
                 return(*volunteer);
            }
    }
    return(*NoVolunteer);
}

Order& WareHouse:: getOrder(int orderId) const{
    for(Order* Order: pendingOrders){
        if((*Order).getId()==orderId)
            return(*Order);
    };
    for(Order* Order: inProcessOrders){
        if((*Order).getId()==orderId)
            return(*Order);
    };
    for(Order* Order: completedOrders){
       if((*Order).getId()==orderId)
            return(*Order);
    };
    return(*NoOrder); 
}

const vector<BaseAction*>& WareHouse:: getActions() const{
    return(actionsLog);}
void WareHouse:: close() {isOpen=false;}
void WareHouse:: open() {isOpen = true;}

//functions added by us:
void WareHouse:: addCustomer(Customer* customer) {
    customers.push_back(customer);
    customerCounter++;
}
void WareHouse:: addVolunteer(Volunteer *volunteer) {
    volunteers.push_back(volunteer);
    volunteerCounter++ ;
}


int WareHouse:: getOrderCount() const {return orderCounter;}
int WareHouse:: getCustomerCount() const {return customerCounter;}
int WareHouse:: getVolunteerCount() const {return volunteerCounter;}

const vector<Order*> WareHouse:: getPendingOrders() const{ return(pendingOrders);}
const vector<Order*> WareHouse:: getInProcessOrders() const{ return(inProcessOrders);}
const vector<Order*> WareHouse:: getCompletedOrders() const{ return(completedOrders);}

void WareHouse::makeStep() {
    int i = 0;
    int j = pendingOrders.size();
    while(i < j) {
            bool erased = false;
            vector<Volunteer*>::iterator volIter = volunteers.begin();
            bool found = false;
            while (volIter != volunteers.end() && !found)
            {
                if ((*volIter)->canTakeOrder(*pendingOrders[i])){
                    (*volIter)->acceptOrder(*pendingOrders[i]);
                    if(pendingOrders[i]->getStatus() == (OrderStatus::PENDING)) {
                        pendingOrders[i]->setStatus(OrderStatus::COLLECTING);
                        pendingOrders[i]->setCollectorId((*volIter)->getId());
                    }
                    else if(pendingOrders[i]->getStatus() == OrderStatus::COLLECTING) {
                        (pendingOrders[i])->setStatus(OrderStatus::DELIVERING);
                        (pendingOrders[i])->setDriverId((*volIter)->getId());
                    }
                    inProcessOrders.push_back(pendingOrders[i]);
                    pendingOrders.erase(pendingOrders.begin() + i);
                    j--;
                    found = true;
                    erased = true;
                }
                volIter++;
            }
            if(!erased) {i++;}
    }
    int q = 0;
    int z = volunteers.size();
    while(q < z){
        bool wasErased = false;
        if(volunteers[q]->isBusy()){
            volunteers[q]->step();
            if(!volunteers[q]->isBusy()){
                int k = 0 ;
                while(((inProcessOrders[k])->getId()!=volunteers[q]->getCompletedOrderId())){
                        k++;
                }
                if( (inProcessOrders[k])->getStatus() == OrderStatus::COLLECTING) {    
                    pendingOrders.push_back(inProcessOrders[k]);
                    inProcessOrders.erase(inProcessOrders.begin() + k);
                    }
                else if((inProcessOrders[k])->getStatus() == OrderStatus::DELIVERING) {
                         (inProcessOrders[k])->setStatus(OrderStatus::COMPLETED);
                         completedOrders.push_back(inProcessOrders[k]);
                         inProcessOrders.erase(inProcessOrders.begin() + k);
                         }
                if(!volunteers[q]->hasOrdersLeft()){
                    Volunteer * temp = volunteers[q];
                    volunteers.erase(volunteers.begin() + q);
                    z--;
                    delete (temp);
                    (temp) = nullptr;
                    wasErased = true;
                }
            }
        }
        if(!wasErased) {q++;}
    }
}