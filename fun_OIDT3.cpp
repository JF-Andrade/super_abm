//#define EIGENLIB			// uncomment to use Eigen linear algebra library
//#define NO_POINTER_INIT	// uncomment to disable pointer checking

#include "fun_head_fast.h"

// do not add Equations in this area

MODELBEGIN


EQUATION( "Z_Q" )

	/*
	Autonomous expenditures-Output ratio
	*/
	
	v[0] = V("Auton");		// Autonomous expenditures
	v[1] = V("basic_total");	// Aggregate Basic Income
	v[2] = V("Q_t");			// Output
	
	v[3] = (v[0] + v[1])/v[2];

RESULT( v[3] )


EQUATION( "Auton" )

	/*
	Gasto Autônomo
	*/
   
	v[0] = VL("Auton", 1);
	v[1] = V("c");		// Autonomous expenditures' growth rate
   
	v[2] = (1 + v[1])*v[0];
   
RESULT( v[2] )


EQUATION( "basic" )

	/*
	"Renda básica universal" por agente
	*/
	
	v[0] = V("minsal_t");	// minimum wage in t
	v[1] = V("p_minsal");	// minimum wage proportion
	v[2] = V("c_basic");	// basic income growth

	v[3] = (1 + v[2])*(v[0]*v[1]);

RESULT(v[3])


EQUATION( "basic_total" )
	
	/*
	Aggregate or total basic income
	*/
	
RESULT( SUM("basic") )


EQUATION( "Q_fc" )
	
	/*
	Full capacity output
	*/
	
	v[1] = V("K_t");		// Capital stock
	v[2] = V("gamma"); 	// inverso da produtividade de K
	v[3] = v[1]/v[2]; 	// Warning: supply constraint
	v[4] = V("xi"); 		// Labor Productivity
	v[5] = V("N");		// Number of workers
	v[6] = v[4]*v[5]; 	// 'Warning: labor supply constraint'
	
	v[7] = min(v[3],v[6]);

RESULT( v[7] )


EQUATION( "u_t" )

	/*
	Capacity utilization equation
	*/
	
	v[0] = V("Q_t");		//
	v[1] = V("Q_fc"); 	// 
	v[2] = v[0]/(v[1]);
	/*
	if (v[2] >= 1)
		{v[2] = 1;}
	else {v[2] = v[2];}
	
	if (v[2] < 0)
		{v[2] = 0;}
	else {v[2] = v[2];}
	*/

RESULT( v[2] )


EQUATION("r")

	/*
	Profit rate
	*/

RESULT((V("pi")*VL("u_t",1))/V("gamma") )


EQUATION( "I_t" )

	/*
	Investment equation
	*/
	
	v[20] = V("flag_super");		// Supermiltiplier equation flag
	
	if (v[20] == 1){				// If==1, use Supermultiplier;  otherwise, use Dutt or Marglin&Bhaduri
	
		v[0] = VL("Q_t",1);		// Output
		v[1] = VL("h",1);			// Marginal propensity to invest
		v[2] = v[0]*v[1];
		v[15] = v[2]; 			// For compability reasons
	} else {
		if(V("flag_dutt") == 0 && V("flag_marglin") == 1){ 										// Assegura que não são simultaneos
			v[15] = (V("param_auton") + V("profit_param")*V("pi") + V("gap_param")*VL("u_t",1))*VL("K_t", 1); 	// Bhaduri
		} else if (V("flag_dutt") == 1 && V("flag_marglin") == 0){
			v[15] = (V("param_auton") + V("profit_param")*V("r") + V("gap_param")*VL("u_t",1))*VL("K_t", 1); 	// Dutt
		}
	}

RESULT( v[15] )


EQUATION( "h" )
	
	/*
	Investment adjustment - Marginal propensity to invest
	*/
	
	v[20] = V("flag_super");
	if(v[20] == 1){
		v[0] = VL("h",1);
		v[1] = V("gamma_u");		// adjustment sensibility
		v[2] = VL("u_t",1);		// capacity utilization
		v[3] = V("ud");			// normal capacity utilization
		v[4] = v[0]*(1 + v[1]*(v[2] - v[3]));
	} else {
		v[0] = V("Q_t");			// output
		v[1] = V("I_t");			// Investment
		v[4] = v[1]/v[0];
		}
		
RESULT( v[4] )


EQUATION( "K_t" )

	/*
	Capital stock equation. K in t-1 + Eq. (8)
	*/
	
	v[0] = V("delta");			// Depreciation rate
	v[1] = VL("K_t", 1);			// Capital stock in t-1
	v[2] = V("I_t");				// Total investiment in t
	
	v[3] = (1 - v[0])*v[1] + v[2];	// Capital Stock in t

RESULT( v[3] )


EQUATION( "price_t" )

	/*
	Calculate the price. Eq (9)
	*/

	v[0] = V("mu");				// mark-up parameter
	v[1] = V("wbarocc");			// average wage of employed workers in t
	v[2] = V("xi");				// Labor-produtivity (Q/L)
	v[3] = (1+v[0])*(v[1]/v[2]);
	
RESULT( v[3] )


EQUATION( "psi" )

	/*
	Wage-share equation. Eq (10)
	*/

	v[0] = V("mu"); 			// mark-up parameter
	v[1] = 1/(1+v[0]); 

	PARAMETER //Transform the variable into a parameter.

RESULT( v[1] )


EQUATION( "pi" )

	/*
	Profit-share equation. Eq (11)
	*/
	
	v[0] = V("mu"); 			// mark-up parameter
	v[1] = (v[0])/(1+v[0]); 

	PARAMETER //Transform the variable into a parameter.

RESULT( v[1] )


EQUATION( "A_t" )

	/*
	Retained profit. Eq. (12)
	*/
	
	v[1] = V("pi");				// Profit share
	v[2] = V("price_t");			// Price in t
	v[3] = V("Q_t");				// Output
	v[4] = V("i");				// Interest rate
	v[5] = VL("B_t", 1);			// Bonds in t-1

	v[6] = (v[1]*v[2]*v[3] - v[4]*v[5]);

RESULT( v[6] )


EQUATION( "B_t" )

	/*
	Stock of Bonds. Bonds in t-1 + Eq. (13)
	*/

	v[0] = VL("B_t", 1);			// Bonds in t-1
	v[1] = 1;					// Share of investment funded by debt
	v[2] = V("I_t");				// Total investiment in t
	v[3] = V("A_t");				// Retained profits in t

	v[4] = v[0] + v[1]*(v[2] - v[3]);

RESULT( v[4] )


EQUATION( "state_b_it" )

	/*
	State of workers - borrow behavior. 1= borrowing; 0= not borrowing. Eqs. (21) and (22)
	*/
	
	if(V("leque") >= V("Acesso_Credito"))
		v[2] = 1;				// If it is true, the worker can borrow
	else
		v[2] = 0;				// Otherwise, he can't

RESULT( v[2] )


EQUATION( "Yw_t" )

	/*
	Workers' Total income. Eq. (23)
	*/
	
	v[20] = V("flag_deposits");		// 1 se depósitos remunerados, 0 caso contrário
	v[0] = V("w_it");				// Wage in t
	v[1] = V("i");				// Interest rate
	v[2] = VL("D_t", 1);			// Workers' debt in t-1
	v[3] = V("basic");			// Renda básica
	v[4] = VL("Mw_t",1);
	
	v[5] = v[0] - v[1]*v[2] + v[3] + v[20]*(v[1]*v[4]);

RESULT( v[5] )


EQUATION("Ydw_t")
	
	/*
	Workers' DISPOSABLE Income
	*/

	v[0] = V("Yw_t");			// Worker Income
	v[1] = V("Tax_w");		// Workers' tax payment	
	v[2] = v[0] - v[1];

RESULT( v[2] )


EQUATION("Ydw_total")
	/*
	Aggregate disposable income
	*/
RESULT( SUM("Ydw_t") )


EQUATION("Tax_w")
	
	/*
	Workers' tax payment
	*/

	v[0] = V("Tax");			// Tax aliquot
	v[1] = V("Yw_t");			// Worker Income
	v[2] = VL("state_it", 1);	// Employment status
	v[3] = v[0]*v[1]*v[2];

RESULT( v[3] )


EQUATION( "Sw" )

	/*
	Workers' savings. p. 7
	*/

	v[0] = V("Ydw_t");		// Workers' disposable income in t
	v[1] = V("Cw_t");			// Workers' consumption in t
	
	v[2] = v[0] - v[1];

RESULT( v[2] )


EQUATION( "Sw_total" )

	/*
	Workers' Total savigns
	*/

RESULT( SUM("Sw") )


EQUATION( "Ww_t" )

	/*
	Workers' wealth. Ww_1 in t-1 + Eq. (25)
	*/

	v[0] = VL("Ww_t", 1);		// Workers' wealth in t-1
	v[1] = V("Sw");			// Workers' disposable income in t
	v[2] = VL("D_t",1);		// Worker debt
	v[4] = V("D_t");
	
	v[3] = v[0] + v[1] - (v[4] - v[2]);

RESULT( v[3] )


EQUATION( "Ww_total" )

	/*
	Workers' Total Wealth.
	*/

RESULT( SUM("Ww_t") )


EQUATION( "Mw_t" )

	/*
	Workers’ demand for money. Eqs. (27) and (28)
	*/
	
	v[0] = VL("Mw_t", 1);		// Worker's amount of money in t-1
	v[1] = V("Sw");			// Worker's savings in t
	
	v[2] = v[0] + v[1];

RESULT( v[2] )


EQUATION( "Mw_total" )

	/*
	Workers' Total demand for money. Eq. (29)
	*/

RESULT( SUM("Mw_t") )


EQUATION( "D_t" )

	/*
	Workers' debt derived from Eq. (26)
	*/

	v[0] = VL("D_t", 1);		// Workers' debt in t-1
	v[1] = V("Sw");			// Workers' savings in t
	v[2] = VL("Mw_t", 1);		// Each Workers' amount of money in t-1

	v[3] = v[0] - (v[1] + v[2]);

RESULT( v[3] )


EQUATION( "D_total" )

	/*
	Workers' TOTAL debt derived from Eq. (30)
	*/

RESULT( SUM("D_t") )


EQUATION( "Cw_t" )

	/*
	Workers' consumption. Eq. (31) - ALTERADO
	*/

	v[0] = V("spsi");			// Propensity to save out of workers' income
	v[1] = V("w_it");			// Wage in t
	v[2] = V("eta");			// Sensitivity to workers’ relative income
	v[3] = V("wbarocc");		// Average wage of employed workers in t-1
	v[4] = V("sigmapsi");		// Propensity to save out of workers' wealth
	v[5] = VL("Mw_t", 1);		// Workers' wealth in t-1
	v[7] = V("Ydw_t");		// DISPOSABLE Income in t

	v[6] = (1 - v[0])*v[7]  + (v[5] >= 0)*(1 - v[4])*v[5] + v[2]*(v[3] - v[1]); 

	if(V("state_b_it") == 1){	// Credit access status
		v[6] = v[6];
	} else {
		if (v[6] > v[7]){
			v[6] = v[7];
		} else {
			v[6] = v[6];
		}
	}
	
RESULT( v[6] )


EQUATION( "Cw_total" )

	/*
	Workers' Total Consumption. Eq. (32)
	*/

RESULT( SUM("Cw_t") )


EQUATION( "Wfinance" )

	/*
	Financial sector wealth. From Eq. (33)
	*/

	v[0] = V("i");				// interest rate
	v[1] = VL("D_total", 1);		// Workers' TOTAL debt in t-1
	v[2] = VL("B_t", 1);			// Bonds in t-1
	v[3] = VL("Wfinance", 1);		// Financial sector wealth in t-1
	
	v[4] = v[3] + v[0]*(v[1] + v[2]);

RESULT( v[4] )


EQUATION( "Q_t" )
	
	/*
	Output
	*/

	v[0] = V("Cw_total");			// Total workers' consumption in t
	v[2] = V("I_t");				// Investment in t
	v[4] = V("price_t");			// price
	v[5] = V("Auton");			// Autonomous expenditures
	
	v[3] = (v[0] + v[2] + v[5])/v[4];
	
RESULT( v[3] )


EQUATION( "M_TOTAL" )

	/*
	Total money in the system - not used or revised yet
	*/

RESULT( V("Mw_total"); )


EQUATION( "EL_t" )

	/*
	Number of employed workers
	*/
	
	v[0] = V("N");				// Total numbers of workers
	v[1] = V("Q_t");				// Output in t
	v[2] = V("xi");				// Labor productivity
	v[3] = round(v[1]/v[2]);		// Round of output-labor productivity ratio

	v[4] = min(v[0],v[3]);

	if (v[4] <= 0)
		v[4] = 1;

	v[5] = v[4]; // Contador
	
	CYCLE(cur, "Consumer")
	{
		if ( RND < v[4] / v[0])
		{	
			WRITES(cur, "state_it", 1);
			v[5] --;
		} 
		else
		{ 
			WRITES(cur, "state_it", 0);
		}
	}

RESULT( v[4] - v[5] )


EQUATION( "size_unemp" )

	/*
	Number of unemployed workers
	*/
	
	v[0] = V("N");			// Total numbers of workers
	v[1] = V("EL_t");			// Number of employed workers
	v[2] = v[0] - v[1];

RESULT( v[2] )


EQUATION( "UnRate" )

	/*
	Unemployment rate
	*/
	
	v[0] = V("N");			// Total numbers of workers
	v[1] = V("size_unemp");		// Number of employed workers

	v[2] = v[1]/v[0];

RESULT( v[2] )


EQUATION( "wbarocc" )

	/*
	Average wage of employed workers
	*/

	v[0] = SUM("w_it");
	v[2] = VL("EL_t", 1);			// Number of employed workers

	v[3] = v[0]/v[2];

RESULT( v[3] )


EQUATION( "Yw_total" )

	/*
	Workers' Total income
	*/

RESULT( SUM("Yw_t") )


EQUATION( "minsal_t" )

	/*
	Update minimum salaries.
	*/
	
	v[0] = VL("minsal_t", 1); 		// minsal in t-1
	v[1] = VL("inflation",1);		// Inflation rate. To fix deadlock error
	
	if (v[1] > 0)
		v[2] = v[0]*(1 + v[1]); 	// minimun wage in t
	else
		v[2] = v[0];

RESULT( v[2] )


EQUATION("leque")
	
	/*
	Leque salarial com distribuição de pareto com shape=1 e mean=1 tal como no artigo original
	*/
	
	v[0] = V("shape");
	v[1] = V("pareto_mean");
	v[2] = pareto(v[1], v[0]);
	
	PARAMETER

RESULT(v[2])


EQUATION( "inflation" )

	/*
	Inflation rate
	*/
	
	v[0] = VL("price_t",1);
	v[1] = V("price_t");
	v[2] = (v[1]-v[0])/v[0];

RESULT( v[2] )


EQUATION_DUMMY( "state_it" , "EL_t")

	/*
	State of worker i at time t (Inactive=0, Active=1)
	*/


EQUATION( "w_it" )

	/*
	Updates wages by worker, evaluates wich one is below the minimum and set them to it.
	*/
	
	v[0] = V("leque")*VL("minsal_t", 1);	// Minimum wage proportion
	v[1] = VL("inflation",1);			// Inflation rate. To Fix Deadlock error

	if (v[1] >= 0){
		v[4] = v[0]*(1 + v[1]);
	} else {
		v[4] = v[0];
		}
	
	v[5] = VL("state_it",1);

	v[4] = v[4]*v[5]; 			// If unemployed, w_it = 0

RESULT( v[4] )


EQUATION( "W" )

	/*
	Total Wealth, workers and managers
	*/

	v[0] = V("Ww_total");		// Workers' Aggregate Wealth

RESULT( v[0] )


EQUATION( "Yf_t" )

	/*
	Financial sector wealth. Eq. (33). Need revision
	*/

	v[0] = V("i");				// Interest rate
	v[1] = VL("D_total", 1);		// Workers' debt in t-1
	v[2] = VL("B_t", 1);			// Bonds in t-1
	v[3] = VL("Gov_Debt",1);		// Government Debt
	v[4] = VL("M_TOTAL",1);			// Amount of money

	v[5] = v[0]*(v[1] + v[2] + v[3] - v[4]);

RESULT( v[5] )


EQUATION( "M_t" )

	/*
	Amount of money. Need revision
	*/
	
	v[0] = V("W");		// Total wealth in t

RESULT(v[0])


EQUATION("Borrow")

	/*
	Quantos efetivamente tomam empréstimo
	*/

	v[0] = V("D_t");			// Worker debt
	v[1] = VL("D_t", 1);
	
	if(v[0] > v[1])
		v[2] = 1;
	else{
	v[2] = 0;
	}

RESULT(v[2])


EQUATION("Share_Borrow")

	/*
	Ratio of workers that effectively borrow
	*/
	
	v[0] = SUM("Borrow");
	v[1] = V("N");
	v[2] = (v[0]/v[1]);

RESULT( v[2] )


EQUATION( "numOfBorrowWorkers_t" )

	/*
	Number of Borrowers
	*/
	
RESULT( SUM("state_b_it") )


EQUATION( "shareOfBorrWorkers_t" )

	/*
	Share of Borrowers
	*/
	
	v[0] = V("numOfBorrowWorkers_t");	// Number of borrowers
	v[1] = V("N");				// Total number of workers

	v[2] = v[0]/v[1];

RESULT( v[2] )


EQUATION( "numOfNonBorrowWorkers_t" )

	/*
	Number of non-borrower workers
	*/
	
	v[0] = V("numOfBorrowWorkers_t");	// Number of non-borrowers
	v[1] = V("N");				// Total number of workers

	v[2] = v[1] - v[0];

RESULT( v[2] )


EQUATION( "shareOfNonBorrWorkers_t" )

	/*
	Share of Non-Borrowers
	*/
	
	v[0] = V("numOfNonBorrowWorkers_t");	// Number of non-borrowers
	v[1] = V("N");				// Total number of workers

	v[2] = v[0]/v[1];

RESULT( v[2] )


EQUATION( "Y_total" )

	/*
	Total income
	*/

	v[0] = V("Yw_total");			// Workers' total income
	v[1] = V("Yf_t");
	v[2] = v[0];

RESULT( v[2] )



EQUATION("Gov_Total")
	
	/*
	Government total expenditure (fully autonomous)
	Warning: DO NOT include Gov_Total in GDP calculation. 
	Reason: basic income already included in consumers disposable income
	*/
	
	v[0] = V("Auton");
	v[1] = V("basic_total");
	v[2] = v[0] + v[1];

RESULT( v[2] )


EQUATION("Tax_Total")

	/*
	Government total revenue collect at consumers disposable income
	*/

RESULT( SUM("Tax_w") )


EQUATION("Gov_Balance")
	
	/*
	Government net Balance (+) if surplus, (-) otherwise
	*/
	
	v[0] = V("Gov_Total");
	v[1] = V("Tax_Total");
	v[2] = v[1] - v[0];

RESULT( v[2] )


EQUATION("Gov_Debt")
	
	/*
	Government debt
	*/

	v[0] = V("i"); 			// Interest rate
	v[1] = V("Gov_Balance");	// Primary Surplus
	v[2] = VL("Gov_Debt",1);	// Gov Debt
	v[3] = (1+v[0])*v[2] - v[1];

RESULT( v[3] )


EQUATION("Gini_bTax")
/*
Gini calculation before taxes
*/

v[0] = V("N");
v[1] = SUM("Yw_t"); // Sum all income
v[2] = v[0]*v[1]; // Denominador
v[3] = 1; // Consumer index start from 1 to N
v[4] = 0; // Resultado do numerador
SORT( "Consumer", "Yw_t", "UP" ); // Values must be sorted

CYCLE(cur, "Consumer")
	{
	v[5] = VS(cur, "Yw_t");
	v[6] = (2*v[3] - v[0] - 1)*v[5];
	v[4] = v[4] + v[6];
	v[3] = v[3] + 1;
	}
	
v[7] = v[4]/v[2];


RESULT(v[7])

EQUATION("Gini_aTax")
/*
Gini calculation after taxes
*/

v[0] = V("N");
v[1] = SUM("Ydw_t"); // Sum all income
v[2] = v[0]*v[1]; // Denominador
v[3] = 1; // Consumer index start from 1 to N
v[4] = 0; // Resultado do numerador
SORT( "Consumer", "Ydw_t", "UP" ); // Values must be sorted

CYCLE(cur, "Consumer")
	{
	v[5] = VS(cur, "Ydw_t");
	v[6] = (2*v[3] - v[0] - 1)*v[5];
	v[4] = v[4] + v[6];
	v[3] = v[3] + 1;
	}
	
v[7] = v[4]/v[2];


RESULT(v[7])


EQUATION("Gini_wages")
/*
Gini calculation of wages
*/

v[0] = V("N");
v[1] = SUM("w_it"); // Sum all income
v[2] = v[0]*v[1]; // Denominador
v[3] = 1; // Consumer index start from 1 to N
v[4] = 0; // Resultado do numerador
SORT( "Consumer", "w_it", "UP" ); // Values must be sorted

CYCLE(cur, "Consumer")
	{
	v[5] = VS(cur, "w_it") + 0.001; // Values cannot be zero
	v[6] = (2*v[3] - v[0] - 1)*v[5];
	v[4] = v[4] + v[6];
	v[3] = v[3] + 1;
	}
	
v[7] = v[4]/v[2];

RESULT(v[7])


EQUATION("Tax")
/*
Se flag_progress = 1, imposto progressivo. Caso contrário, flat
Se flag_transf = 1, transferência de renda
Faixas estão em termos de SM
*/

v[0] = V("flag_progress");
v[1] = V("flag_transf");
v[2] = V("faixa_1");
v[3] = V("faixa_2");
v[4] = V("faixa_3");
v[6] = V("tax_flat");
v[7] = V("tax_1");
v[8] = V("tax_2");
v[9] = V("tax_3");
v[10] = V("tax_transf");

if (v[0] == 0){
	v[20] = v[6];
	} else {
		v[5] = V("leque");
		if(v[5] < V("faixa_1") && V("flag_transf") == 1){
				v[20] = V("tax_transf"); // Deve ser negativo
		} else if (v[5] < V("faixa_1") && V("flag_transf") == 0) {
				v[20] = 0;
		} else if (v[5] >= V("faixa_1") && v[5] < V("faixa_2")){
				v[20] = V("tax_1");
		} else if (v[5] >= V("faixa_2") && v[5] < V("faixa_3")){
				v[20] = V("tax_2");
		} else {
			v[20] = V("tax_3");
		
	}
}
PARAMETER // Checar
RESULT(v[20])



MODELEND

// do not add Equations in this area

void close_sim( void )
{
	// close simulation special commands go here
}
