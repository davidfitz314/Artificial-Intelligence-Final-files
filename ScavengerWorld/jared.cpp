#include "jared.h"
#include <cstdio>
#include <cstring>
#include <math.h>
#include "WorldAndAgent.h"

namespace df
{
	namespace Scavenger
	{
		/* For more on options look at the files:
		* ai-lib/include/Agent/Options.h
		* ai-lib/src/Agent/Options.cpp
		* al-lib/src/Agent/AgentDriver.cpp
		*/
		/*
		* Run with:
		* ./RunProg ./SA_Test -a s -U 1
		*/
		Jared::Jared(ai::Agent::Options *opts)
		{
		SetName("Jared");
		//std::cout << "The value of the -U option is: " << opts->GetArgInt("user1") << std::endl;
		this->looking = true;
		this->goHome = false;
		this->first_time = true;
		this->direction = 'z';
		this->finalCheck = false;
		this->examining = false;
		//this->otest = false;
		//this->objectCount = 0;
		this->preceptCheck = false;
		this->lastExamine = "empty";
		this->lastpicked = "empty";
		}

		Jared::~Jared()
		{
		}

		ai::Agent::Action * Jared::Program(const ai::Agent::Percept * percept)
		{
		std::stringstream ss;

		std::string cell_north, cell_south, cell_east, cell_west;
		int base;
		double CHARGE, HP;		

		ss.str(percept->GetAtom("BASE").GetValue()); ss.clear();
		ss >> base;
		mModel.setBase(base);

		double X_LOCATION;
		ss.str(percept->GetAtom("X_LOC").GetValue()); ss.clear();
		ss >> X_LOCATION;

		double Y_LOCATION;
		ss.str(percept->GetAtom("Y_LOC").GetValue()); ss.clear();
		ss >> Y_LOCATION;

		double Z_LOCATION;
		ss.str(percept->GetAtom("Z_LOC").GetValue()); ss.clear();
		ss >> Z_LOCATION;
		if (X_LOCATION < 0)
		  X_LOCATION -= .001;
		else if (X_LOCATION > 0)
		  X_LOCATION += .001;
		if (Y_LOCATION < 0)
		  Y_LOCATION -= .001;
		else if (Y_LOCATION > 0)		
		  Y_LOCATION += .001;
		cLocation locate;
		locate.x = X_LOCATION;
		locate.y = Y_LOCATION;
		
		this->mModel.setAgentLocation(locate);
		//initial cell creation, only used for the start cell
		if (this->first_time){
			cLocation current_cell_location;
			current_cell_location.x = X_LOCATION;
			current_cell_location.y = Y_LOCATION;
			
			cell mCell = df::Scavenger::cell();		
			this->mModel.addCell(current_cell_location, mCell);
			mCell = this->mModel.getCell(locate);
			this->mModel.updateMap(mCell);
			/*
			gLocation base_goal;
			base_goal.x = X_LOCATION;
			base_goal.y = Y_LOCATION;
			base_goal.z = Z_LOCATION;
			
			this->mModel.setGoalLocation(base_goal);*/
			this->first_time = false;

			df::Scavenger::object mObject = df::Scavenger::object();
		} 

		cell mCell = this->mModel.getCell(this->mModel.getAgentLocation());
		//mCell.display();
		mCell.setZ(Z_LOCATION);
		
		this->mModel.updateMap(mCell);

		ss.str(percept->GetAtom("CHARGE").GetValue()); ss.clear();
		ss >> CHARGE;
		this->mModel.setCharge(CHARGE);

		ss.str(percept->GetAtom("HP").GetValue()); ss.clear();
		ss >> HP;
		this->mModel.setHP(HP);
		
		//this->mModel.modelDisplay();
		ai::Scavenger::Action *action = new ai::Scavenger::Action;
////////////////////update the map's cells object map precepts///////////
		if (mCell.getLocalObjectMap().size() == 0){
		unsigned int i;
		for(i = 0; i < percept->NumAtom(); i++)
			{
				ai::Agent::PerceptAtom a = percept->GetAtom(i);
				if (a.GetName().find("OBJECT_") == false)
				{
					//std::cout << a.GetName() << ": " << a.GetValue() << std::endl;
					object tempObject = df::Scavenger::object(a.GetValue());
					mCell.addLocalObject(a.GetValue(), tempObject);	
					action->SetObjectId(a.GetValue());
					this->mModel.updateMap(mCell);
				} 
			}
		} 

////////////////////fifo queue action////////////////////////////////////
		if (!this->myFifo.empty()){
			df::Scavenger::Action* currentAct = this->myFifo.front();
		   	action->SetCode(currentAct->getAction());
			//std::cout << this->myFifo.front()->getAction() << " action" << std::endl;
			this->myFifo.pop();
		   	return action;
		}


////////////////////recharge to reduce time steps////////////////////////
		if (this->mModel.getCharge() < 90 && this->mModel.getAgentLocation().x == 0 && this->mModel.getAgentLocation().y == 0){					
			action->SetCode(ai::Scavenger::Action::RECHARGE);
			this->mModel.setExplore(true);
			this->mModel.setBaseGoal(false);
			this->examining = false;
			
			return action;
		}

/////////////////////deposit objects/////////////////////////////////////
		//std::cout << "hopper count is " << this->hopperMap.size() << std::endl;
		if (this->hopperMap.size() > 0 && this->mModel.getAgentLocation().x == 0 && this->mModel.getAgentLocation().y == 0){
			//std::cout << "get in my belly" << std::endl;
			for (std::map<std::string,df::Scavenger::object>::iterator it=this->hopperMap.begin(); it!=this->hopperMap.end(); ++it){
				//std::cout << it->first << std::endl;
				action->SetObjectId(it->first);
				action->SetCode(ai::Scavenger::Action::DEPOSIT);
				this->removelObject(it->second);				
				return action;
				
			}
		}

////////////////////looking section//////////////////////////////////////		
		if (this->looking){
			//std::cout << "a" << std::endl;
			if (this->direction == 'n'){
				ss.str(percept->GetAtom("LOOK").GetValue()); ss.clear();
				ss >> cell_north;
				mCell.setNorth(cell_north);
				this->mModel.updateMap(mCell);
				this->direction = 'z';

				//std::cout << "b" << std::endl;
				cLocation nloca = this->mModel.getAgentLocation();
				nloca.y += 1000;
				//if statement here to test if there isn't already a cell before overwriting.
//				if (cell_north != "wall"){					
					if (!this->mModel.isMapCell(nloca)){
						cell northern = df::Scavenger::cell(0, nloca.x, nloca.y, mCell.getz(), "empty", "empty", "empty", "empty");
						this->mModel.addCell(nloca, northern);
						if (mCell.getNorth() == "wall"){
						northern.setSouth("cliff");
						} else if (mCell.getNorth() == "cliff"){
							northern.setSouth("wall");
						} else {
							northern.setSouth(mCell.getNorth());
						}
						this->mModel.updateMap(northern);
					} else {
						cell northern = this->mModel.getCell(nloca);
						if (mCell.getNorth() == "wall"){
							northern.setSouth("cliff");
						} else if (mCell.getNorth() == "cliff"){
							northern.setSouth("wall");
						} else {
							northern.setSouth(mCell.getNorth());
						}
						this->mModel.updateMap(northern);
					}
//				}
				
			}	
			if (this->direction == 's'){
				ss.str(percept->GetAtom("LOOK").GetValue()); ss.clear();	
				ss >> cell_south;	
				mCell.setSouth(cell_south);
				this->mModel.updateMap(mCell);	
				this->direction = 'z';	

				//std::cout << "c" << std::endl;
				cLocation sloca = this->mModel.getAgentLocation();
				sloca.y -= 1000;
				//if statement here to test if there isn't already a cell before overwriting.
//				if (cell_south != "wall"){					
					if (!this->mModel.isMapCell(sloca)){
						cell southern = df::Scavenger::cell(0, sloca.x, sloca.y, mCell.getz(), "empty", "empty", "empty", "empty");
						this->mModel.addCell(sloca, southern);
						if (mCell.getSouth() == "wall"){
							southern.setNorth("cliff");
						} else if (mCell.getSouth() == "cliff"){
							southern.setNorth("wall");
						} else {
							southern.setNorth(mCell.getSouth());
						}
						this->mModel.updateMap(southern);
					} else {
						cell southern = this->mModel.getCell(sloca);
						if (mCell.getSouth() == "wall"){
							southern.setNorth("cliff");
						} else if (mCell.getSouth() == "cliff"){
							southern.setNorth("wall");
						} else {
							southern.setNorth(mCell.getSouth());
						}
						this->mModel.updateMap(southern);
					}				
//				}


			}	
			if (this->direction == 'e'){
				ss.str(percept->GetAtom("LOOK").GetValue()); ss.clear();
				ss >> cell_east;
				mCell.setEast(cell_east);
				this->mModel.updateMap(mCell);
				this->direction = 'z';

				//std::cout << "d" << std::endl;
				cLocation eloca = this->mModel.getAgentLocation();
				eloca.x += 1000;
				//if statement here to test if there isn't already a cell before overwriting.
//				if (cell_east != "wall"){					
					if (!this->mModel.isMapCell(eloca)){
						cell eastern = df::Scavenger::cell(0, eloca.x, eloca.y, mCell.getz(), "empty", "empty", "empty", "empty");
						this->mModel.addCell(eloca, eastern);
						if (mCell.getEast() == "wall"){
							eastern.setWest("cliff");
						} else if (mCell.getEast() == "cliff"){
							eastern.setWest("wall");
						} else {
							eastern.setWest(mCell.getEast());
						}
						this->mModel.updateMap(eastern);

					} else {
						cell eastern = this->mModel.getCell(eloca);
						if (mCell.getEast() == "wall"){
							eastern.setWest("cliff");
						} else if (mCell.getEast() == "cliff"){
							eastern.setWest("wall");
						} else {
							eastern.setWest(mCell.getEast());
						}
						this->mModel.updateMap(eastern);
					}
//				}

			}	
			if (this->direction == 'w'){
				ss.str(percept->GetAtom("LOOK").GetValue()); ss.clear();
				ss >> cell_west;
				mCell.setWest(cell_west);
				this->mModel.updateMap(mCell);
				this->direction = 'z';

				//std::cout << "e" << std::endl;
				cLocation wloca = this->mModel.getAgentLocation();
				wloca.x -= 1000;
				//if statement here to test if there isn't already a cell before overwriting.
//				if (cell_west != "wall"){
					if (!this->mModel.isMapCell(wloca)){
						cell western = df::Scavenger::cell(0, wloca.x, wloca.y, mCell.getz(), "empty", "empty", "empty", "empty");
						this->mModel.addCell(wloca, western);
						if (mCell.getWest() == "wall"){
							western.setEast("cliff");
						} else if (mCell.getWest() == "cliff"){
							western.setEast("wall");
						} else {
							western.setEast(mCell.getWest());
						}
						this->mModel.updateMap(western);

					} else {
						cell western = this->mModel.getCell(wloca);
						if (mCell.getWest() == "wall"){
							western.setEast("cliff");
						} else if (mCell.getWest() == "cliff"){
							western.setEast("wall");
						} else {
							western.setEast(mCell.getWest());
						}
						this->mModel.updateMap(western);

					}
//				}
			}
				
		
			if (mCell.getNorth() == "empty"){
				
				action->SetDirection(ai::Scavenger::Location::NORTH);	 
				action->SetCode(ai::Scavenger::Action::LOOK);
				this->direction = 'n';
				//std::cout << "f" << std::endl;
				return action;
			} 

			if (mCell.getSouth() == "empty"){
				action->SetDirection(ai::Scavenger::Location::SOUTH);	 
				action->SetCode(ai::Scavenger::Action::LOOK);
				this->direction = 's';	
				//std::cout << "g" << std::endl;			
				return action;
			} 			

			if (mCell.getEast() == "empty"){
				action->SetDirection(ai::Scavenger::Location::EAST);	 
				action->SetCode(ai::Scavenger::Action::LOOK);
				this->direction = 'e';		
				//std::cout << "h" << std::endl;		
				return action;
			} 
			
			if (mCell.getWest() == "empty"){
				
				action->SetDirection(ai::Scavenger::Location::WEST);	 
				action->SetCode(ai::Scavenger::Action::LOOK);
				this->direction = 'w';
				//std::cout << "i" << std::endl;
				return action;
			} 
			this->examining = true;
			this->preceptCheck = true;
			
		}
////////////////////////////Test charge////////////////////////////////////
/*
		df::Scavenger::secretAgentMan testModel;
		testModel = this->mModel;
		action = new ai::Scavenger::Action();
		
		
		testModel.setExplore(true);
		df::Scavenger::Search *mySearch = new df::Scavenger::Search(&testModel);
		mySearch->goSearch();
		
		double toUnvisitedCost = mySearch->getPathCost();
		slocation finalxy = mySearch->getLast();
			
		df::Scavenger::secretAgentMan copiedModel;
		copiedModel = this->mModel;
		//copy over the locations passed from search
		cLocation newxy;
		newxy.x = finalxy.x;
		newxy.y = finalxy.y;
		copiedModel.setAgentLocation(newxy);
		copiedModel.setExplore(false);
		copiedModel.setBaseGoal(true);
		df::Scavenger::Search *toBaseFrom = new df::Scavenger::Search(&copiedModel);		
		toBaseFrom->goSearch();
		
		//if this cost is to great then don't use it
		toUnvisitedCost += toBaseFrom->getPathCost() + 3.5;
		if (this->mModel.getCharge() < toUnvisitedCost){
			//create a search of the model that is going back home to base
			this->mModel.setExplore(false);
			this->mModel.setBaseGoal(true);
			df::Scavenger::Search *toHome = new df::Scavenger::Search(&mModel);
			toHome->goSearch();
			this->looking = false;
			this->examining = false;
			this->myFifo = toHome->getFifo();
			//Action section for what was in the queue
			df::Scavenger::Action* currentAct = this->myFifo.front();
		   	action->SetCode(currentAct->getAction());
			//std::cout << this->myFifo.front()->getAction() << " action" << std::endl;
			this->myFifo.pop();
			//std::cout << "1b" << std::endl;
			return action;
		}*/
////////////////////////////examine and pickup objects////////////////////////////////////////////////////////

		if (this->examining){
			this->looking = false;
			if (lastExamine != "empty"){
				//std::cout << lastExamine << " was examined" << std::endl;
				std::string oid, ocolor, oshape, osize;
				ss.str(percept->GetAtom("EXAMINE").GetValue()); ss.clear();
				ss >> oid; ss.ignore();
				ss >> ocolor; ss.ignore();
       			ss >> oshape; ss.ignore();
        		ss >> osize; ss.ignore();
				//std::cout << "color,shape,size " << ocolor << "," << oshape << "," << osize << std::endl;
				if (ocolor == "dayglo-orange" && oshape == "cylinder" && osize == "medium" && lastExamine != lastpicked){
					action->SetObjectId(lastExamine);
					action->SetCode(ai::Scavenger::Action::PICKUP);
					df::Scavenger::object temp_obj = df::Scavenger::object(lastExamine, osize, oshape, ocolor);
					temp_obj.setWanted(true);
					this->lastpicked = lastExamine;
					//add object to bin map
					this->addlObject(temp_obj);
					//delete object from cell map					
					this->mModel.deleteObjectFromMap(mCell, temp_obj);
					//this->mModel.updateObjectMap(mCell, temp_obj);
					this->lastExamine = "empty";
					return action;
				} 
				if (ocolor == "orchid" && oshape == "quadruped" && osize == "tiny" && lastExamine != lastpicked){
					action->SetObjectId(lastExamine);
					action->SetCode(ai::Scavenger::Action::PICKUP);
					df::Scavenger::object temp_obj = df::Scavenger::object(lastExamine, osize, oshape, ocolor);
					temp_obj.setWanted(true);
					this->lastpicked = lastExamine;
					//add object to bin map
					this->addlObject(temp_obj);
					//after picked up delete it from cell map
					this->mModel.deleteObjectFromMap(mCell, temp_obj);
					//this->mModel.updateObjectMap(mCell, temp_obj);
					this->lastExamine = "empty";
					return action;
				} 
				if (lastExamine == lastpicked){
					examining = false;
					std::cout << "go home you're full " << std::endl;
					this->mModel.updateMap(mCell);
					this->mModel.setExplore(false);
					this->mModel.setBaseGoal(true);
					df::Scavenger::Search *toHome = new df::Scavenger::Search(&mModel);
					toHome->goSearch();
					this->looking = false;
					this->myFifo = toHome->getFifo();
					//Action section for what was in the queue
					df::Scavenger::Action* currentAct = this->myFifo.front();
				   	action->SetCode(currentAct->getAction());
					//std::cout << this->myFifo.front()->getAction() << " action" << std::endl;
					this->myFifo.pop();
					//std::cout << "1b" << std::endl;
					return action;
					
				}
				df::Scavenger::object temp_obj = df::Scavenger::object(lastExamine, osize, oshape, ocolor);
				temp_obj.setWanted(false);
				this->mModel.updateObjectMap(mCell, temp_obj);
				this->lastExamine = "empty";
///////////we are not completely updating our map of objects fix that before trying to pick up specific objects
			}
			
			//std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
			std::map<std::string, object> temp_map = this->mModel.getCell(this->mModel.getAgentLocation()).getLocalObjectMap();
			std::map<std::string, object>::iterator it;
			for (it = temp_map.begin(); it != temp_map.end(); it++)
		    {
				if (it->second.getWanted()){
					action->SetObjectId(it->first);
					action->SetCode(ai::Scavenger::Action::EXAMINE);
					this->lastExamine = it->first;
			        return action;
				}
		    }
			//std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;	
			
			examining = false;
			
		}
		
		action = new ai::Scavenger::Action();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		

//setting current cells visited to true since it has passed the looking and examining/picking up section
		mCell = this->mModel.getCell(this->mModel.getAgentLocation());
		mCell.setVisited(true);

		this->mModel.updateMap(mCell);
		mCell = this->mModel.getCell(this->mModel.getAgentLocation());
/*
		//prints out the map of cells
		std::cout << "cell count is: " << this->mModel.getMapSize() << std::endl;
		std::map<cLocation, cell> temp_map = this->mModel.getMap();
		std::map<cLocation, cell>::iterator it;
		for (it = temp_map.begin(); it != temp_map.end(); it++)
        {
            it->second.display();
        }

		//charge test make sure it doesn't get too low and cannot quit on its own.
		if (this->mModel.getCharge() < 1.5){
			ai::Scavenger::Action *action = new ai::Scavenger::Action();
	       	action->SetCode(ai::Scavenger::Action::QUIT);
			return action;
		}
		
*/		
			

	

//std::cout << "hopper count is " << this->hopperMap.size() << std::endl;
/*		if (this->hopperMap.size() > 0 && this->mModel.getAgentLocation().x == 0 && this->mModel.getAgentLocation().y == 0){
			//std::cout << "get in my belly" << std::endl;
			for (std::map<std::string,df::Scavenger::object>::iterator it=this->hopperMap.begin(); it!=this->hopperMap.end(); ++it){
				//std::cout << it->first << std::endl;
				action->SetObjectId(it->first);
				action->SetCode(ai::Scavenger::Action::DEPOSIT);
				this->removelObject(it->second);				
				return action;
				
			}
		}
*/
//////////////////////////////////Search Section/////////////////////////////////////////////////
		//testing if queue is empty before starting more searches
		if (this->myFifo.empty()){
			//set up a bool in search to decide if there has been an unvisited cell found as of right now we are just assuming there is
			this->examining = false;

			this->mModel.setExplore(true);
			df::Scavenger::Search *mySearch = new df::Scavenger::Search(&mModel);
			mySearch->goSearch();
			bool foundUnVisited = mySearch->isUnvisited();
			double toUnvisitedCost = mySearch->getPathCost();

			slocation finalxy = mySearch->getLast();
				
			df::Scavenger::secretAgentMan copiedModel;
			copiedModel = this->mModel;
			//copy over the locations passed from search
			cLocation newxy;
			newxy.x = finalxy.x;
			newxy.y = finalxy.y;
			copiedModel.setAgentLocation(newxy);
			copiedModel.setExplore(false);
			copiedModel.setBaseGoal(true);
			df::Scavenger::Search *toBaseFrom = new df::Scavenger::Search(&copiedModel);		
			toBaseFrom->goSearch();
			
			//if this cost is to great then don't use it
			toUnvisitedCost += toBaseFrom->getPathCost() + 5;

			

			//if (foundUnVisited){				
			
			//std::cout << "found unvisited is " << foundUnVisited << std::endl;
			if (foundUnVisited){
				if (this->mModel.getCharge() > toUnvisitedCost){
					this->looking = true;
					this->examining = false;
					this->myFifo = mySearch->getFifo();	
					df::Scavenger::Action* currentAct = this->myFifo.front();
				   	action->SetCode(currentAct->getAction());	
					this->myFifo.pop();
					return action;
				}

				if (this->mModel.getAgentLocation().x == 0 && this->mModel.getAgentLocation().y == 0){
					if (this->mModel.getCharge() == 100){
						action->SetCode(ai::Scavenger::Action::QUIT);
						return action;
					}
					action->SetCode(ai::Scavenger::Action::RECHARGE);
					this->mModel.setExplore(true);
					this->mModel.setBaseGoal(false);
					this->examining = false;
					return action;
				} else {
					//create a search of the model that is going back home to base
					this->mModel.setExplore(false);
					this->mModel.setBaseGoal(true);
					this->examining = false;
					df::Scavenger::Search *toHome = new df::Scavenger::Search(&mModel);
					toHome->goSearch();
					this->looking = false;
					this->myFifo = toHome->getFifo();
					//Action section for what was in the queue
					df::Scavenger::Action* currentAct = this->myFifo.front();
				   	action->SetCode(currentAct->getAction());
					//std::cout << this->myFifo.front()->getAction() << " action" << std::endl;
					this->myFifo.pop();
					//std::cout << "1b" << std::endl;
					return action;
				}
			} 

			if (!foundUnVisited) {
				if (this->mModel.getAgentLocation().x == 0 && this->mModel.getAgentLocation().y == 0){
					if (this->mModel.getCharge() < 90){					
						action->SetCode(ai::Scavenger::Action::RECHARGE);
						this->mModel.setExplore(true);
						this->mModel.setBaseGoal(false);
						this->examining = false;
						//std::cout << "am i getting to here?" << std::endl;
						return action;
					} else {
						action->SetCode(ai::Scavenger::Action::QUIT);
						//std::cout << "or am i getting to here?" << std::endl;
						return action;
					}	
				} else {
					//create a search of the model that is going back home to base
					this->mModel.setExplore(false);
					this->mModel.setBaseGoal(true);
					df::Scavenger::Search *toHome = new df::Scavenger::Search(&mModel);
					toHome->goSearch();
					this->looking = false;
					this->examining = false;
					this->myFifo = toHome->getFifo();
					//Action section for what was in the queue
					df::Scavenger::Action* currentAct = this->myFifo.front();
				   	action->SetCode(currentAct->getAction());
					//std::cout << this->myFifo.front()->getAction() << " action" << std::endl;
					this->myFifo.pop();
					//std::cout << "1c" << std::endl;
					return action;
				}
			}
				
			
			//std::cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
			//std::cout << toUnvisitedCost << std::endl;
			//std::cout << "yyyyyyyyyyyyyyyyyyyyyyyy" << std::endl;

									
		} 

		if (!this->myFifo.empty()){
			//Action section for what was in the queue
			df::Scavenger::Action* currentAct = this->myFifo.front();
	       	action->SetCode(currentAct->getAction());
			//std::cout << this->myFifo.front()->getAction() << " action" << std::endl;
			this->myFifo.pop();
			//std::cout << "10" << std::endl;
			return action;
		}
		action->SetCode(ai::Scavenger::Action::QUIT);
		//std::cout << "Agent got confused, sending a quit" << std::endl;
		//std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
		//mCell.display();
		//std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
       	return action;
		}	

		void Jared::addlObject(df::Scavenger::object object_in)
		{
			this->hopperMap[object_in.getID()] = object_in;
		}

		void Jared::updatelObject(df::Scavenger::object object_in)
		{
			std::string id_key = object_in.getID();

			std::map<std::string, object> temp_map = this->hopperMap;
			std::map<std::string, object>::iterator it = temp_map.find(id_key);
			if(it != temp_map.end())
			{
		    	//element found;
				if (object_in.getSize() != "empty" && hopperMap[id_key].getSize() == "empty"){
					this->hopperMap[id_key].setSize(object_in.getSize());
				} 
				if (object_in.getShape() != "empty" && hopperMap[id_key].getShape() == "empty"){
					this->hopperMap[id_key].setShape(object_in.getShape());
				} 
				if (object_in.getColor() != "empty" && hopperMap[id_key].getColor() == "empty"){
					this->hopperMap[id_key].setColor(object_in.getColor());
				} 
				this->hopperMap[id_key].setWanted(object_in.getWanted());
			}
		}

		void Jared::removelObject(df::Scavenger::object object_in)
		{
			std::string id_key = object_in.getID();
			std::map<std::string,df::Scavenger::object>::iterator it;
			it=this->hopperMap.find(id_key);
			this->hopperMap.erase(it);
			//std::cout << "deleted succesfully" << std::endl;
		}
		
	}
}
