#include "OthelloAI.hpp"

namespace mings1
{
	class MyOthelloAI : public OthelloAI
	{
	public:
		virtual std::pair<int, int> chooseMove(const OthelloGameState& state);
	};
}
