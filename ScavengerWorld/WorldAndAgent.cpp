#include "WorldAndAgent.h"
#include <iostream>

namespace df
{
  	namespace Scavenger
  	{
		//////////////////////////////////objects//////////////////////////
		object::object()
		{
			this->setID("empty");
			this->setSize("empty");
			this->setShape("empty");
			this->setColor("empty");
			this->setWanted(true);
		}

		object::object(std::string id_in)
		{
			this->setID(id_in);
			this->setSize("empty");
			this->setShape("empty");
			this->setColor("empty");
			this->setWanted(true);
		}

		object::object(std::string id_in, std::string size_in, std::string shape_in, std::string color_in)
		{
			this->setID(id_in);
			this->setSize(size_in);
			this->setShape(shape_in);
			this->setColor(color_in);
			this->setWanted(true);
		}

		object::~object()
		{
			//object deconstructor
		}

		std::string object::getID()
		{
			return this->oID;
		}
	
		std::string object::getSize()
		{
			return this->oSize;	
		}

		std::string object::getShape()
		{
			return this->oShape;
		}

		std::string object::getColor()
		{
			return this->oColor;
		}
	
		void object::setID(std::string id_in)
		{
			this->oID = id_in;
		}
	
		void object::setSize(std::string size_in)
		{
			this->oSize = size_in;
		}

		void object::setShape(std::string shape_in)
		{
			this->oShape = shape_in;
		}

		void object::setColor(std::string color_in)
		{
			this->oColor = color_in;
		}

		bool object::getWanted()
		{
			return this->wanted;
		}

		void object::setWanted(bool yesistrue)
		{
			this->wanted = yesistrue;
		}

		void object::display()
		{
			std::cout << "+-----------------+" << std::endl;
			std::cout << "Id is " << this->getID() << std::endl;
			std::cout << "Shape is " << this->getShape() << std::endl;
			std::cout << "Size is " << this->getSize() << std::endl;
			std::cout << "Color is " << this->getColor() << std::endl;
			std::cout << "+-----------------+" << std::endl;
		}		

		//////////////////////////////////cell/////////////////////////////
		cell::cell()
		{
			//initializing a first with all 0's
			this->setId(0);
			this->setX(0);
			this->setY(0);
			this->setZ(0.0);
			this->setNorth("empty");
			this->setEast("empty");
			this->setSouth("empty");
			this->setWest("empty");
			this->setVisited(false);
			
			
			//setZ(0);
		}
		
		//constructor with each cells id, location, and what is n,s,e,w of it.
		cell::cell(int Id, int x, int y, double z, std::string n, std::string s, std::string e, std::string w)
		{
			setId(Id);
			setX(x);
			setY(y);
			setZ(z);
			setNorth(n);
			setSouth(s);
			setEast(e);
			setWest(w);
			setVisited(false);
		}

		cell::~cell()
		{
			//deconstructor
		}

		int cell::getId()
		{
			return cId;
		}
	
		int cell::getX()
		{
			return cX;
		}

		int cell::getY()
		{
			return cY;
		}
	
		double cell::getz()
		{
			return cZ;
		}

		std::string cell::getNorth()
		{
			return cNorth;
		}

		std::string cell::getSouth()
		{
			return cSouth;
		}

		std::string cell::getEast()
		{
			return cEast;
		}

		std::string cell::getWest()
		{
			return cWest;
		}
	
		void cell::setId(int id)
		{
			this->cId = id;
		}
	
		void cell::setX(int x)
		{
			this->cX = x;
		}

		void cell::setY(int y) 
		{
			this->cY = y;
		}
	
		void cell::setZ(double z)
		{
			this->cZ = z;
		}
	
		void cell::setNorth(std::string north)
		{
			this->cNorth = north;
		}

		void cell::setSouth(std::string south)
		{
			this->cSouth = south;
		}
	
		void cell::setEast(std::string east)
		{
			this->cEast = east;
		}
		void cell::setWest(std::string west)
		{
			this->cWest = west;
		}

		bool cell::getVisited()
		{
			return this->visited;
		}
	
		void cell::setVisited(bool in_visit)
		{
			this->visited = in_visit;
		}

		//this grabs the object location out of the map
		object cell::getLocalObject(std::string id_in)
		{
			return this->localObjects[id_in];
		}

		//this adds an object to the map			
		void cell::addLocalObject(std::string id_in, object newObject)
		{
			this->localObjects[id_in] = newObject;
		}

		std::map<std::string, object > cell::getLocalObjectMap()
		{
			return this->localObjects;
		}

		bool cell::isLocalObjectMapCell(std::string key_in)
		{
			std::map<std::string, object> temp_map = this->localObjects;
			std::map<std::string, object>::iterator it = temp_map.find(key_in);

			if(it != temp_map.end())
			{
		    	//element found;
				return true;
			} 
			return false;
		}

		int cell::getLocalObjectMapSize()
		{
			return this->localObjects.size();
		}

		void cell::updateLocalObjectMap(df::Scavenger::object object_in)
		{
			std::string id_key = object_in.getID();

			std::map<std::string, object> temp_map = this->localObjects;
			std::map<std::string, object>::iterator it = temp_map.find(id_key);
			if(it != temp_map.end())
			{
		    	//element found;
				if (object_in.getSize() != "empty" && localObjects[id_key].getSize() == "empty"){
					this->localObjects[id_key].setSize(object_in.getSize());
				} 
				if (object_in.getShape() != "empty" && localObjects[id_key].getShape() == "empty"){
					this->localObjects[id_key].setShape(object_in.getShape());
				} 
				if (object_in.getColor() != "empty" && localObjects[id_key].getColor() == "empty"){
					this->localObjects[id_key].setColor(object_in.getColor());
				} 
				this->localObjects[id_key].setWanted(object_in.getWanted());
			}
		}

		bool cell::removeLocalObject(object object_in)
		{
			std::string key = object_in.getID();
			std::map<std::string, object> temp_map = this->localObjects;
			std::map<std::string, object>::iterator it = temp_map.find(key);
			if(it != temp_map.end())
			{
				temp_map.erase(it);
				this->localObjects = temp_map;
				return true;
			}
			return false;
		}

		bool cell::objectExamined(object object_in)
		{
			std::string key = object_in.getID();
			std::map<std::string, object> temp_map = this->localObjects;
			std::map<std::string, object>::iterator it = temp_map.find(key);
			if(it != temp_map.end())
			{
				if (this->localObjects[key].getSize() != "empty" && this->localObjects[key].getShape() != "empty" && this->localObjects[key].getColor() != "empty"){
					return true;
				} 
				return false;
			}
			return false;
		}
		
		void cell::setObjectMap(std::map<std::string, object> map_in)
		{
			this->localObjects = map_in;
		}

		void cell::display()
		{
			std::cout << "+-----------+" << std::endl;
			std::cout << "x,y,z " << this->getX() <<"," << this->getY() << "," << this->getz() << std::endl;
			std::cout << "north,south,east,west " << this->getNorth() <<"," << this->getSouth() << "," << this->getEast() << "," << this->getWest() << std::endl;
			std::cout << "visited = " << this->getVisited() << std::endl;
			std::cout << "+-----------+" << std::endl;
		}

		

/////////////////////////////////////////////world model/////////////////////////////////////////////////
		secretAgentMan::secretAgentMan()
		{
			cLocation myInit;
			myInit.x = 0;
			myInit.y = 0;
			//myInit.z = 0;
			gLocation gInit;
			gInit.x = 0;
			gInit.y = 0;
			gInit.z = 0.0;
			setAgentLocation(myInit);
			setGoalLocation(gInit);
			setCharge(0);
			setBase(0);
			setHP(0);	
			this->setExplore(false);
			this->setBaseGoal(false);
		}
		
		//agents immediate information accessed here
		secretAgentMan::secretAgentMan(cLocation agentLocation, gLocation goalLocation, double charge, double base, double hitpoints)
		{
			setAgentLocation(agentLocation);
			setGoalLocation(goalLocation);
			setCharge(charge);
			setBase(base);
			setHP(hitpoints);
			this->setExplore(false);
			this->setBaseGoal(false);
		}

		secretAgentMan::~secretAgentMan()
		{
			//empty deconstructor for agents imediate info		
		}

		cLocation secretAgentMan::getAgentLocation()
		{
			return mLocation;
		}
	
		gLocation secretAgentMan::getGoalLocation()
		{
			return mGoal;
		}

		double secretAgentMan::getCharge()
		{
			return mCharge;
		}

		double secretAgentMan::getBase()
		{
			return mBase;
		}

		double secretAgentMan::getHP()
		{
			return mHP;
		}

		void secretAgentMan::setAgentLocation(cLocation aLocation)
		{
			mLocation = aLocation;
		}

		void secretAgentMan::setGoalLocation(gLocation gLocation_in)
		{
			mGoal = gLocation_in;
		}

		void secretAgentMan::setCharge(double newCharge)
		{
			mCharge = newCharge;
		}

		void secretAgentMan::setBase(double newBase)
		{
			mBase = newBase;
		}

		void secretAgentMan::setHP(double newHP)
		{
			mHP = newHP;
		}

		//this grabs the cell location out of the map
		cell secretAgentMan::getCell(cLocation key)
		{
			return mCells[key];
		}

		//this adds a cell to the map			
		void secretAgentMan::addCell(cLocation locate, cell newCell)
		{
			mCells[locate] = newCell;
		}

		int secretAgentMan::getMapSize()
		{
			return this->mCells.size();
		}

		bool secretAgentMan::getExplore()
		{
			return this->mExplore;
		}
		void secretAgentMan::setExplore(bool explore_in)
		{
			this->mExplore = explore_in;
		}

		std::map<cLocation, cell > secretAgentMan::getMap()
		{
			return this->mCells;
		}
		
		bool secretAgentMan::isMapCell(cLocation key_in)
		{
			std::map<cLocation, cell> temp_map = this->mCells;
			std::map<cLocation, cell>::iterator it = temp_map.find(key_in);

			if(it != temp_map.end())
			{
		    	//element found;
				return true;
			} 
			return false;			
		}

		void secretAgentMan::modelDisplay(){
			std::cout << "+------------+" << std::endl;
			std::cout << "Charge is: " << this->getCharge() << std::endl;
			std::cout << "Base is: " << this->getBase() << std::endl;
			std::cout << "HP is: " << this->getHP() << std::endl;
			std::cout << "Explore is: " << this->getExplore() << std::endl;
			cLocation agentTest = this->getAgentLocation();
			std::cout << "agent x,y: " << agentTest.x << "," << agentTest.y << std::endl;	
			std::cout << "+------------+" << std::endl;
		}

		bool secretAgentMan::getBaseGoal(){
			return this->base_goal;
		}

		void secretAgentMan::setBaseGoal(bool bg_in){
			this->base_goal = bg_in;
		}

		void secretAgentMan::updateObjectMap(df::Scavenger::cell cell_in, df::Scavenger::object object_in)
		{
			cLocation getkey;
			getkey.x = cell_in.getX();
			getkey.y = cell_in.getY();

			std::map<cLocation, cell> temp_map = this->mCells;
			std::map<cLocation, cell>::iterator it = temp_map.find(getkey);
			if(it != temp_map.end())
			{
				this->mCells[getkey].updateLocalObjectMap(object_in);
			}
			else 
			{
				std::cout << "cell not found " << std::endl;
			}
		}

		void secretAgentMan::deleteObjectFromMap(df::Scavenger::cell cell_in, df::Scavenger::object object_in)
		{
			cLocation getkey;
			getkey.x = cell_in.getX();
			getkey.y = cell_in.getY();

			std::map<cLocation, cell> temp_map = this->mCells;
			std::map<cLocation, cell>::iterator it = temp_map.find(getkey);
			if(it != temp_map.end())
			{
				bool deleteO = this->mCells[getkey].removeLocalObject(object_in);
				if (!deleteO){
					std::cout << "object not removed" << std::endl;
				}
			}
			else 
			{
				std::cout << "cell not found " << std::endl;
			}
		}

		void secretAgentMan::updateMap(df::Scavenger::cell cell_in)
		{
			cLocation getkey;
			getkey.x = cell_in.getX();
			getkey.y = cell_in.getY();

			std::map<cLocation, cell> temp_map = this->mCells;
			std::map<cLocation, cell>::iterator it = temp_map.find(getkey);
			if(it != temp_map.end())
			{
		    	//element found;
				
			//cell temp_cell = this->mCells[getkey];
			//if (this->mCells[getkey].getX() == getkey.x && this->mCells[getkey].getY() == getkey.y){
				if (this->mCells[getkey].getVisited()){				
					if (cell_in.getz() != this->mCells[getkey].getz())
					{
						this->mCells[getkey].setZ(cell_in.getz());
					}
				}
				if (cell_in.getNorth() != "empty" && this->mCells[getkey].getNorth() == "empty"){
					//std::cout << this->mCells[getkey].getNorth() << " -> " << cell_in.getNorth() << std::endl; 
					this->mCells[getkey].setNorth(cell_in.getNorth());
				} 
				if (cell_in.getSouth() != "empty" && this->mCells[getkey].getSouth() == "empty"){
					//std::cout << this->mCells[getkey].getSouth() << " -> " << cell_in.getSouth() << std::endl;
					this->mCells[getkey].setSouth(cell_in.getSouth());
				}
				if (cell_in.getEast() != "empty" && this->mCells[getkey].getEast() == "empty"){
					//std::cout << this->mCells[getkey].getEast() << " -> " << cell_in.getEast() << std::endl;
					this->mCells[getkey].setEast(cell_in.getEast());
				}
				if (cell_in.getWest() != "empty" && this->mCells[getkey].getWest() == "empty"){
					//std::cout << this->mCells[getkey].getWest() << " -> " << cell_in.getWest() << std::endl;
					this->mCells[getkey].setWest(cell_in.getWest());
				}
				if (cell_in.getVisited() == true && this->mCells[getkey].getVisited() == false){
					//std::cout << this->mCells[getkey].getVisited() << " -> " << cell_in.getVisited() << std::endl;
					this->mCells[getkey].setVisited(cell_in.getVisited());
				}
				this->mCells[getkey].setObjectMap(cell_in.getLocalObjectMap());
			}
		}


		bool operator<(cLocation const& one, cLocation const& two)
        {	
			/*
        	if (one.x != two.x) return one.x < two.x;
        	if (one.y != two.y)
			return one.y < two.y;
        	return one.z < two.z; 
			*/
			if (one.x < two.x)
				return true;
			if (one.x > two.x)
				return false;
			if (one.y < two.y)
				return true;
			if (one.y > two.y)
				return false;	
			return false;
		}

	}
}
