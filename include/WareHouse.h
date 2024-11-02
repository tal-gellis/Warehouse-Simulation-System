#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"
class BaseAction;

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        WareHouse(const WareHouse& other);  // Copy constructor
        WareHouse& operator=(const WareHouse& other);  // Copy assignment operator
        WareHouse(WareHouse&& other);  // Move constructor
        WareHouse& operator=(WareHouse&& other);  // Move assignment operator
        ~WareHouse();  // Destructor
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        //--- Added by us
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        int getOrderCount() const ;
        int getCustomerCount() const ;
        int getVolunteerCount() const ;

        const vector<Order*> getPendingOrders() const;
        const vector<Order*> getInProcessOrders() const;
        const vector<Order*> getCompletedOrders() const;

        void makeStep();       



    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
    
        //--- Added by us  
        int orderCounter; 
        SoldierCustomer* NoCustomer = new SoldierCustomer(-1,"NoCustomer",0,0);
        CollectorVolunteer* NoVolunteer = new CollectorVolunteer(-1,"NoVolunteer",0);
        Order* NoOrder = new Order(-1,-1,0);
    


};
