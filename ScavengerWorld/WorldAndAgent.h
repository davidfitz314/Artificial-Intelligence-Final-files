#ifndef _WORLDANDAGENT_H_
#define _WORLDANDAGENT_H_
#include <map>
#include <cstring>
#include <string>

namespace df 
{
	namespace Scavenger
  	{
		struct cLocation
		{
			int x,y;//,z;
		};
		bool operator<(cLocation const& one, cLocation const& two);

		struct gLocation
		{
			int x,y;
			double z;
		};

		class object
		{
		public:
			object();
			object(std::string id_in);
			object(std::string id_in, std::string size_in, std::string shape_in, std::string color_in);
			~object();
			std::string getID();
			std::string getSize();
			std::string getShape();
			std::string getColor();
			
			void setID(std::string id_in);
			void setSize(std::string size_in);
			void setShape(std::string shape_in);
			void setColor(std::string color_in);
			bool getWanted();
			void setWanted(bool yesistrue);
			void display();
		private:
			std::string oID;
			std::string oSize, oShape, oColor;
			bool wanted;
		};

		class cell
		{
		public:
			cell();
			cell(int Id, int x, int y, double z, std::string n, std::string s, std::string e, std::string w);
			~cell();
			int getId();
			int getX();
			int getY();
			double getz();
			std::string getNorth();
			std::string getSouth();
			std::string getEast();
			std::string getWest();
			void setId(int id);		
			void setX(int x);
			void setY(int x);
			void setZ(double x);
			void setNorth(std::string north);
			void setSouth(std::string south);
			void setEast(std::string east);
			void setWest(std::string west);
			bool getVisited();
			void setVisited(bool in_visit);
			void display();

			//this grabs the object location out of the map
			object getLocalObject(std::string id_in);
			//this adds an object to the map			
			void addLocalObject(std::string id_in, object newObject);
			std::map<std::string, object > getLocalObjectMap();
			bool isLocalObjectMapCell(std::string key_in);
			int getLocalObjectMapSize();
			void updateLocalObjectMap(df::Scavenger::object object_in);
			bool removeLocalObject(object object_in);
			bool objectExamined(object object_in);
			void setObjectMap(std::map<std::string, object> map_in);
		
		protected:
			std::map<std::string, object> localObjects;
		private:
			int cId; 
			int cX, cY;
			double cZ;
			std::string cNorth, cSouth, cEast, cWest;
			bool visited;
			
			
		};

		class secretAgentMan
		{
		public:
			secretAgentMan();
			secretAgentMan(cLocation agentLocation, gLocation goalLocation, double charge, double base, double hitpoints);
			~secretAgentMan();
			cLocation getAgentLocation();
			gLocation getGoalLocation();
			double getCharge();
			double getBase();
			double getHP();
			void setAgentLocation(cLocation aLocation);
			void setGoalLocation(gLocation gLocation_in);
			void setCharge(double newCharge);
			void setBase(double newBase);
			void setHP(double newHP);
			bool getExplore();
			void setExplore(bool explore_in);
			void modelDisplay();
			//this grabs the cell location out of the map
			cell getCell(cLocation key);
			//this adds a cell to the map			
			void addCell(cLocation locate, cell newCell);
			std::map<cLocation, cell > getMap();
			bool isMapCell(cLocation key_in);
			int getMapSize();
			void updateMap(df::Scavenger::cell cell_in);
			
			bool getBaseGoal();
			void setBaseGoal(bool bg_in);

			void updateObjectMap(df::Scavenger::cell cell_in, df::Scavenger::object object_in);
			void deleteObjectFromMap(df::Scavenger::cell cell_in, df::Scavenger::object object_in);			
			//std::map<cLocation, cell> getMyMap();
		protected:
			cLocation mLocation;
			gLocation mGoal;
			double mCharge;
			double mBase;
			double mHP;
			bool mExplore;
			bool base_goal;		
			std::map</*(x,y,z)*/cLocation, cell > mCells;//information about each cell in a convienient map	
			
		private:
		};
	}
}
#endif 
