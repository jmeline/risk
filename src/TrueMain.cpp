#if 0

// yep... it's a global variable.  But it makes sense here.
std::default_random_engine rng;

int main(int argc, char** argv) {
	//do setup
	//do MPI setup
	//spawn master on 0
		//call needed public functions to do its work
	//spawn slaves on all others
		//call needed public functions to do its work
		//the class "GameSlave" may actually be later destroyed, and its work put here instead
	//clean up MPI stuff
}

#endif
