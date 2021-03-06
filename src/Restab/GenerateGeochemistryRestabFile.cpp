/*
 * GenerateGeochemistryRestabFile.cpp
 *
 *  Created on: 15 déc. 2015
 *      Author: delpopol
 */

#include "GenerateGeochemistryRestabFile.h"

using namespace Restab;

int GenerateGeochemistryRestabFile::GeochemistryRestab(GeochemistryParameters& param, std::vector<MasterSolution>& vect_solution){
	StringVector vect_species = param.getSpecies();

	DataCube datacube;

		///====================================
		/// Set Description
		///====================================

		{
			IndexDescription index;

			index.label = "TimeStep";
			index.name  = "iTimeStep";

			datacube.list_of_index_description.add(index);
		}
		{

			VariableDescription var;

			var.label = "Time";

			datacube.list_of_variable_description.add(var);
		}

		Integer num_species = vect_species.size();
		for(Integer idx = 0; idx < num_species; idx++){
			Restab::VariableDescription var;

			var.label = "Amount"+vect_species[idx];

			datacube.list_of_variable_description.add(var);
		}

		///====================================
		/// Add Step Values
		///====================================

		MapInteger step_index_values;
		MapReal step_var_values;

		Integer num_solution = vect_solution.size();

		//!Steps

		for(Integer i = 0; i < num_solution; i++){

			step_index_values.clear();
			step_var_values.clear();

			step_index_values.insert(KeyInteger("TimeStep",i+1));
			step_var_values.insert(KeyReal("Time",vect_solution[i].getT()));

			RealVector y_sol = vect_solution[i].getY();
			for(Integer j = 0; j < y_sol.size(); j++){
				String current_name = "Amount"+ vect_species[j];
				step_var_values.insert(KeyReal(current_name ,y_sol[j]));
			}
			datacube.table_of_index_values.add(step_index_values);
			datacube.table_of_variable_values.add(step_var_values);
		}
		///======================================
		/// Write Results
		///======================================

		String filename = "Geochemistryresult";

		//! Description

		String filenameDesc = filename + ".restab.desc";

		String text_desc = datacube.toStringDescription();
		std::cout << text_desc << "\n";

		datacube.writeDescription(filenameDesc);

		//! Data

		String filenameData = filename + ".restab";

		String text_data = datacube.toStringData();
		//std::cout << text_data << "\n";

		datacube.writeData(filenameData);

		//======================================


		return 0;
}

