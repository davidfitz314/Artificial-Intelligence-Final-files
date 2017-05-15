#include "df_Problem.h"
#include <cmath> 

namespace df
{
  	namespace Scavenger
  	{
		Problem::Problem(df::Scavenger::State *initial_state_in)
			: ai::Search::Problem(initial_state_in)
		{
			// default default constructor that totally annoys me
		}

		Problem::Problem(df::Scavenger::State *initial_state_in, secretAgentMan *model_in)
		  : ai::Search::Problem(initial_state_in), model(model_in)
	  	{
			// The input parameter, initial_state_in, must be allocated
			// with `new`.  It will be `delete`d automatically in the
			// destructor of this class.  Do not use it or delete it
			// elsewhere.
		}

		Problem::~Problem()
		{
			// Deletes initial_state.
			//if(this->model)
			//{
			//	delete this->model;
			//	this->model = 0;
			//}
			if (!this->model->getExplore()){
				if(this->goal_state)
				{
					delete this->goal_state;
					this->goal_state = 0;
				}
			}
		}

		void Problem::setModel(df::Scavenger::secretAgentMan *model_in)
		{
			this->model = model_in;
		}

		void Problem::setGoal(df::Scavenger::State *goal_in)
		{
			this->goal_state = goal_in;
		}

		void Problem::Display() const
		{
			std::cout << "+-------+" << std::endl;
			std::cout << "***" << this->model->getCharge() << "|" << this->model->getBase() << "|" << this->model->getHP() << "***" << std::endl;
			std::cout << "+-------+" << std::endl;
		}
	
		// Legal and desired actions to consider given state_in.
		// Each of the `Action *` objects must have been allocated
		// with `new`.  They will be deleted elsewhere.
		//
		std::vector<ai::Search::Action *> Problem::Actions(const ai::Search::State * const state_in)
		{
			const df::Scavenger::State * const state = dynamic_cast<const State * const>(state_in);
			std::vector<ai::Search::Action *> actions;
			
			int myX, myY;
			
			df::Scavenger::cell type;
			
			myX = state->getX();
			myY = state->getY();
			cLocation cellular;
			cellular.x = myX;
			cellular.y = myY;
			//std::cout << "*******" << std::endl;
			if (this->model->isMapCell(cellular)){
				type = this->model->getCell(cellular);
			} else {
				//std::cout << "error it is trying to create a new cell" << std::endl;
				return actions;
			}
			double currCharge = state->getCharge();
			
			std::string loca = type.getNorth();
			if (currCharge <= 0){
				actions.clear();
				return actions;
			}
			
			if (loca == "plain" || loca == "mud") {
//				if (currCharge > 5) {
					Action *new_action = new Action();
					new_action->setAction(((int)Action::GO_NORTH));
					//std::cout << "north is here" << std::endl;
					actions.push_back(new_action);
//				}
			}
			loca = type.getEast();
			if (loca == "plain" || loca == "mud") {
//			if (loca != "cliff" && loca != "wall" && loca != "rocks") {

					Action *new_action = new Action();
					new_action->setAction(((int)Action::GO_EAST));
					//std::cout << "east is here" << std::endl;
					actions.push_back(new_action);
//				}
			}				
			loca = type.getSouth();
			if (loca == "plain" || loca == "mud") {
//			if (loca != "cliff" && loca != "wall" && loca != "rocks") {

					Action *new_action = new Action();
					new_action->setAction(((int)Action::GO_SOUTH));
					//std::cout << "south is here" << std::endl;
					actions.push_back(new_action);
			}
			loca = type.getWest();
			if (loca == "plain" || loca == "mud") {
//			if (loca != "cliff" && loca != "wall" && loca != "rocks") {
					Action *new_action = new Action();
					new_action->setAction(((int)Action::GO_WEST));
					//std::cout << "west is here" << std::endl;
					actions.push_back(new_action);
			
			}
			return actions;
		}

		// State that results if in state_in, action_in is taken.
		// The `State *` object must have been allocated with `new`.
		// It will be deleted elsewhere.
		ai::Search::State *Problem::Result(const ai::Search::State * const state_in, const ai::Search::Action * const action_in)
		{
			const df::Scavenger::State * const state = dynamic_cast<const State * const>(state_in);
			const df::Scavenger::Action * const action = dynamic_cast<const Action * const>(action_in);
			int myX = state->getX();
			int myY = state->getY();
			double myZ = state->getZ();
			double myCharge = state->getCharge();
			double newCharge = 0.0;
			int actValue = action->getAction();
			cLocation locaChange;
			df::Scavenger::State *newState;
			if (actValue == action->ActionEnum::GO_NORTH) {
				locaChange.x = myX;
				locaChange.y = myY;
				myY += 1000;
				df::Scavenger::cell tempCell = this->model->getCell(locaChange);	
				if (!this->model->isMapCell(locaChange)){			
					return newState = new df::Scavenger::State();
				}
				
				double newZ = tempCell.getz();
				if (tempCell.getNorth() == "mud") {
					newCharge = myCharge -(((myZ - newZ) * (1/1000)) +2);	
				} else {
					newCharge = myCharge -(((myZ - newZ) / 1000) + 1);
				}
				
				return newState = new df::Scavenger::State(myX, myY, newZ, newCharge);
				//where do i plug in the HP?
			} 
			if (actValue == action->ActionEnum::GO_EAST) {
				locaChange.x = myX;
				locaChange.y = myY;
				myX += 1000;
				//grabbing new cell based on its x y position				
				df::Scavenger::cell tempCell = this->model->getCell(locaChange);
				//error checking
				if (!this->model->isMapCell(locaChange)){			
					//std::cout << "error could not find cell" << std::endl;
					return newState = new df::Scavenger::State();
				}
				double newZ = tempCell.getz();
				//charge = delta z * 1/1000, forgot to add x and y movement to charge change
				if (tempCell.getEast() == "mud") {
					newCharge = myCharge - (((myZ - newZ) * (1/1000)) +2);	
				} else {
					newCharge = myCharge - (((myZ - newZ) / 1000) + 1);
				}

				return newState = new df::Scavenger::State(myX, myY, newZ, newCharge);
				//where do i plug in the HP?
			}
			if (actValue == action->ActionEnum::GO_WEST) {
				locaChange.x = myX;
				locaChange.y = myY;
				myX -= 1000;
				//grabbing new cell based on its x y position				
				df::Scavenger::cell tempCell = this->model->getCell(locaChange);
				//error checking
				if (!this->model->isMapCell(locaChange)){			
					//std::cout << "error could not find cell" << std::endl;
					return newState = new df::Scavenger::State();
				}
				double newZ = tempCell.getz();
				//charge = delta z * 1/1000, forgot to add x and y movement to charge change
				if (tempCell.getWest() == "mud") {
					newCharge = myCharge - (((myZ - newZ) * (1/1000)) +2);	
				} else {
					newCharge = myCharge - (((myZ - newZ) / 1000) + 1);
				}
				return newState = new df::Scavenger::State(myX, myY, newZ, newCharge);
				//where do i plug in the HP?
			}
			if (actValue == action->ActionEnum::GO_SOUTH) {
				locaChange.x = myX;
				locaChange.y = myY;
				myY -= 1000;
				//grabbing new cell based on its x y position				
				df::Scavenger::cell tempCell = this->model->getCell(locaChange);
				//error checking
				if (!this->model->isMapCell(locaChange)){			
					//std::cout << "error could not find cell" << std::endl;
					return newState = new df::Scavenger::State();
				}
				double newZ = tempCell.getz();
				//charge = delta z * 1/1000, forgot to add x and y movement to charge change
				if (tempCell.getSouth() == "mud") {
					newCharge = myCharge - (((myZ - newZ) * (1/1000)) +2);	
				} else {
					newCharge = myCharge - (((myZ - newZ) / 1000) + 1);
				}
				return newState = new df::Scavenger::State(myX, myY, newZ, newCharge);
				//where do i plug in the HP?
			}

		return newState = new df::Scavenger::State();
				
		}

					
		bool Problem::GoalTest(const ai::Search::State * const state_in) const
		{
		  
			const df::Scavenger::State * const state = dynamic_cast<const State * const>(state_in);
			if (this->model->getBaseGoal() && !this->model->getExplore()){
				if (state->getX() == 0.0 && state->getY() == 0.0){
			 		return true; 
				} 
				return false;
			}
			
			if (this->model->getExplore()){
				cLocation new_state;
				new_state.x = state->getX();
				new_state.y = state->getY();
				//error handling
				if (!this->model->isMapCell(new_state)){return false;}
				df::Scavenger::cell temp_cell = this->model->getCell(new_state);
				return temp_cell.getVisited() == false;
			} else {
				
				return *(this->goal_state) == *state;
			}
		}

		// Calculate path cost for the taking action_in in state1_in,
		// producing state2_in.
		//
		double Problem::StepCost(const ai::Search::State  * const state1_in, const ai::Search::Action * const action_in,	const ai::Search::State  * const state2_in) const
		{
			//if your going from 1 state to another do you really need an action?
			//if you were changing a state and its contents then yes but moving no?
			const df::Scavenger::State  * const state1 = dynamic_cast<const State * const>(state1_in);
      		const df::Scavenger::Action * const action = dynamic_cast<const Action * const>(action_in);
      		const df::Scavenger::State  * const state2 = dynamic_cast<const State * const>(state2_in);
			int x1 = state1->getX();
			int y1 = state1->getY();
			cLocation myCell;
			myCell.x = x1;
			myCell.y = y1;
			//error handling
			if (!this->model->isMapCell(myCell)){return 0.0;}
			df::Scavenger::cell loca = this->model->getCell(myCell);
			double z1 = state1->getZ();
			double z2 = state2->getZ();
			int myact = action->getAction();			
			if (myact == action->ActionEnum::GO_NORTH) {
				if (loca.getNorth() == "mud") {
					return (((z1 - z2) * (1/1000)) +2);	
				} else {
					return (((z1 - z2) / 1000) + 1);
				}
			} else if (myact == action->ActionEnum::GO_SOUTH) {
				if (loca.getSouth() == "mud") {
					return (((z1 - z2) * (1/1000)) +2);	
				} else {
					return (((z1 - z2) / 1000) + 1);
				}
			} else if (myact == action->ActionEnum::GO_EAST) {
				if (loca.getEast() == "mud") {
					return (((z1 - z2) * (1/1000)) +2);	
				} else {
					return (((z1 - z2) / 1000) + 1);
				}
			} else if (myact == action->ActionEnum::GO_WEST) {
				if (loca.getWest() == "mud") {
					return (((z1 - z2) * (1/1000)) + 2);	
				} else {
					return (((z1 - z2) / 1000) + 1);
				}
			} 
			// left room for different actions for later on, with different charge/step costs
			
			return 0.0;
		}

		double Problem::Heuristic(const ai::Search::State  * const state_in) const
		{
			if (this->model->getExplore()){
				return 0.0;
			}
			
			const df::Scavenger::State * const state = dynamic_cast<const State * const>(state_in);
			cLocation new_state;
		    new_state.x = state->getX();
		    new_state.y = state->getY();
			//error handling
			if (!this->model->isMapCell(new_state)){return 0.0;}
		    df::Scavenger::cell temp_cell = this->model->getCell(new_state);
		    //if (temp_cell.getVisited() == false){
			//	return 0.0;
		    //}
			
			double gx = this->goal_state->getX();
			double gy = this->goal_state->getY();
			double gz = this->goal_state->getZ();
			//std::cout << gx << " " << gy << " " << gz << std::endl;
			double sx = state->getX();
			double sy = state->getY();
			double sz = state->getZ();
			//std::cout << sx << " " << sy << " " << sz << std::endl;
			double cost = 0.0;
			
			cost += ((abs(gx - sx)/1000) + (abs(gy - sy)/1000) + ((gz - sz)/1000.0));
			return cost;
		}

		void Problem::SetFindAll()
		{
			find_all = true;
		}
  	}
}
