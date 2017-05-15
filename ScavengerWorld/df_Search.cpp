#include "df_Search.h"

namespace df 
{
	namespace Scavenger
	{
		Search::Search(df::Scavenger::secretAgentMan * model_in)
		: world_model(model_in)
		{
			//constructor
			this->mUnvisited = false;
		}

		Search::~Search()
		{
			//deconstructor maybe go delete the model after using it?
		}

		void Search::goSearch()
		{

			
			//first get the worlds model, then generate the start state and the goal state from there 
			//and insert it into problem's class
			cLocation init_state = world_model->getAgentLocation();
			df::Scavenger::cell start_cell = world_model->getCell(init_state);
			
			df::Scavenger::State   *state   = new df::Scavenger::State(start_cell.getX(), start_cell.getY(), start_cell.getz(), world_model->getCharge());
			df::Scavenger::Problem *problem = new df::Scavenger::Problem(state);
			problem->setModel(world_model);

			

			if (world_model->getExplore() && !world_model->getBaseGoal()){
				//std::cout << "+++++explore mode+++++" << std::endl;
				//std::cout << "+++++explore mode+++++" << std::endl;
				//world_model->modelDisplay();
				//std::cout << "++++++++++++++++++++++" << std::endl;
				ai::Search::Frontier *fringe  = new ai::Search::UCFrontier;
				ai::Search::Algorithm *search = new ai::Search::Graph(problem, fringe);
				if(search->Search())
				{
					std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
					std::list<ai::Search::Node *>::const_iterator it;

					for(it = solution->begin(); it != solution->end(); it++)
					{
						if((*it)->GetAction())
						{
							//(*it)->GetAction()->Display();
							const df::Scavenger::Action * const newAcc = dynamic_cast<const Action * const>((*it)->GetAction());
							//newAcc->Display();
							df::Scavenger::Action *newAction = new df::Scavenger::Action();
							newAction->setAction(newAcc->getAction());
					
							this->fifo.push(newAction);
							this->mUnvisited = true;

						}
						if((*it)->GetState())
						{
							//(*it)->GetState()->Display();
						}
						const df::Scavenger::State * const newState = dynamic_cast<const State * const>((*it)->GetState());
						slocation lastxy;
						lastxy.x = newState->getX();
						lastxy.y = newState->getY();
						this->setLast(lastxy);
					}
					
					//std::cout << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
					this->setPathCost(solution->back()->GetPathCost());
					//std::cout << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
					//std::cout << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
					
				}
				
				delete search;
				search = 0;			
				
				
			} else if (!world_model->getExplore() && world_model->getBaseGoal()) {
				//std::cout << "+++++not explore mode+++++" << std::endl;
				//original search with base as goal state

				//setting goal location
				//std::cout << "are we in the else statement for search?" << std::endl;
				
				cLocation goal_key;
				goal_key.x = 0;
				goal_key.y = 0;

				if (!this->world_model->isMapCell(goal_key)){
					std::cout << "Important error, no goal was found" << std::endl;
					return;
				}
				df::Scavenger::cell goal_cell = this->world_model->getCell(goal_key);
				df::Scavenger::State *goal_state = new df::Scavenger::State(goal_cell.getX() , goal_cell.getY(), goal_cell.getz(), world_model->getCharge());			
	
				problem->setGoal(goal_state);
				
			
				ai::Search::Frontier *fringe  = new ai::Search::AStarFrontier;
				ai::Search::Algorithm *search = new ai::Search::Graph(problem, fringe);
				
				if(search->Search())
				{
					std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
					std::list<ai::Search::Node *>::const_iterator it;

					for(it = solution->begin(); it != solution->end(); it++)
					{
						if((*it)->GetAction())
						{
							//(*it)->GetAction()->Display();
							const df::Scavenger::Action * const newAcc = dynamic_cast<const Action * const>((*it)->GetAction());
							//newAcc->Display();
							df::Scavenger::Action *newAction = new df::Scavenger::Action();
														
							newAction->setAction(newAcc->getAction());
							//newAction->Display();
							//std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
							this->fifo.push(newAction);
							
						}

						if((*it)->GetState())
						{
							//(*it)->GetState()->Display();
						}
						
						
						
					}

						

				//std::cout << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
				this->setPathCost(solution->back()->GetPathCost());
				//std::cout << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
				//std::cout << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;

				}

				
			
				delete search;
				search = 0;
				
				
			}
			
			//original search		 
		}


		bool Search::isEmpty()
		{
			return this->fifo.empty();
		}
	
		int Search::qSize()
		{
			return this->fifo.size();
		}

		df::Scavenger::Action *Search::getQueAction()
		{	
			df::Scavenger::Action * qFront = this->fifo.front();
			//this->fifo.pop(); 
			return qFront;
		}

		std::queue<df::Scavenger::Action *> Search::getFifo()
		{
			//std::queue<ai::Search::Action *> outQ;
			//while(!this->fifo.isEmpty())
			//{
			//	outQ.push(this->fifo.front());
			//	this->fifo.pop();
			//}
			//ai::Search::Action fron = *this->fifo.front();
			//fron.Display();
			//std::cout << fron.GetAction() std::endl;
			//std::cout << "original fifo is getting a size of " << this->fifo.size() << std::endl;
			//std::cout << "fifo begining and end in search " << this->fifo.front()->getAction() << "," << this->fifo.back()->getAction() << std::endl;
			//std::cout <<"fifo  address getfifo method "<< this->fifo.back() << std::endl;
			return this->fifo;
		}
		
		void Search::setPathCost(double path_in)
		{
			this->current_path_cost = path_in;
		}	
	
		double Search::getPathCost()
		{
			return this->current_path_cost;
		}

		void Search::setLast(slocation last_in)
		{
			this->lastxy = last_in;
		}

		slocation Search::getLast()
		{
			return this->lastxy;
		}

		void Search::setUnvisited(bool isitVisited){
			this->mUnvisited = isitVisited;
		}

		bool Search::isUnvisited()
		{
			if (this->mUnvisited)
				return true;
			return false;
		}
	
	}
}
