// *************************************************************************************
// ! Animation Graph Constants Table
// *************************************************************************************
class HumanCommandScript_DoggoPlrST
{
	void HumanCommandScript_DoggoPlrST(Human pHuman)
	{
		HumanAnimInterface 	hai = pHuman.GetAnimInterface();

		m_CmdStartControlDog 	= hai.BindCommand("CMD_Vehicle_GetIn");
		//m_CmdStartControlDog 	= hai.BindCommand("CMD_Vehicle_GetOut");			
	}	


	
	// TAnimGraphCommand 		m_CmdStartControlDog;		//! command to start swimming
	// TAnimGraphVariable 		m_VarSpeed;				//! float variable - 0,1,2,3 speed
	// TAnimGraphVariable 		m_VarDirection;			//! float variable - angle 
	// TAnimGraphTag			m_TagIsSwimming;		//! 
	int 					m_CmdStartControlDog;		//! command to start swimming

}



// *************************************************************************************
// ! Fully Working Flying implemented by Human Command Script
// *************************************************************************************
class HumanCommandScript_DoggoControl : HumanCommandScript
{

	//! constructor
	void 	HumanCommandScript_DoggoControl(DayZPlayer pPlayer, HumanCommandScript_DoggoPlrST pTable)
	{
		m_pPlayer 	= pPlayer;
		m_Table 	= pTable;
	}

	//! 
	override void 	OnActivate()
	{
		dBodyEnableGravity(m_pPlayer, false);
		PreAnim_CallCommand(m_Table.m_CmdStartControlDog, 0, 0);
	}

	override void 	OnDeactivate()
	{
		dBodyEnableGravity(m_pPlayer, true);
	}

	// called to set values to animation graph processing 
	override void 	PreAnimUpdate(float pDt)
	{
		
	}


	//! called to change local translations / rotations
	//! called to get events / tags from animation graph processing 
	override void 	PrePhysUpdate(float pDt)
	{
	
	}

	//! called when all animation / pre phys update is handled
	override bool	PostPhysUpdate(float pDt)
	{

		if (m_bNeedFinish)
		{
			return false;
		}

		return true;	// handled with SetFlagFinished();
	}


	DayZPlayer 							m_pPlayer;
	HumanCommandScript_DoggoPlrST 	m_Table;
	HumanInputController 				m_Input;
	float 								m_fTime;	
	bool								m_bNeedFinish;

}




modded class DayZPlayerImplement
{
	ref HumanCommandScript_DoggoPlrST	m_DoggoPlrST;

	//!
	/*
	void 	InitTables()
	{
		if (m_DoggoPlrST == null)
		{
			m_DoggoPlrST = new HumanCommandScript_DoggoPlrST(this);
		}
	}
	*/
	void StartCommand_DogControl()
	{
		if ( !m_DoggoPlrST )
			m_DoggoPlrST = new HumanCommandScript_DoggoPlrST( this );

		StartCommand_Script( new HumanCommandScript_DoggoControl(this, m_DoggoPlrST) );
	}

	//! override 
	override bool ModCommandHandlerAfter (float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//! MUST BE HERE TO NOT DISABLE OTHER MODS
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}
		
		//! 
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT)
		{
			HumanCommandScript hcs = GetCommand_Script();	
		
			if (HumanCommandScript_DoggoControl.Cast(hcs) != null)
			{
				// we are running  HumanCommandScript_SwimSample
				// Print ("HumanCommandScript_SwimSample Cast Working");

				return true;		// is handled 
			}

			return false;			// not handled
	
		}

		// check water levels
		{

			if (this.GetParent() && Dayz_Doggo.Cast(this.GetParent()))
			{
				//! init tables - done just once
				//InitTables();

				//! allocate / new swim sample
            	//HumanCommandScript_DoggoControl cmdSSS = new HumanCommandScript_DoggoControl(this, m_DoggoPlrST);
			
				// start it now
				//StartCommand_Script(cmdSSS);
				StartCommand_DogControl();
				return true;
			}
		}
		
		return false;	// not handled
	}
}