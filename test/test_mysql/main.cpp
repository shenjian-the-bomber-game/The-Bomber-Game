#include <string>
#include <iostream>
#include "../../include/DatabaseConnection.hpp"
using namespace std;

DatabaseConnection *DatabaseConnection::obj = NULL;

int main()
{
        //cout << "My ptr:" << DatabaseConnection::get_instance() << endl;        
        DatabaseConnection::get_instance()->DatabaseInit();

        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "Hello!");
        DatabaseConnection::get_instance()->push_message("Cyanic", "yanic", "Yes!");
        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "GOOD!");
        DatabaseConnection::get_instance()->push_message("Dyanic", "Cyanic", "GOOD!");
        
	vector<string> test;
	
	int k;
	k = DatabaseConnection::get_instance()->retrive_history_count("Cyanic");
        cout << k << endl;
        test = DatabaseConnection::get_instance()->retrive_message("Cyanic");
        int i;
        for(i = 0; i < 12; i++) {
               cout << test[i] <<endl;
      	 }

        return 0;
}
