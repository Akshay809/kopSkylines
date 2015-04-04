## Directory structure:
- **bin**
	- Final executable program(s) goes here
	- included in gitignore
- **build**
	- intermediate object files generated during compilation goes here
	- included in git ignore
- **data**
	- Data source files needed by the program goes here
	- all data format (including .dat, .csv, .xlsx, .json, .xml) are allowed, but only JSON-subset is supported currently
- **include**: header files declaration goes here
	- **rapidjson** - (sub-directory)
		- header-only library
		- Header files of "memory based JSON parser for c++"
		- Source: https://github.com/miloyip/rapidjson
	  - **Data.h**
	  	- Includes declaration of in-memory based data-structures used by the program
	  	- Objects defined
	  		- **DataValue**
	  			- abstract class
			  	- Datavalue types currently implemented by extending the above class
			  		- IntDataValue : public DataValue
			  		- DoubleDataValue : public DataValue
			  		- StringDataValue : public DataValue
			  	- **Why this hierarchical structure with polymorphic reference and *not a Union instead*?**
			  		- Main reason is to avoid the use of switch-case statements everywhere in the program, therby *making the current library implementaion more **easily extensible** *
			  		- Besides we get to define comparison rules for each type sepeartely, which includes comparision rules for similar as well as non-similar types
			  			- **e.g.** comparing Hexadecimal type with Integer(Decimal) type
			  	- **Properties** ( w.r.t. extended DataValue classes )
			  		- Value
			  		- TypeID (static member)
			  		- TypeMinimum Value (static member)
			  	- **Methods**
			  		- **Note:** Relevant methods are declared virtual and are resolved at run-time
			  		- compareWith(V)
			  			- compares the current value with other value of same as well as different type
			  			- define comparison rules in lib/DataValue.cpp
			  			- missing values and different-type comparision (if rules not specified), makes values incomparable
			  		- updateTo(V)
			  			- updates current value, if possible, to the value of object V
			  			- needed as 'Value' is an exclusive property of derived classes and we use base class reference
						- minimize
							- sets the value to minimum as per the MIN defined for that type
						-	createCopy
							- used like a copy constructor, only the value object of the same type is constructed and returned
							- *because this method is virtual*
						-	isDominatedBy(DataInstance|instanceSet)
							- Checks if instance is dominated by given instance or a set of instances
	  		- **DataInstance**
	  			- **Properties**
	  				- unique instance id
	  					- number of instance already created in whole world + 1
	  				- ObjectReference
	  					- of object for which this instance is constructed
	  					- reverse relation is not implied
	  						- Referred object may not include this instance
	  				- Weight
	  					- Used to compute occurence probability
	  				- DataStore
	  					- map of Attribute-Value pair
	  						- Attribute
	  							- string
	  						- Value
	  							- Pointer to a DataValue object
	  					- Generic design: can have any number of attributes, with values of any among the defined types
	  			- **Construction**
	  				- Requires the reference of the object for which this instance is being created
	  				- Initializes id and properties
	  			- **Methods**
		  			  - isMinimumCornerOfU
							  - Checks if current instance is the minimum corner of the MBB of the object it refers to
					  - isDominatedBy(I)
						  - Checks if current instance is dominated by I
						  - uses the "compareWith" function for every pair of DataValues corresponding to each attribute
					    - **IMP: lesser_than is used as a preference function for every attribute **
							  - however **definition of 'lesser_than' is subjective**
							  - defined in lib/DataValue.cpp
					  - minimize(maximize)WRT(some_instance)
						  - updates current_instance as best(worst) of itself and the other instance
						  - used in calculating Umin and Umax
					  - minimizeDS
						  - minimizes the current DataStore
						  - sets every value in the DataStore of the current instance to the minimum value as defined for that type of Value
						  - Used for constructing the DataInstanceOrigin
	  			- **DataInstanceOrigin**
	  				- First '0 weight' instance created, refering DataObjectOrigin
	  				- static member
	  				- has minimum value for each attribute
	  		- **DataObject**
	  			- **Properties**
	  				- unique id: number of objects already created + 1
	  				- list of DataInstances
	  				- Object Weight
	  					- sum of weights of all instances present
	  				- Umin(Umax)
	  					- Minimum(Maximum) corner of MBB of itself
	  			- **Construction**
	  				- No parameters required
	  				- Initializes id and properties
	  			- **Methods**
						- update(get)Minimum(Maximum)Corner
							- Updates/Returns the corners of the MBB of itself
	  				- addInstance
	  					- Adds a "deep-copy" of the instance to the list
	  					- updates the corners and object weight
	  					- **TODO:** Update validation check for DataInstance(if needed) here, currently only weight and object reference of the instance are checked
	  				- removeInstance
	  					- remove by instance id
	  					- update the corners and object weight
	  				- get and update functions for DataObject properties
	  			- **DataObjectOrigin**
	  				- First object created
	  				- static member
	  				- Referenced by DataInstanceOrigin 
	  - **DataAdapter.h**
	  	- Defines the interface for Reader and Writer objects
	  		- Extended for specific data formats
	  		- **Another extensibility feature** of the program, as Data input of any format can be easily supported by using/writing a specific data-format parser and defining the declared functions in the extended class definition
	  	- Declaration of 'import' and 'export' functions
	  - **JSONAdapter.h** | **XMLAdapter.h**
	  	- Header only extended class definitions for respective Datatype
	  	- **Please go through the described template of supported subset in the begining of these files(currently only JSON)**
	  - **DataReader.h**
	  	- Identifies file format and call appropriate sub-routine
	  - **Exceptions.h**
	  	- Custom exception declaration
	  - **Heap.h**
	  	- Mention about Heap Interface, code currently available on 'skyline' and 'top-down' branches
	  - **RTree.h**
	  	- Mention about RTree Interface, code currently available on 'skyline' and 'top-down' branches
	  - **Skyline.h**
	  	- Mention about Skyline Interface, code currently available on 'skyline' and 'top-down' branches
- **lib**
	- implementations of interfaces/class-methods declared above
	- Refactored individual components to reduce Code-Smell
	- Directory structure
		- DataAdapter.cpp
		- DataValue.cpp
		- DataInstance.cpp
		- DataObject.cpp
		- RTree.cpp
		- Heap.cpp
		- p_TopDown.cpp - Definition of functions specific to TopDown Algorithm
		- p_BottomUp.cpp - Definition of functions specific to BottomUp Algorithm
		- kop.cpp - Definition of functions specific to kop Algorithm
- **test**
	- main programs(for test) directory
- **Makefile**
	- currently defined the structure, will correct it on completion
	- To compile code currently use 
		- in build directory
			- g++ -c ./../lib/DataObject.cpp -I ./../include/
			- g++ -c ./../lib/DataInstance.cpp -I ./../include/
			- g++ -c ./../lib/DataValue.cpp -I ./../include/
			- g++ -c ./../test/test_prog.cpp -I ./../include/
		- in bin directory
			- g++ -o test_prog ./../build/DataValue.o ./../build/DataInstance.o ./../build/DataObject.o ./../build/DataAdapter.o ./../build/test_prog.o
			- To Run: ./test_prog ./../data/file002.json
- **README.md**
	- this file