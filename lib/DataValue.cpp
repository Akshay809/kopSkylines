#include <Data.h>

void DataValue::updateIfLargerThan(DataValue& I) {
	if(this->compareWith(I)==1)
		this->updateTo(I);
}

void DataValue::updateIfSmallerThan(DataValue& I) {
	if(this->compareWith(I)==-1)
		this->updateTo(I);
}

int IntDataValue::min = 0;
const int IntDataValue::type = 1; //Should be unique

int IntDataValue::compareWith(DataValue &I) {
	if(type!=I.getType()) return 2;
	/*TODO|Upgrade: Can compare values of two different types. e.g. comparing 'int' with 'hexadecimal' type, change the typecasting below accordingly*/
	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	int Ivalue = Iptr->getValue();
	if(this->value < Ivalue) return -1;
	else if(Ivalue == this->value) return 0;
	return 1;
}

void IntDataValue::updateTo(DataValue &I) {
	if(type!=I.getType()) return;
	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	this->value = Iptr->getValue();
}

DataValue& IntDataValue::createCopy() {
	DataValue * newVal = new IntDataValue(value);
	return *newVal;
}

double DoubleDataValue::min = 0.0;
const int DoubleDataValue::type = 2; //Should be unique

int DoubleDataValue::compareWith(DataValue& I) {
	if(type!=I.getType()) return 2;

	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	double Ivalue = Iptr->getValue();
	if(this->value < Ivalue) return -1;
	else if(Ivalue == this->value) return 0;
	return 1;
}

void DoubleDataValue::updateTo(DataValue& I) {
	if(type!=I.getType()) return;
	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	this->value = Iptr->getValue();
}

DataValue& DoubleDataValue::createCopy() {
	DataValue * newVal = new DoubleDataValue(value);
	return *newVal;
}

string StringDataValue::min = "";
const int StringDataValue::type = 3; //Should be unique

int StringDataValue::compareWith(DataValue &I) {
	if(type!=I.getType()) return 2;
	/*Can implement lexiographic comparisions*/
	return 2;
}

void StringDataValue::updateTo(DataValue &I) {
	if(type!=I.getType()) return;
	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	this->value = Iptr->getValue();
}

DataValue& StringDataValue::createCopy() {
	DataValue * newVal = new StringDataValue(value);
	return *newVal;
}
