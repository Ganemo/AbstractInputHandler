#pragma once
#include "Windows.h"

#include <vector>
#include <map>
#include <iostream>

using namespace std;

namespace GAInputHandler
{
	/// <summary>
	/// A tuple combining an object and its function
	/// </summary>
	template<class S> struct InputFireTuple
	{
	public:
		S* object;
		void (S::*function)();

		InputFireTuple<S>(S* obj, void(S::*funct)())
		{
			object = obj;
			function = funct;
		}
	};

	/// <summary>
	/// Handles input by firing bound dispatches.
	/// Must be updated every game tick.
	/// </summary>
	template<class T> class InputHandler
	{
	private:
		/// <summary>
		/// all the mappings between virtual keys and function dispatches
		/// </summary>
		map<int, vector<InputFireTuple<T>*>> functionmapings;

	public:
		InputHandler() {};
		~InputHandler() {};

		/// <summary>
		/// Checks all inputs and fires all associated dispatches
		/// </summary>
		void UpdateInput()
		{
			for (int x = 0x00; x < 0xFE; ++x)
			{
				if (GetAsyncKeyState(x))
				{
					for (size_t y = 0; y < functionmapings[x].size(); ++y)
					{
						((*functionmapings[x][y]->object).*(functionmapings[x][y]->function))();
					}
				}
			}
		};
		
		/// <summary>
		/// Adds a function dispatch to an input. Multiple dispatches can be fired from one input.
		/// </summary>
		/// <param name="vKey">Virtual Key code used for input lookup https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx </param>
		/// <param name="object">Object pointer to call function on</param>
		/// <param name="function">Function to bind keypress to</param>
		void SetInputDispatch(int vKey, T* object, void (T::*function)())
		{
			functionmapings[vKey].push_back(new InputFireTuple<T>(object, function));
		};

		/// <summary>
		/// Removes a specific dispatch from being bound from all keys.
		/// </summary>
		/// <param name="object">Object pointer that function is called on</param>
		/// <param name="function">Function being removed</param>
		void RemoveInputDispatch(T* object, void (T::*function)()) 
		{
			for (int x = 0x00; x < 0xFE; ++x)
			{
				for (size_t y = 0; y < functionmapings[x].size(); ++y)
				{
					if (functionmapings[x][y]->object == object && functionmapings[x][y]->function == function)
					{
						functionmapings[x].erase(functionmapings[x].begin() + y);
						y--;
					}
				}
			}
		};

		/// <summary>
		/// Removes a specific dispatch from being bound from a specific key.
		/// </summary>
		/// <param name="vKey">Virtual Key with bound function to be removed</param>
		/// <param name="object">Object pointer that function is called on</param>
		/// <param name="function">Function being removed</param>
		void RemoveInputDispatch(int vKey, T* object, void (T::*function)())
		{
			for (size_t y = 0; y < functionmapings[vKey].size(); ++y)
			{
				if (functionmapings[vKey][y]->object == object && functionmapings[vKey][y]->function == function)
				{
					functionmapings[vKey].erase(functionmapings[vKey].begin() + y);
					y--;
				}
			}
		};

		/// <summary>
		/// Clears all bindings from a key.
		/// </summary>
		/// <param name="vKey">Virtual Key to remove bindings from</param>
		void CleanDispatchesFromInput(int vKey)
		{
			for (size_t y = 0; y < functionmapings[vKey].size(); ++y)
			{
				functionmapings[vKey].erase(functionmapings[vKey].begin() + y);
				y--;
			}
		};
	};
}