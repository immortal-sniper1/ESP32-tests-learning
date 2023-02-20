#ifndef EMA_FILTER_H
#define EMA_FILTER_H

//LOW pass filter
// Y(n)= a*X(n) + (1 - a) * Y(n - 1)
// Y(n) current output aka the new value
// Y(n-1) last output aka what we allready have in memory 
// X(n) current input sample (the thing that we just read with a ADC)
// a koeffcient from 0 to 1 , 0 so no foltering, 1 total filtering

typedef struct EMA_Low_Struct
{
	// the coefficient [0...1] 
	float alfa;  
	// the output
	float Filter_Output;
} 

void EMA_Init( EMA_Low_Struct  *filt, float alfa )
{
	// set alpha
	EMA_Set_Alpha( filt, alfa);

	//clear output
	filt->Filter_Output = 0.0f;
}


void EMA_Set_Alpha( EMA_Low_Struct  *filt, float alfa )
{
	if(alfa < 0.0f)
	{
		alfa = 0.0f;
	}

	if(alfa > 1.0f)
	{
		alfa = 1.0f;
	}

	filt->Filter_Output=alfa;
}


void EMA_Update( EMA_Low_Struct  *filt, float input )
{
	filt->Filter_Output = filt->alfa * input + (1.0f - filt->alfa ) * filt->Filter_Output ; 
	return filt->Filter_Output;
}


// High pass filter
// Y(n)= (2-b)*( X(n) - X(n-1) )/2 + (1 - b) * Y(n - 1)
// Y(n) current output aka the new value
// Y(n-1) last output aka what we allready have in memory 
// X(n) current input sample (the thing that we just read with a ADC)
// X(n-1) lasst input sample , stored in memory
// b koeffcient from 0 to 1 , 0 so no filtering, 1 total filtering

typedef struct EMA_High_Struct
{
	// the coefficient [0...1] 
	float beta;  
	// the output
	float Filter_Output;
	// the input
	float Filter_Input;
} 

void EMA_Init( EMA_High_Struct  *filt, float beta )
{
	// set alpha
	EMA_Set_Alpha( filt, beta);

	//clear output
	filt->Filter_Output = 0.0f;

	//clear input
	filt->Filter_Input = 0.0f;
}


void EMA_Set_Alpha( EMA_High_Struct  *filt, float beta )
{
	if(beta < 0.0f)
	{
		beta = 0.0f;
	}

	if(beta > 1.0f)
	{
		beta = 1.0f;
	}

	filt->Filter_Output=beta;
}

void EMA_Update( EMA_High_Struct  *filt, float input )
{
	filt->Filter_Output = 0.5f * (2.0f - filt->beta)* ( input - inp->Filter_Input ) + (1.0f - filt->beta ) * filt->Filter_Output ; 

    filt->Filter_Input = input;

	return filt->Filter_Output;
}



#endif 
