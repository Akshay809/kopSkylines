#include <Data.h>

void DataValue::updateIfLargerThan(DataValue& I) {
	/*Implement : check for typeId matching*/
	if(this->compareWith(I)==1)
		this->updateTo(I);
}

void DataValue::updateIfSmallerThan(DataValue& I) {
	/*Implement : check for typeId matching*/
	if(this->compareWith(I)==-1)
		this->updateTo(I);
}

int IntDataValue::compareWith(DataValue &I) {
	int Ivalue = I.objectReference()->getValue();
	if(this->value < Ivalue) return -1;
	else if(Ivalue == this->value) return 0;
	return 1;
}

void IntDataValue::updateTo(DataValue &I) {
	this->value = I.objectReference()->getValue();
}

int DoubleDataValue::compareWith(DataValue& I) {
	double Ivalue = I.objectReference()->getValue();
	if(this->value < I.value) return -1;
	else if(I.value == this->value) return 0;
	return 1;
}

void DoubleDataValue::updateTo(DataValue& I) {
	this->value = I.objectReference()->getValue();
}
