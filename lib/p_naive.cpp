/*naive-version of p-Skyline*/
#include <Data.h>

using namespace std;

void p_(const objectSet& data, double p, vector<const DataObject*>& Skyline) {

	for(auto itr = data.cbegin(); itr!=data.cend(); ++itr) {
		/*For every object*/

		const DataObject* U = *itr;
		double pO = 0;	/*Object skyline-probability*/

			const vector<DataInstance>& instances = U->getDataInstances();

			for(auto it = instances.cbegin(); it != instances.cend(); ++it) {
				/*For every instance of an object*/

				const DataInstance& u = *it;
				double pu = 1;	/*Instance skyline-probability*/

				for(auto i = data.cbegin(); i != data.cend(); ++i) {
					/*For every other object*/
					if(i==itr) continue;

					const DataObject& V = **i;
					const vector<DataInstance>& v_Instances = V.getDataInstances();

						double _pu = 0;	/*Probaility that object V dominates current instance*/

						for(int j = 0; j < v_Instances.size(); ++j) {

							const DataInstance& v = v_Instances[j];

							if(u.isDominatedBy(v)) {
								_pu += v.getProbability();
							}
						}
						pu *= (1-_pu);	/* prob. that current instance is not dominated by this object and ...*/
				}

				/*Object s-p += Instance Ocurrence-p * Instance s-p*/
				pO += u.getProbability() * pu;
			}

			if(pO>=p) {
				Skyline.push_back(U);
			}
	}
}
