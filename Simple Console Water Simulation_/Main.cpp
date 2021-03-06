#include <iostream>
#include <chrono>
#include <thread>
#include "Display.h"
#include "UsefulConstants.h"
#include "Array1DThatIsActually2D.h"
#include "Simulation.h"

int main()
{
	short *state = new short[gridSizeX * gridSizeY];
	for (int i = 0; i < gridSizeX * gridSizeY; i++)
	{
		int x, y;
		indexToXY(i, x, y, gridSizeX);
		state[i] = isBorder(x, y, gridSizeX, gridSizeY) ? -1 : (fillWithRandomValues ? std::rand() % 7 : 0);
	}

	//setElementIn1DArray(state, 5, 1, gridSizeX, (short)1000);

	double previous = clock() * clockTicksToMs;
	double lag = 0;
	while (true)
	{
		double current = clock() * clockTicksToMs;
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		//simulate
		while (lag >= msPerSimulationStep)//lag >= ms per frame
		{
			advanceSimulation(state, gridSizeX, gridSizeY);
			lag -= msPerSimulationStep;
		}

		//render
		clearConsole();
		display1DArrayThatIsActually2D(state, gridSizeX, gridSizeY);

		std::cout << "ms spent on last cycle: " << elapsed << '\n';

		if (renderStepsPerSecond != 0)
		{
			//wait if designed time (fps) for this frame was not exceeded
			int msSpentDoingThings = (int)((double)(clock() * clockTicksToMs) - current);
			int sleepTime = (int)(msPerRenderStep - msSpentDoingThings); //sleep = time_that_must_be_spent_rendering - time_that_was_spent_rendering
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}
	}
}
