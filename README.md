# C-2D-game

Iceman description of public member functions 
	StudentWorld class
1.	GameWorld* createStudentWorld(string assetDir) is the first function in my StudentWorld file. This function was here by default and uses the GameWorld class to create a new StudentWorld object.
2.	StudentWorld(std:string assetDir) : GameWorld(assetDir) is the constructor of my StudentWorld class. It takes all the private and public member variables used in the StudentWorld class and initializes them with default values. This class is also derived from GameWorld so the constructor initializes the GameWorld constructor.
3.	~StudentWorld is the class destructor. This function actually does almost the same operations as the cleanup function is this class. However, the destructor is called when the entire level is trashed(either player died or is on to the next level) so here all dynamically allocated memory is freed and all initializations are cleared as to start the level new. However some variables are kept, like score if the player hasn’t lost all his lives yet. This is because the game actually calls the clean up function most of the time and so the variables are spared. 
4.	int Init is another function that came with the skeleton code. This function is called by GameWorld to allocate memory to the player, ice objects, and ice field. This method is only called once at the start of the game so every object that is supposed to be in the game at the start is there. Init also called another function called makeTHEnpc which is function that calls other function to make ice objects. When this function returns, the game continues. 
5.	 void makeTHEnpc is a simple function that just contains several other function found in StudentWorld. Those functions are called in order and each one makes sure the right amount of each object is made and distributed correctly. The objects are boulders, oil, and gold. 
6.	void protestPicket is the function used to create new protester objects – both regular and hardcore. The function works by calculating the total number of protesters that can be created in that particular level. It then counts the number of protesters currently alive and stores the number in a variable which is then compared to the first calculation mentioned. A variable called T is used a ticker to make sure protested aren’t being created to quickly and so this function keeps track of that too. So then once both numbers are determined, one for total number of protesters allowed and one for a ticker for next protester to be allowed to come if the total number number isn’t exceeed then those numbers are both used to create a new protester object. There is probability associated with whether an hardcore protester will be made or just a regular one. The rand function is used to find that probability. 
7.	void makeWaterGun this function is called when the iceman pulls his squirtgun trigger and fires off a blast of water. The squirt of the icemans gun only goes off(and thus a squirt object is made) if the iceman has enough water left as ammo. Otherwise he would just be firing blanks and no water would be produced to cool off those angry protesters. Thus no new object is made in that case. This function is actually called by another class, the Iceman class in the Actor file. Thus all squirt objects are made on the spot, that is the object is created as the user presses the key to use the squirt gun, ammo permitting. 
8.	void oilSpot is called to create the oil objects which will be distributed across the ice field each level. This function is only called once in the beginning of each level and it works by obtaining a random location to hide the oil within the ice of the icefield. Once the oil location is determined via the rand() function, is makes sure that location is a valid spot, that the function ensures that the oil will be placed under the ice and completely hidden. Once those conditions are satisfied, a new Oil object is made and the entire process is repeated, depending on how many Oil objects need to be created. 
9.	void goldReserve  is a function that makes and distributes the gold across the ice field at the start of each level. It works similarly to other function of the same base class, namely the ActivatingAgent class. As above with oil, the function created the correct amount of gold and places it in random spots in the ice field that are valid in location.
10.	int randomGoodie this function is used to calculate the probability of a one of two object being created in the ice field, namely, a sonar or water pool. It uses the rand() function to determine the probability of each item appearing on the map and is called often in the StudentWorld class.
11.	void waterhole this function, like the ones before it, determine the random location of placing an item, the water pool used to refill the icemans squirt gun. This function makes use of the randomGoodie function to determine the probability of the water appearing in the level for the iceman to pick up. It also verifies that the placement of the item will be valid. This function is called often during the course of the game. 
12.	void sonarActivate this is a simple function that simply calls the randomGoodie function to determine the probability of it appearing in the icefield in any given level. This is because the sonar item is placed in the same location every level and thus the function only needs to determine when a new one should be created.
13.	bool itemMap this function is called by most other iceobject creating function because it operation is somewhat singular. Its purpose is to determine if any iceobject is attempting to be created to close to any other iceobject. The function determines this by cacluating the distance between a given object and all other objects. Much of the time the entire calculation is probably not made because the calculation is dependent on factors that would warrant for it to continue. The formula used is the Euclidean distance formula and each item checks to see the extreme values any x, y could be, and based off that it is determined if the distance is proper for the placement of a new ice object. 
14.	void makeRocks this function finds how many boulders should be made, determines a location, validates that location, then creates a new Boulder object based on how many boulders should be created at the start of each level. 
15.	void makeIce this function makes the icefield objects and stores them in a array of pointers. So the entire icefield is basically projected onto this array. Many location runtime operations by other classes reference the icefield and this is the function that make it.
16.	bool protesterRuled this function takes and x, y location and checks to see if that location is particular distance away from any protester. If the object is class enough the protester, the function makes use of its third parameter to determine which item it is close to. This function all updates the protesters behavior if it returns true. It can notify other functions if the protester is annoyed, or if the iceman got points for affecting the iceman in some way, whether it be with gold, boulders, or his squirt gun.
17.	bool isBoulder is the function that determines if a player or iceman is close to a boulder. It does so by getting the direction that the iceman or protester or squirt gun, and sees if any boulders are going to be overrun by any of those objects heading in that direction. 
18.	void cleanup clean is a function that was given with the skeleton program. It is called by the GameWorld class and is used to ‘clean up’ the level when the iceman dies, or beats a level. It is like the destructor in that is frees up all dynamically allocated memory. 
19.	void clearIce this function is used to clear the icefield of ice if the iceman plows through it with his pickaxe. The function works simply by getting the location of the iceman, his direction, and position relative to the icefield 2d array. Based on this array the function updates the array and clears the iceobjects, setting an element to null and invisible if need be. It also calls the playSound function to emulate the sound of ice being dug through. 
20.	int move this function came with the skeleton and is called by GameWorld repeatedly to determine the course of the game. I designed it so that first it would call functions relating to new items or protesters appearing in the level. After that is calls the iceman player doSomething function and all the other iceobject doSomething function to receive input from the user and also to update the actions of all the objects. After that it checks to see if anything died that turn. If the player dies the game ends. Otherwise all the dead none playable objects are destroyed in a function called removedDeadGameObjects. Next move() checks to see if the iceman has died or has beaten the level and updates his stats accordingly, calling appropriate function and a function to display the states. 
21.	int getCurrentGameLevel  This function is called just to get the game level.
22.	int getNumLivesLeft This function is called to get the number of lives the iceman has left
23.	int getCurrentHealth returns current health of iceman
24.	int getSquirtsLeftInSquirtGun returns number of squirts the iceman has left in his squirtgun
25.	int getPlayerGoldCount returns the amount of gold the iceman is packing
26.	int getNumberOfBarrelsRemaining(bool found) returns the number of barrels remaining in the icefield for that particular level 
27.	int getPlayerSonarChargeCount() returns the amount of sonars the iceman has
28.	int getCurrentScore() returns the current score for the player
29.	string formatDisplay(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score) This function is used to display the stats of the iceman/player and formats it.
30.	void removeDeadGameObject() this function removes dead gameObjects and deletes them after each tick.
31.	void setDisplayText() this function calls other functions to update the stats on the top of the page

	Actor file
	ActorInterface
1.	ActorInterface() this is the default constructor it just initializes the GraphObject parent class.
2.	~ActorInterface() default constructor 
3.	virtual void doSomething() = 0 abstract function since ActorInterface object is never made and this function is never called but every other function calls it 
4.	virtual int getStatus() get status is called by every function it returns a value and tells Student world to if a object should be deleted
5.	virtual void changeTicks(int) both protester functions use this to update their ticks
6.	virtual StudentWorld* getWorld() polymorphic function to reference objects made in StudentWorld every class uses it 
7.	virtual void setVisible() this is function that sets GraphObject’s setVisible(false)
8.	virtual void makeSound(int) this function used to play a each objects sound, never finished it
9.	virtual bool playerRuled(double, double) function to determine if object is close to the iceman
10.	virtual int Xcorr() function to get x coordinate, used by boulder class
11.	virtual int Ycorr() same as above except y coordinate
12.	virtual int protesterHitsPts(int) used by protesters to update their hitpoints 
13.	virtual bool leaveOilField(bool) used by protesters to update their status of being annoyed or not
14.	virtual bool proleaving() returns true if a protester is in the process of leaving, both protesters use it
	IceField : ActorInterface
1.	icefield(int, int, StudentWorld*) ice field constructor used to initialize variables 
2.	virtual ~iceField() default destructor 
3.	virtual void doSomething() override this function is called every tick to determine what the object will do, in iceFields case it does nothing.
	Iceman : ActorInterface
1.	Iceman(StudentWorld*) Iceman constructor used to initialize variables
2.	Virtual ~Iceman() default destructor 
3.	Virtual void doSomething() override called every tick and determines what the iceman will do based on used input(change direction, move, drop gold, etc). Also determines valid movement for iceman
4.	Int getStatus override determines if iceman is dead or not
5.	Int getHealth(int) keeps track of iceman health points 
6.	Int numberOfLivesLeft(int) keeps track of icemans lives
7.	Int getPlayerHitPts(int) keeps track of hit points 
8.	Int getSquirtsLeft(int) keeps track of ammo left for squirt gun or updates it
9.	getPlayerGold(int) keeps track of gold collected as well as decrements if dropped
10.	getPlayerSonarCharge(int) keeps track of sonars left 
	Protester: ActorInterface
1.	Protester(int, StudentWorld*); Protester constructor initializes variables 
2.	virtual ~Protester() {}; default destructor 
3.	bool yellAtIceman(int, int); function used by both protesters, when protester is close enough to iceman they yell at him
4.	bool findIceman(int, int); if iceman is in sight then protester moves towards him
5.	bool withinDistance(int, int); measures distance of protester from iceman
6.	void moveProtester();  updates protestors position the tries to move him
7.	virtual int protesterHitPts(int) = 0;  update protester hitpoints
8.	bool crossroad(Direction&);  if protester is at a intersection
9.	bool canMove(Direction);  checks to see if protester can move
10.	virtual void changeTicks(int) override; updates ticks of a protester if he or she is effected by another object
11.	virtual bool leaveOilField(bool) override; if protester is prompted to leave after being annoyed
12.	virtual bool proleaving() override; protester in process of leaving 
	RegularProtester : Protester
1.	RegularProtester(StudentWorld*); initialize variables
2.	virtual ~RegularProtester() {}; this is virtual because it’s a destructor of a virtual class
3.	virtual void doSomething() override; returns if protester cant move yet, if sees ice man goes after him, or just a general movement is all sourced from here.
4.	virtual int getStatus() override; determines if a protester should be deleted if they are ‘dead’
5.	virtual int protesterHitPts(int) override; updates health of protester
6.	   bool takeABreak(int); is in resting state 
	HardcoreProtester : Protester
1.	RegularProtester(StudentWorld*); note all these function are the same as above 
2.	virtual ~RegularProtester() {};
3.	virtual void doSomething() override;
4.	virtual int getStatus() override;
5.	virtual int protesterHitPts(int) override;
6.	   bool takeABreak(int);
	Boulder : ActorInterface
1.	Boulder(int, int, StudentWorld*); initialize variables 
2.	   virtual ~Boulder() {}; default destructor 
3.	   virtual void doSomething() override; determines if a boulder has any ice beneath it if not it changes state and eventually falls until it hits something
4.	   int getStatus() override; determines if boulder is dead
5.	   int Xcorr() override; gets x position of boulder
6.	   int Ycorr() override; gets y position of boulder
	Squirt : ActorInterface
1.	Squirt(int col, int row, Direction d, StudentWorld*); initializes variables 
2.	   virtual ~Squirt(); default 
3.	   virtual void doSomething() override; controls everything about when squirt is fired off including direction, movement, if it hits any object like a boulder, ice, out of bounds, or a protester. 
4.	   int getStatus() override; determines if squirt has reached its limit and then gets marked for deallocation 
	ActivatingAgent : ActorInterface
1.	ActivatingAgent(int, int, int, StudentWorld*); initializes 
2.	   virtual ~ActivatingAgent() {}; default 
3.	   void doSomething(); this is the control center for all its derived classes, it controls distance and sounds played
	Oil : ActivatingAgent
1.	Oil(int, int, StudentWorld*); initializes 
2.	   virtual ~Oil(); default 
3.	   void virtual doSomething() override; controls whether is oil barrels is visible and score 
4.	   int getStatus() override; marks for destruction if picked up by iceman
	Gold : ActivatingAgent
1.	Gold(int, int, StudentWorld*, bool); initializes 
2.	   virtual ~Gold(); default virtual 
3.	   virtual void doSomething() override; similar as above controls whether a object is dropped or in a pickupable state or not 
4.	   int getStatus() override; controls whether it should be marked for destructor/clean up/deletion 
	SonarKit : ActiavtingAgent 
1.	SonarKit(int, int, StudentWorld*); initializes 
2.	   virtual ~SonarKit() {}; default 
3.	   virtual void doSomething() override; controls how long its active and if functionality if iceman uses it 
4.	   int getStatus() override; marks for deletion 
	WaterPool : ActivatingAgent 
1.	waterPool(int, int, StudentWorld*); initializes 
2.	   virtual ~waterPool() {}; default 
3.	   virtual void doSomething() override; controls how long is active and if within distance from iceman 
4.	   int getStatus() override; if marked for deletion 

Functionalities 
For the most part my program works as intended but I did fail to complete a few thing by the dead line. Regardless, I learned a lot and I am at least somewhat pleased by efforts. Some things I was not able to complete were items regarding the protesters. Both protesters pretty much do the same thing. Also they are killed immediately by the squirt gun. Yes, that is correct, I use that language because the protesters to get wet and leave. They yell their give up sound and then turn around quickly and are immediately taken off the screen. They become invisible. Maybe the iceman was packing more than water? So it seems there is a minor bug regarding that. Also neither protesters are able to exit the icefield because no breadth search sort is implemented. Besides that, there are some formatting issues with the text. 

Design decisions
Design decisions began from the beginning. In fact this entire project has had one glaring aspect to it, and that is that the majority of the time spent working on it came from thinking of ways to implement a feature in a simple and useful way. For the most part I didn’t have trouble with what to program but largely how to program something. It was mostly clear that if I didn’t implement something in a smart, and thoughtful way, that I could end up with a mess on my hands. Therefore I spend a lot of time thinking of how I could build a solid program, one with a back bone, something to hold it all together. I would spend extra time if I was ever stuck on something that just seemed like it need to be shoe horned in the existing program, refusing to just force it through. Some things I would of liked to do is to clean up the code a bit and organize it. Besides that the few trivial issues I had with the spec was the placement of the boulders for example. It was exactly clear to me if any boulder could be partially sticking out of the ice, but this concern was quickly resolved by just playing the finished game repeatedly. 

Class Description 
StudentWorld class. I tested this class use the debugger often. So I could test values produced but helper functions the create new objects. These values included probabilities, locations, distances, outputs, etc. Besides this I checked how the loops created new objects or called the doSomething function for each object. I also spend of a bit of time walking through the destruction of objects. I tested some formatting. 
ActorInterface class. This function was tested by passing it values and initializing variables. I also tested the class by changing keywords like virtual and = 0.
ActivatingAgent class. Tested this function by passing variables and debugging, changing key words and adding member variables.
Boulder class. Tested this class extensively by changing values and even inserting the object in particular places to watch its behavior.
Gold class. Tested this class by changing variables, changing key words, moving function around, and seeing how other functions interacted with the class itself. 
Icefield class. Tested this function by construction and its seeing how it related to its parent class.
Protester class. Tested this function by debugging, passing variables, changing parameters, changing keywords. 
RegularProtester and hardcore protester tested by seeing how it related to its parent class
Squirt, sonarkit, waterpool, oil tested to see how it kept track of interactions in StudentWorld and with the iceman. 

