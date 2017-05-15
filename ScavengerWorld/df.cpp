#include "df.h"
#include "WorldAndAgent.h"
#include "df_State.h"

#include <cstdio>
#include <cstring>
#include <math.h>
#include "df_Problem.h"
#include "df_Search.h"



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
    Dagent::Dagent(ai::Agent::Options *opts)
    {
      SetName("Dagent");
      std::cout << "The value of the -U option is: " << opts->GetArgInt("user1") << std::endl;
	  this->finished = false;
	  this->first = true;
    }

    Dagent::~Dagent()
    {
    }

    ai::Agent::Action * Dagent::Program(const ai::Agent::Percept * percept)
    {
	  std::stringstream  ss;
      double 			 cell_x, cell_y, cell_z;
      std::string        cell_north, cell_south, cell_east, cell_west;
	  int base;

      ss.str(percept->GetAtom("BASE").GetValue()); ss.clear();
      ss >> base;
	  mModel.setBase(base);
	  //std::cout << mModel.getBase() << std::endl;

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
	  else
		  X_LOCATION += .001;
	  if (Y_LOCATION < 0)
		  Y_LOCATION -= .001;
	  else		
		  Y_LOCATION += .001;
	  
	  cLocation locate;
	  locate.x = X_LOCATION;
	  locate.y = Y_LOCATION;
	  //locate.z = Z_LOCATION;
	  //agent location
	  //std::cout << "agent location " << locate.x << " x " << locate.y << " y " << std::endl;
	  mModel.setAgentLocation(locate);
	  
	  std::string mNorth, mSouth, mEast, mWest;

	  unsigned int i;
	  for(i = 0; i < percept->NumAtom(); i++)
	  {
		if(percept->GetAtom(i).GetName().substr(0, 5) == "CELL_")
		{
			//std::cout << percept << std::endl;
			// retrieve the ID
            int ID;
            std::string cellID = percept->GetAtom(i).GetName().substr(6);
            std::stringstream newss;
            newss.str(cellID); newss.clear();
            newss >> ID;
            
            std::string value = percept->GetAtom(i).GetValue();
            ss.str(value); ss.clear();
            // retrieve x, y, z,
            ss >> cell_x; ss.ignore();
            ss >> cell_y; ss.ignore();
            ss >> cell_z; ss.ignore();
			if (cell_x < 0)
				cell_x -= 0;
			else
				cell_x += .001;
			if (cell_y < 0)
				cell_y -= .001;
			else
				cell_y += .001;
			           
            std::getline(ss, cell_north, ',');
            std::getline(ss, cell_south, ',');
            std::getline(ss, cell_east,  ',');
            std::getline(ss, cell_west,  ',');
            
			cLocation temp;//(cell_x, cell_y, cell_z);
            temp.x = cell_x;
            temp.y = cell_y;
            //temp.z = cell_z;
              
            // add cell to mCells
               
			mModel.addCell(temp, cell(ID, cell_x, cell_y, cell_z, cell_north, cell_south, cell_east, cell_west));
			df::Scavenger::cell setVisit = mModel.getCell(temp);
			setVisit.setVisited(true);
            // create temporary location key for map
            		
		}
	  }
	  	
		double currCharge, currHP;
		double gx, gy, gz;
		ss.str(percept->GetAtom("CHARGE").GetValue()); ss.clear();
		ss >> currCharge; ss.clear();
		mModel.setCharge(currCharge);
		
		gLocation goalIn;				
		std::string goaly = percept->GetAtom("GOAL_LOC").GetValue();
		ss.str(goaly); ss.clear();
		ss >> gx; ss.ignore();
        ss >> gy; ss.ignore();
        ss >> gz; ss.ignore();
		if (gx < 0)
			gx -= .001;
		else
			gx += .001;
		if (gy < 0)
			gy -= .001;
		else
			gy += .001;		
		goalIn.x = gx;
		goalIn.y = gy;
		goalIn.z = gz;
		
		mModel.setGoalLocation(goalIn);
		//goal test
		//gLocation gg = mModel.getGoalLocation();
		//std::cout << gg.x << " x " << gg.y << " y " << gg.z << " z " << std::endl;
		ss.str(percept->GetAtom("HP").GetValue()); ss.clear();
		ss >> currHP; ss.clear();
		mModel.setHP(currHP);
		df::Scavenger::cLocation currPlace = mModel.getAgentLocation();
		df::Scavenger::cell loca = mModel.getCell(currPlace);
		loca.setZ(Z_LOCATION);
		//std::cout << mModel.getCharge() << "charged" << std::endl;
		//std::string cNorth = loca.getNorth();
		//std::string cEast = loca.getEast();
		//std::string cSouth = loca.getSouth();
		//std::string cWest = loca.getWest();
		
		if (this->afifo.empty()){
			//cLocation current_location = mModel.getAgentLocation();	
			//if (current_location.x != gg.x && current_location.y != gg.y){
					
			if (this->first){
				df::Scavenger::Search *mySearch = new df::Scavenger::Search(&mModel);
				mySearch->goSearch();	
				this->afifo = mySearch->getFifo();
				this->first = false;
			}
			if (!this->finished){
				/*this->count = this->afifo.size();
				std::queue<df::Scavenger::Action *> newafifo;
				if (this-afifo.size()){
					this->afifo = newafifo;
					this->finished = true;
				}*/
				df::Scavenger::secretAgentMan *base_path = &mModel;
				gLocation oldGoal = base_path->getGoalLocation();
				cLocation new_start;
				new_start.x = oldGoal.x;
				new_start.y = oldGoal.y;
				gLocation new_goal;
				new_goal.x = X_LOCATION;
				new_goal.y = Y_LOCATION;
				new_goal.z = Z_LOCATION;
				base_path->setGoalLocation(new_goal);
				base_path->setAgentLocation(new_start);
				df::Scavenger::Search *myBaseSearch = new df::Scavenger::Search(base_path);
				myBaseSearch->goSearch();
				std::queue<df::Scavenger::Action *> temp = myBaseSearch->getFifo();
				//std::cout << "fifo copied into temp" << std::endl;
				while (!temp.empty())
				{
					this->afifo.push(temp.front());
					temp.pop();
				}
				this->finished = true;
			
				//this->finished = true;
				//std::cout << "been there once no need to do it again " << std::endl;
			}			
		}
		df::Scavenger::Action* currentAct = this->afifo.front();
		
		if (afifo.empty() && !this->first && this->finished){
			std::cout <<"exxxxxxxxxxxxxxxxxxxxxxity" << std::endl;
			
			ai::Scavenger::Action *action = new ai::Scavenger::Action;
		   	action->SetCode(ai::Scavenger::Action::QUIT);
		   	return action;
		}

		if (currentAct->getAction() == Action::GO_NORTH)
		{
			//std::cout << "North" << std::endl;
			ai::Scavenger::Action *action = new ai::Scavenger::Action;
           	action->SetCode(ai::Scavenger::Action::GO_NORTH);
		   	//std::cout << " quit " << std::endl;
			this->afifo.pop();
           	return action;
		} else if (currentAct->getAction() == Action::GO_SOUTH){
			ai::Scavenger::Action *action = new ai::Scavenger::Action;
           	action->SetCode(ai::Scavenger::Action::GO_SOUTH);
		   	//std::cout << " quit " << std::endl;
			this->afifo.pop();
           	return action;
		} else if (currentAct->getAction() == Action::GO_EAST){
			ai::Scavenger::Action *action = new ai::Scavenger::Action;
           	action->SetCode(ai::Scavenger::Action::GO_EAST);
		   	//std::cout << " quit " << std::endl;
			this->afifo.pop();
           	return action;
		} else if (currentAct->getAction() == Action::GO_WEST){
			ai::Scavenger::Action *action = new ai::Scavenger::Action;
           	action->SetCode(ai::Scavenger::Action::GO_WEST);
		   	//std::cout << " quit " << std::endl;
			this->afifo.pop();
           	return action;
		} else {
			std::cout <<"exxxxxxxxxxxxxxxxxxxxxxity" << std::endl;
			
			ai::Scavenger::Action *action = new ai::Scavenger::Action;
		   	action->SetCode(ai::Scavenger::Action::QUIT);
		   	return action;
		}
		
				
	


    }
  }
}
