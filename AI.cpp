#include <ics46/factory/DynamicFactory.hpp>
#include "MyOthelloAI.hpp"
#include <iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, mings1::MyOthelloAI, "Ming's Othello AI (Required)");

int evaluation(std::unique_ptr<OthelloGameState>& copyState){
	int width = copyState->board().width();
	int height = copyState->board().height();
	std::vector<std::pair<int,int>> moveVector;
	for (int i = 0; i<width; i++){
		for(int j = 0; j<height; j++){
			if (copyState->isValidMove(i,j) == true){
				moveVector.push_back(std::pair<int,int> {i,j});
			}
		}
	}
	if(copyState->isWhiteTurn() == true){
		return 100*(copyState->whiteScore()-copyState->blackScore())/(copyState->blackScore()+copyState->whiteScore())
		+(moveVector.size()/(64-(copyState->blackScore()+copyState->whiteScore())))*100;
	}
	else{
		return 100*(copyState->blackScore()-copyState->whiteScore())/(copyState->blackScore()+copyState->whiteScore())
		-(moveVector.size()/(64-(copyState->blackScore()+copyState->whiteScore())))*100;
	}
}

int search(std::unique_ptr<OthelloGameState>& state, int depth, std::string turn){
	std::vector<std::pair<int,int>> moveVector;
	for (int i = 0; i<state->board().width(); i++){
		for(int j = 0; j<state->board().height(); j++){
			if (state->isValidMove(i,j) == true){
				moveVector.push_back(std::pair<int,int> {i,j});
			}
		}
	}
	int maxium = -10000;
	int minium = 10000;
	int value;
	if ((state->isGameOver()!= true) ||depth == 0){
		return evaluation(state);
	}
	else{
		if((turn == "black" && state->isBlackTurn() == true) || (turn == "white" && state->isWhiteTurn() == true)){
			for(int index = 0; index < moveVector.size(); index++){
				std::unique_ptr<OthelloGameState> copyState = state->clone();
				copyState->makeMove(moveVector[index].first, moveVector[index].second);
				value = search(copyState, depth-1, turn);
				if (value >= maxium){
					maxium = value;
				}
			}
			return maxium;
		}
		else{
			for(int index = 0; index < moveVector.size(); index++){
				std::unique_ptr<OthelloGameState> copyState = state->clone();
				copyState->makeMove(moveVector[index].first, moveVector[index].second);
				value = search(copyState, depth-1, turn);
				if (value <= minium){
					minium = value;
				}
			}
			return minium;
		}
	}
}


std::pair<int,int> mings1::MyOthelloAI::chooseMove(const OthelloGameState& state){
	std::string turn;
	int best = -10000;
	int value;
	if(state.isBlackTurn() == true){
		turn = "black";
	}
	else{
		turn = "white";
	}
	std::vector<std::pair<int,int>> moveVector;
	for (int i = 0; i<state.board().width(); i++){
		for(int j = 0; j<state.board().height(); j++){
			if (state.isValidMove(i,j) == true){
				moveVector.push_back(std::pair<int,int> {i,j});
			}
		}
	}
	std::pair<int, int> bestMove (moveVector[0].first, moveVector[0].second);
	for (int index = 0; index < moveVector.size(); index++){
		std::unique_ptr<OthelloGameState> copy = state.clone();
		copy->makeMove(moveVector[index].first, moveVector[index].second);
		value = search(copy, 2, turn);
		if (value >= best){
			best = value;
			bestMove.first = moveVector[index].first;
		    bestMove.second = moveVector[index].second;
		}
	}
	return bestMove;

}

