
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Include File...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"

#include "Transexe.h"
#include "models.h"

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Externals...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
extern D3DMATRIX identity;
extern D3DMATRIXHANDLE hWorld;
extern	MODEL	Models[MAXNUMOFMODELS];
extern	GLOBALSHIP	Ships[MAX_PLAYERS];

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Globals...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
int16	NumOfTransExe = 0;
TRANSEXE TransExe[MAXTRANSEXE];




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Add Translucent Execute Buffer...
		Input	: LPD3DMATRIX  Matrix...Only needed if ya need to do a compunded view Matrix
				  lpExBuf... POinter to Execution Buffer
				  int UseIdentity....set to 0 for normal background...1 for Bikes etc..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AddTransExe( LPD3DMATRIX  Matrix , LPDIRECT3DEXECUTEBUFFER lpExBuf , int UseIdentity, uint16 Model, uint16 group, int16 NumVerts )
{
	if( NumOfTransExe < MAXTRANSEXE	)
	{
		TransExe[NumOfTransExe].UseIdentity = UseIdentity;
		
		if( UseIdentity == 0 )
		{
			TransExe[NumOfTransExe].Matrix = *Matrix;
		}
		TransExe[NumOfTransExe].lpExBuf = lpExBuf;
		TransExe[NumOfTransExe].Model = Model;
		TransExe[NumOfTransExe].NumVerts = NumVerts;
		TransExe[NumOfTransExe].group = group;
		NumOfTransExe++;
	}
}



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Execute Translucent Execute Buffers for specific group...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void ExecuteTransExe( uint16 group )
{
	int16	i;
	uint16	Model;
	BOOL	Display;

	for( i = 0 ; i < NumOfTransExe ; i++ )
	{
		if ( TransExe[i].group == group )
		{
			Display = TRUE;

			if( TransExe[i].Model != (uint16) -1 )
			{
				Model = TransExe[i].Model;
				if( !( Models[ Model ].Flags & MODFLAG_Clip ) )
					continue; // do not display fields clipped

				if( ( Models[ Model ].Flags & MODFLAG_Light ) )
				{
					if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
				}

				switch( Models[ Model ].Func )
				{
					case MODFUNC_Explode:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
						break;
		
					case MODFUNC_Regen:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
						break;
					case MODFUNC_Scale:
					case MODFUNC_Scale2:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
						break;

					case MODFUNC_SphereZone:
						ProcessSphereZoneModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, (uint8) Models[ Model ].Red, (uint8) Models[ Model ].Green, (uint8) Models[ Model ].Blue );
						break;

					case MODFUNC_OrbitPulsar:
						if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
						{
							if( !LightModel( Model, &Models[ Model ].Pos ) ) Display = FALSE;
						}
						break;

					case MODFUNC_ScaleDonut:
//						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 1, 1 );
						break;
					
					case MODFUNC_Nothing:
					default:
						break;
				}
			}

			if( TransExe[i].UseIdentity	== 1 )
			{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
			}else{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &TransExe[i].Matrix);
			}

			if( Display )
					d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, TransExe[i].lpExBuf , d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED);
		}
	}
	d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
	
}


void ExecuteTransExeUnclipped( uint16 group )
{
	int16	i;
	uint16	Model;
	BOOL	Display;

	for( i = 0 ; i < NumOfTransExe ; i++ )
	{
		if ( TransExe[i].group == group )
		{
			Display = TRUE;

			if( TransExe[i].Model != (uint16) -1 )
			{
				Model = TransExe[i].Model;
				if( ( Models[ Model ].Flags & MODFLAG_Clip ) )
					continue; // do not display clipped models here

				if( ( Models[ Model ].Flags & MODFLAG_Light ) )
				{
					if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
				}

				switch( Models[ Model ].Func )
				{
					case MODFUNC_Explode:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
						break;
					case MODFUNC_Regen:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
						break;
		
					case MODFUNC_Scale:
					case MODFUNC_Scale2:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
						break;

					case MODFUNC_SphereZone:
						ProcessSphereZoneModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, (uint8) Models[ Model ].Red, (uint8) Models[ Model ].Green, (uint8) Models[ Model ].Blue );
						break;

					case MODFUNC_OrbitPulsar:
						if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
						{
							if( !LightModel( Model, &Models[ Model ].Pos ) ) Display = FALSE;
						}
						break;

					case MODFUNC_ScaleDonut:
//						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 1, 1 );
						break;
					
					case MODFUNC_Nothing:
					default:
						break;
				}
			}
			else
				continue; // do not display non-model translucencies

			if( TransExe[i].UseIdentity	== 1 )
			{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
			}else{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &TransExe[i].Matrix);
			}

			if( Display )
					d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, TransExe[i].lpExBuf , d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED);
		}
	}
	d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
	
}



