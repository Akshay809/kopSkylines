Directory structure:
- bin
	+ Final executable program(s) goes here
	+ included in gitignore
- build
	+ intermediate object files generated during compilation goes here
	+ included in git ignore
- data
	+ Data source files needed by the program goes here
	+ all data format (including .dat, .csv, .xlsx, .json, .xml) are allowed, but only JSON-subset is supported
- include: header files declaration goes here
	+ rapidjson - a sub-directory actually
		- Header files of "memory based JSON parser for c++"
		- header-only library
		- Source: https://github.com/miloyip/rapidjson
  + Data.h
  	- Includes declration of in-memory data-structures used by the program
  	- Objects defined
  		+ DataValue
  			- an abstract class for declaring the properties of a DataValue
		  	- Data values currently implemented by extending the above class
		  		+ IntDataValue : public DataValue
		  		+ DoubleDataValue : public DataValue
		  		+ StringDataValue : public DataValue
		  	- Why polymorphic reference?
	  			+ each value object is referred by the base class reference in the entire program because,
	  				- the actual value of a DataValue class is a property of the classes derived from it, as it is of different type(int/string/double..), it is not accessible directly by using the reference to base class
	  				- and the reason to have base class refrence, and not use a Union instead, in the first place is to avoid the use of switch-case statements everywhere in the program therby making the current library implementaion more easily extensible
		  	- methods declared in the DataValue class
		  		+ compareWith(V)
		  			- compares the current value with other value of 
		  				+ same type
		  					- comparing Integer type with Integer type
		  				+ of different type
		  					- comparing say a Hexadecimal type with an integer(decimal) type
		  			- define comparison rules in lib/DataValue.cpp
		  			- incomparable, also possible
		  			- missing values supported
		  		+ updateTo(V)
		  			- updates current value, if possible, to the value of object V
					+ minimize
						- sets the value to minimum defined in the value class of that type
					+	createCopy
						- useful like a copy constructor, but for an "abstract class"
  		+ DataInstance
  			- Properties
  				+ unique instance id
  					- number of instance already created in whole world + 1
  				+ ObjectID and ObjectReference
  					- constructed for which object
  					- reverse relation is not implied
  						+ Referred object may not include this instance
  				+ Weight
  					- Used to compute occurence probability
  				+ DataStore
  					- map of Attribute-Value pair
  						+ Attribute
  							- string
  						+ Value
  							- Pointer to a DataValue object
  			- Construction
  				+ Requires the reference of the object for which this instance is being created
  				+ Initializes id and properties
  			- Methods
					+ isMinimumCornerOfU
						- is minimum corner of the object it refers to
					+ isDominatedBy(some_instance)
						- uses the "compareWith" function for every pair of DataValues corresponding to each attribute
						- lesser_than is used as a preference function for every attribute
							+ however definition of 'lesser_than' is subjective
							+ defined in lib/DataValue.cpp
					+ minimize(maximize)WRT(some_instance)
						- updates current_instance as best(worst) of itself and the other instance
						- used in calculating Umin and Umax
					+ minimizeDS
						- minimizes the current DataStore
						- sets every value in the DataStore of the current instance to the minimum value as defined for that type of Value
						- Used for constructing the DataInstanceOrigin
  		+ DataObject
  			- Properties
  				+ unique id: number of objects already created + 1
  				+ list of DataInstances
  				+ Object Weight
  					- sum of weights of all instances present
  				+ Umin(Umax)
  					- Minimum(Maximum) corner of MBB of itself
  			- Construction
  				+ No parameters required
  				+ Initializes id and properties
  			- Methods
  				+ addInstance
  					- Adds a "deep-copy" of the instance to the list
  					- updates the corners and object weight
  					- TODO: Add validation check for DataInstance(if needed) here
  				+ removeInstance
  					- remove by instance id
  					- update the corners and object weight
  				+ get and update functions for DataObject properties
  			- DataObjectOrigin
  				+ First object created
  				- static member
  				- Referenced by DataInstanceOrigin 
  + DataAdapter.h
  	+ Defines the interface for 'FileReader' and 'FileWriter' objects
  		- Extended for specific data formats
  	+ Declaration of 'import' and 'export' functions
  		- Reader object used uses base class referencing, and since required functions declared in the interface are virtual, run time resolution occurs
  + JSONAdapter.h | XMLAdapter.h
  	- Header only extended class definitions for respective Datatype
  	- Described template of supported subset in the begining
  + DataReader.h
  	- Identifies file format and call appropriate sub-routine
  + Exceptions.h
  	- Custom exception declaration
  + Heap.h
  	- Update the heap content
  + RTree.h
  	- Update the RTree content
  + Skylines.h
  	- Update the Skyline content
- lib
	+ implementations of interfaces/class methods declared
	+ Refactored individual components to reduce Code-Smell
- src | test
	+ main programs directory
- Makefile
	+ currently defined the structure, will correct it on completion
- README.md
	+ this file