#include <ros/ros.h>
#include <rosbasic/Calculation.h>

using namespace std;

int main(int argc,char** argv){
	ros::init(argc,argv,"client_node");
	ros::NodeHandle nh;
	ros::service::waitForService("/calculation_service");
	ros::ServiceClient client = nh.serviceClient<rosbasic::Calculation>("/calculation_service" );
	ros::Rate rate(10);

	while(ros::ok()){
		rosbasic::Calculation srv;
		cout << "Enter firsr number " << endl;
		cin >> srv.request.first_number;
		cout << "Enter second number " << endl;
		cin >> srv.request.second_number;
		client.call(srv);
		rate.sleep();
	}
}