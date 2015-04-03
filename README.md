Directory structure:
terminology: using "interface" for "abstract class"
- bin
	+ Final executable program(s) goes here
	+ included in gitignore
- build
	+ intermediate object files generated during compilation goes here
	+ included in git ignore
- data
	+ Data source files needed by the program goes here
	+ all data format (including .dat, .csv, .xlsx, .json, .xml) allowed
- include
	+ all header files goes here
  + Subdirectories
  	- 3rdParty
  		+ rapidjson
	  		- Header files of "memory based JSON parser for c++"
	  		- header-only library
	  		- Source: https://github.com/miloyip/rapidjson
    - Data.h
	  	+ Includes definitions of in-memory data-structures used by the program
	  	+ Class definition of
		  		- DataObject
		  			+ Defined by
		  				- unique id
		  					+ number of objects already created + 1
		  			+ Holds
		  				- list of DataInstances
		  			+ Properties
		  				- Object Weight
		  					+ sum of weights of all instances present
		  				- Umin(Umax)
		  					+ Minimum(Maximum) corner of MBB of itself
		  			+ Construction
		  				- No parameters required
		  				- Initializes id and properties
		  			+ Methods
		  				- addInstance
		  					+ Adds a copy 
		  						- deep copy
		  						- not original DataInstance object
		  					to the list
		  					+ updates the corners and object weight
		  					+ TODO: Add validation check for DataInstance(if needed) here
		  				- removeInstance
		  					+ remove by instance id
		  					+ update the corners and object weight
		  				- get and update functions for DataObject properties
		  			+ DataObjectOrigin
		  				- First object created
		  				- static member
		  				- Referenced by DataInstanceOrigin 
		  		- DataInstance
		  			+ Defined by
		  				- unique instance id
		  					+ number of instance already created in whole world + 1
		  				- ObjectID and ObjectReference
		  					+ constructed for which object
		  					+ reverse relation is not implied
		  						- Referred object may not include this instance
		  			+ Holds
		  				- Weight
		  					+ Used to compute occurence probability
		  				- DataStore
		  					+ map of Attribute-Value pair
		  						- Attribute
		  							+ string
		  						- Value
		  							+ Pointer to a DataValue object
		  			+ Construction
		  				- Requires the reference of the object for which this instance is being created
		  				- Initializes id and properties
		  			+ most used methods
						- isMinimumCornerOfU
							+ is minimum corner of the object it refers to
						- isDominatedBy(some_instance)
							+ self explanatory
							+ uses the "compareWith" function for every pair of DataValues corresponding to each attribute
							+ lesser_than is used as a preference function for every attribute
								- however lesser_than relationship for every DataValue is defined seperately
									+ So I can define lesser_than as '>' in say IntDataValue type
										- then 3 will be considered less than 2
									+ Or I can use the lexographic ordering to define lesser_than for string type
						- minimize(maximize)WRT(some_instance)
							+ updates myself(current_instance) as best(worst) of myself and the other instance
							+ used in calculating Umin and Umax
						- minimizeDS
							+ sets every value in the DataStore of the current instance to the minimum value as defined for that type of Value
							+ Used for constructing the DataInstanceOrigin

							??  Should I fix DataValue type for each attribute  ??

		  		- DataValue - an interface actually
	  		goes here
	  	+ Class definitions of extended classes
		  		- IntDataValue : DataValue
		  		- DoubleDataValue : DataValue
		  		- StringDataValue : DataValue
	  		also present here
	  	+ any NewDataValue should be defined here
	  	+ also includes function definition of "isDominated()"
	  		- finds if given instance is dominated by given set of instances
	  		- generalizes the "Dominates" function present in a DataObject, as the set can have instances from any DataObject
	  - DataAdapter.h
	  	+ Defines the interface for 'FileReader' and 'FileWriter' objects
	  		- 
	  	+ Declaration of 'import' and 'export' functions also present here
	  		- Reader object used by these functions is referenced by the Base class, but since most of the function declared in the interface are virtual run time resolution occurs



+ General comments
	- On how Data is read
		+ DataObject having DataInstances with different type of DataStores(non-relational design) is supported
			- BUT currently(I am doing this, but not necessary) all DataInstances are created with [same type of DataStore and with missing values], and this is done across all DataObjects created
				+ Implemented as follows
					- whole Data file is read once
						+ all possible keys are read into an AttributeList
					- whole Data file is read again
						+ For every instance in every object
							- DataStore for that instance is initialized using all the names in the AttributeList as Keys and NULL for Values
							- Value for an attribute is updated by creating and assigning an appropriate DataValue object, if the value for that attribute is actually found
				+ For details refer JSONAdapter, the definition of JSONReader goes here
		+ A subset of JSON format is supported currently
			- described clearly in start of the JSONAdapter header file
			- Can be updated if needed
	- On how DataInstances are compared
		+ PROBLEM1 with current approach
			- Comparing two instances with missing values
				+ Current solution
					- No instance dominates the other
					- Skyline probability are calculated accordingly
		+ PROBLEM2 with current approach
			- Comparing two instances having different types of DataValues for same attribute
				+ Current solution
					- compare function defined for every DataValue type, defines comparision rules for every other DataValue type it can be compared with
					- If no rule exists, i.e. if two different types can't be compared, following is assumed
						+ No instance dominates the other
						+ Skyline probability are calculated accordingly
